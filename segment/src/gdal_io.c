#include "segment.h"

#include "gdal.h"


uchar_t **
GDAL_read_image(filename)
char* filename;
{
    uchar_t **image;
    int nlines, nsamps, nbands, nbytes;
    int image_size;
    int line_size;
    int stored_image_size;
    uchar_t *row_p;
    GDALDatasetH hDataset;
    GDALRasterBandH hBand;

    hDataset = GDALOpen(filename, GA_ReadOnly);
    if( hDataset == NULL )
        error("Can't open input image file \"%s\"", filename);

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

    // Temporary buffer for scanline
    uchar_t *scanline;
    scanline = (uchar_t *) malloc(nsamps * nbytes);

    printf("Trying to read in image\n");
    CPLErr err = CE_None;
    // Read in image into BIP memory layout
    int b;
    for (b = 0; b < nbands; b++) {
        hBand = GDALGetRasterBand(hDataset, b + 1);

        // TODO: try to read directly into BIP formatted "image" buffer
        for (line = 0; line < nlines; line++) {
            err = GDALRasterIO(hBand, GF_Read,
                0, line, nsamps, 1,
                scanline, nsamps, 1, GDT_Byte, 0, 0);
            if (err != CE_None)
                error("Could not read in image");
            memcpy(image[line], scanline, nsamps * nbytes);
        }
    }

    printf("Read in data\n");

    return (image);
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

    hBand = GDALGetRasterBand(hDataset, 1);

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

        GDALClose(hDataset);

    }

}
