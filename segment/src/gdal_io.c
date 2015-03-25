#include "segment.h"

#include "gdal.h"
/*
#ifndef LINT_CAST
#ifdef  lint
#define LINT_CAST(e)    ( (e) ? 0 : 0 )
#else
#define LINT_CAST(e)    ( e )
#endif
#endif

typedef unsigned char uchar_t;
*/
/*

Need to replace:
    1. wr_region_map
    2. wr_armm
    3. wr_cband

*/

void
GDAL_process_headers(Spr, image_fn, mask_fn)
Seg_proc Spr;
char *image_fn;
char *mask_fn;
{
    GDALAllRegister();

    /*
     * Open image dataset within GDAL and process metadata
     */
    GDALDatasetH hDataset = NULL;
    GDALRasterBandH hBand = NULL;

    hDataset = GDALOpen(image_fn, GA_ReadOnly);
    if( hDataset == NULL ) {
        error("Can't open input image file \"%s\"", image_fn);
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
    if (strlen(mask_fn) > 0){
        hDataset = GDALOpen(mask_fn, GA_ReadOnly);
        if( hDataset == NULL ) {
            error("Can't open input image file \"%s\"", mask_fn);
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
