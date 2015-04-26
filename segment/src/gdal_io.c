#include "segment.h"


uchar_t **
GDAL_read_image(hDataset)
GDALDatasetH hDataset;
{
    uchar_t **image;
    int nlines, nsamps, nbands, nbytes;
    int image_size;
    int line_size;
    int stored_image_size;
    uchar_t *row_p;
    GDALRasterBandH hBand;

    hBand = GDALGetRasterBand(hDataset, 1);

    nlines = GDALGetRasterYSize(hDataset);
    nsamps = GDALGetRasterXSize(hDataset);
    nbands = GDALGetRasterCount(hDataset);
    nbytes = GDALGetDataTypeSize(GDALGetRasterDataType(hBand)) / 8;
    line_size = nbands * nsamps;
    image_size = line_size * nlines;
    stored_image_size = image_size + nlines * sizeof(uchar_t *);

    image = (uchar_t **) LINT_CAST(ecalloc(stored_image_size, 1));
    if (image == NULL)
        error("Can't allocate space for image");

    // Initialize image scanline pointers
    row_p = (uchar_t *) LINT_CAST(image + nlines);
    int line;
    for (line = 0; line < nlines; line++) {
        image[line] = row_p;
        row_p += line_size;
    }

    printf("Trying to read in image\n");
    CPLErr err = CE_None;
    // Read in image into BIP memory layout
    int b;
    for (b = 0; b < nbands; b++) {
        hBand = GDALGetRasterBand(hDataset, b + 1);

        err = GDALRasterIO(hBand, GF_Read,
            0, 0, nsamps, nlines,
            image[0] + b,
            nsamps * nbytes, nlines * nbytes,
            GDT_Byte,
            nbands * nbytes, nsamps * nbands * nbytes);
    }

    printf("Read in data\n");

    return (image);
}


void
GDAL_write_image(Spr, fname)
Seg_proc Spr;
char *fname;
{
    int nbits, nbytes;
    long nregions;
    int err;

    GDALAllRegister();

    // Figure out minimum datatype to use
    GDALDataType eBufType;
    for (nbits = 0, nregions = Spr->nreg; nregions; nbits++, nregions >>= 1);
    if (nbits <= GDALGetDataTypeSize(GDT_Byte)) {
        eBufType = GDT_Byte;
        nbytes = 1;
    } else if (nbits <= GDALGetDataTypeSize(GDT_UInt16)) {
        eBufType = GDT_UInt16;
        nbytes = 2;
    } else if (nbits <= GDALGetDataTypeSize(GDT_UInt32)) {
        eBufType = GDT_UInt32;
        nbytes = 4;
    // } else if (nbits <= GDALGetDataTypeSize(GDT_UInt64)) {
    //     eBufType = GDT_UInt64;
    } else {
        error("Cannot determine datatype\n");
    }

    // Using ENVI driver by default for now
    GDALDriverH hDriver = GDALGetDriverByName("ENVI");
    GDALDatasetH hDstDs;
    GDALRasterBandH hBand;
    char **papszOptions = NULL;

    hDstDs = GDALCreate(hDriver, fname,
                        Spr->nlines, Spr->nsamps, 1,
                        eBufType, papszOptions);
    hBand = GDALGetRasterBand(hDstDs, 1);


    double        adfGeoTransform[6];


    // Write out
    // err = GDALRasterIO(hBand, GF_Write,
    //                    0, 0, Spr->nsamps, Spr->nlines,
    //                    Spr->rband,
    //                    Spr->nsamps, Spr->nlines,
    //                    eBufType,
    //                    0, 0);
    int l;
    for (l = 0; l < Spr->nlines; l++) {
        GDALRasterIO(hBand, GF_Write,
                     0, l, Spr->nsamps, 1,
                     Spr->rband[l],
                     Spr->nsamps * nbytes, 1,
                     eBufType,
                     0, 0);
    }

    // Set projection & geotransform before closing
    GDALSetGeoTransform(hDstDs, Spr->adfGeoTransform);
    GDALSetProjection(hDstDs, Spr->pszProjection);

    GDALClose(hDstDs);
}


void
GDAL_process_headers(Spr)
Seg_proc Spr;
{
    GDALAllRegister();

    /*
     * Open image dataset within GDAL and process metadata
     */
    GDALDatasetH hDataset = NULL;
    GDALRasterBandH hBand = NULL;

    hDataset = GDALOpen(Spr->image_fn, GA_ReadOnly);
    if( hDataset == NULL ) {
        error("Can't open input image file \"%s\"", Spr->image_fn);
    }

    Spr->nlines = GDALGetRasterYSize(hDataset);
    if (Spr->nlines > MAXSHORT)
        error("Image has too many (%d) lines\n", Spr->nlines);
    Spr->nsamps = GDALGetRasterXSize(hDataset);
    if (Spr->nsamps > MAXSHORT)
        error("Image has too many (%d) samps\n", Spr->nsamps);
    Spr->nbands = GDALGetRasterCount(hDataset);
    if (Spr->nbands > MAXSHORT)
        error("Image has too many (%d) bands\n", Spr->nbands);
    const char *pszProjection;
    pszProjection = GDALGetProjectionRef(hDataset);
    Spr->pszProjection = malloc(sizeof(char) * strlen(pszProjection));
    strcpy(Spr->pszProjection, pszProjection);
    if (Spr->pszProjection == NULL || strlen(Spr->pszProjection) == 0)
        warn("Could not get image projection\n");
    printf("Projection is: %s\n", Spr->pszProjection);
    GDALGetGeoTransform(hDataset, Spr->adfGeoTransform);

    hBand = GDALGetRasterBand(hDataset, 1);

    Spr->image = GDAL_read_image(hDataset);

    const char *type = GDALGetDataTypeName(GDALGetRasterDataType(hBand));
    if (strcmp(type, "Byte") != 0) {
        error("Image must be Byte datatype, not %s\n", type);
    }
    GDALClose(hDataset);

    /*
     * Open mask image dataset within GDAL and process metadata
     */
    if (strlen(Spr->mask_fn) > 0){
        hDataset = GDALOpen(Spr->mask_fn, GA_ReadOnly);
        if( hDataset == NULL ) {
            error("Can't open input image file \"%s\"", Spr->mask_fn);
        }

        if (GDALGetRasterYSize(hDataset) != Spr->nlines)
            error("Input and mask images have different number of lines");
        if (GDALGetRasterXSize(hDataset) != Spr->nsamps)
            error("Input and mask images have different number of samples");
        if (GDALGetRasterCount(hDataset) != 1)
            error("Input mask image must have 1 band");
        hBand = GDALGetRasterBand(hDataset, 1);
        if (GDALGetDataTypeName(GDALGetRasterDataType(hBand)) != "Byte")
            error("The mask image is not 1 byte per pixel");

        Spr->imask = GDAL_read_image(Spr->mask_fn);

        GDALClose(hDataset);

    }

}
