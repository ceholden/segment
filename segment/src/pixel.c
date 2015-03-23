#ifndef lint
static char     SCCS_ID[] = "pixel.c 2.3  5/5/89";

#endif

#include "segment.h"

/*
 *	Some comments on the semantics of the contiguity band.  At certain
 *	stages of the algorithm, we want to store information for each
 *	pixel about its 8 neighboring pixels.  8 neighbors maps very
 *	neatly onto 8 bits.  What isn't quite as neat is that we want
 *	to record different information at different points of the
 *	algorithm.  And in the general pass of the algorithm, we would
 *	really like to store more than one byte of state information.
 *	But we get by with one byte, one bit per direction, and the following
 *	changing and slightly tricky semantics for the sake of storage and
 *	speed.
 *
 *	pix_nnbr() uses the cband to indicate which neighboring pixels
 *	are at the minimum distance from the base pixel.  It is very
 *	possible that a pixel may have two or more nearest neighbors.
 *
 *	pix_merge() selects one of the nearest neighbors and guarantees
 *	that for each pixel, at most one of the cband cdir_f's will be
 *	set.  This property is important to make_region_list(), which uses
 *	the cband to determine which pixels to merge with which.
 *
 *	make_region_list() calls pix_check_bounds_and_mask() to adjust
 *	the semantics once more.  The general pass of the segmentation
 *	algorithm checks repeatedly for each pixel (1) which 8-way neighbors
 *	of the pixel belong to other regions and (2) which neighbors are out
 *	of bounds.
 *
 *	To compress this information into 1 byte, pix_check_bounds_and_mask()
 *	makes each bit of the cband byte for a pixel 1 if the neighboring
 *	pixel in that direction is out of bounds or masked out or if it
 *	belongs to a pixel of the same region.  Thus, if it is 0, the
 *	corresponding neighbor pixel is in bounds and belongs to a different
 *	region.
 */


 /*
  * Table of pixel offsets by orientation 
  */

cdelta          cd4_cdelta[4] = {
    {0, -1},			/* N */
    {1, 0},			/* E */
    {0, 1},			/* S */
    {-1, 0}			/* W */
};

cdelta          cd8_cdelta[8] = {
    {0, -1},			/* N */
    {1, -1},			/* NE */
    {1, 0},			/* E */
    {1, 1},			/* SE */
    {0, 1},			/* S */
    {-1, 1},			/* SW */
    {-1, 0},			/* W */
    {-1, -1}			/* NW */
};

cdelta         *cdir_cdeltas;

cdir_f          cd4_flag[4] = {
    DF_N,
    DF_E,
    DF_S,
    DF_W
};

cdir_f          cd8_flag[8] = {
    DF_N,
    DF_NE,
    DF_E,
    DF_SE,
    DF_S,
    DF_SW,
    DF_W,
    DF_NW
};

cdir_f         *cdir_flags;

int             Ncdir;
cd_map          Cinternal;

 /*
  * Masks for 8 way boundary segments
  */

static cd_map   N_EDGE = DF_NW | DF_N | DF_NE;
static cd_map   E_EDGE = DF_NE | DF_E | DF_SE;
static cd_map   S_EDGE = DF_SE | DF_S | DF_SW;
static cd_map   W_EDGE = DF_SW | DF_W | DF_NW;

static void     pix_nnbr();
static void     pix_merge();
static long     pix_dist2();
static void     pix_check_bounds_and_mask();

/*
 *	Allocate storage for an image (input or mask), read it in, and set up
 *	pointers to the start of scan lines.
 */

uchar_t       **
read_image(fd, nbands, nlines, nsamps)
int             fd;
int             nbands;
int             nlines;
int             nsamps;
{
    int             bytes_read;	/* total image bytes available */
    uchar_t       **image;	/* ptr to the doped array for image */
    int             image_size;	/* size in bytes of image to be segmented */
    REG_1 int       line;	/* line index */
    int             line_size;	/* size in bytes of 1 scanline of image */
    REG_2 uchar_t  *row_p;	/* ptr to a scan line */
    int             stored_image_size;	/* size in bytes of image and row
					 * ptrs */

    line_size = nbands * nsamps;
    image_size = nlines * line_size;
    stored_image_size = image_size + nlines * sizeof(uchar_t *);


    image = (uchar_t **) LINT_CAST(ecalloc(stored_image_size, 1));
    if (image == NULL)
	error("can't allocate space for image");

 /*
  * Initialize the image scanline pointers
  */
    row_p = (uchar_t *) LINT_CAST(image + nlines);
    for (line = 0; line < nlines; line++) {
	image[line] = row_p;
	row_p += line_size;
    }

 /*
  * Read in image
  */
    if ((bytes_read = uread(fd,
			    (addr_t) LINT_CAST(image[0]),
			    image_size))
	!= image_size) {
	error("image read failed, bytes read = %d", bytes_read);
    }
    return (image);
}

/*
 *	Free doped (input or mask) image.
 */

void
free_image(img)
uchar_t **img;
{
/* NOSTRICT */
    free((char *) img);
}

/*
 *	void
 *	pixel_pass( Seg_proc Spr )
 *
 *	Conduct two passes over the original image.  During the first
 *	calculate the nearest neighbor(s) of every pixel in the image
 *	and record them in the contiguity band (Spr->cband).  During
 *	the second pass, determine which pairs of pixels will merge
 *	before the initial region list is formed.  In order for pixels
 *	to be merged, each must be a nearest neighbor of the other, and
 *	their distance from each other must be less than the merge
 *	tolerance, Tg.  A pixel is only allowed to merge with one other
 *	pixel at this stage.
 */

void
pixel_pass(Spr)
Seg_proc        Spr;
{

    pix_nnbr(Spr);
    printf("Completed the calculation of pixel nearest neighbors\n");
    pix_merge(Spr);

}


/*
 *	void
 *	pixel_nnbr(Seg_proc Spr)
 *
 *	Calculate the nearest neighbors of every pixel in the image. On
 *	this pass the contiguity band is used to record those pixels
 *	which are nearest neighbors of a given pixel if they are closer
 *	than the merge tolerance, Tg.  A set_nnbr(map, dir) sets the
 *	appropriate flag to indicate that the pixel in the relative
 *	direction dir is an apppropriate nearest neighbor to the pixel
 *	corresponding to cd_map map.  If a mask band is specified,
 *	this routine will check to make sure that no merges with
 *	masked out portions of the image will occur.
 */

static void
pix_nnbr(Spr)
REG_2 Seg_proc  Spr;
{
    REG_1 cdir      d;
    int             l;
    int             s;
    pcoord          cpix;
    pcoord          npix;
    cd_map        **cband;
    REG_4 Ipixel    Cpixv;
    REG_5 Ipixel    Npixv;
    REG_6 long      mdist2;
    long            ndist2[MAX_CDIR];
    REG_3 cd_map   *Cur_map;
    int             nbands;
    cbounds         bounds;
    bool_t          masked;



    nbands = Spr->nbands;
    bounds.x = Spr->nsamps;
    bounds.y = Spr->nlines;
    cband = Spr->cband;
    masked = sf_get(Spr, SF_MASK);

    for (l = 0; l < Spr->nlines; l++) {
	for (s = 0; s < Spr->nsamps; s++) {

	    cpix.x = s;
	    cpix.y = l;
	    Cpixv = pcoord_to_pix(Spr, cpix);
	    Cur_map = &cband[l][s];
	    *Cur_map = CCLEAR;
	    if (masked && !pcoord_to_mask(Spr, cpix))
		continue;
	    mdist2 = MAXLONG;

	    for (d = 0; d < Ncdir; d++) {
		set_pcoord_from_dir(npix, cpix, d);
		if (!pcoord_in_bounds(npix, bounds) ||
		    (masked && !pcoord_to_mask(Spr, npix))) {
		    ndist2[d] = MAXLONG;
		} else {
		    Npixv = pcoord_to_pix(Spr, npix);
		    ndist2[d] = pix_dist2(nbands,
					  Cpixv,
					  Npixv);
		}
		mdist2 = MIN(mdist2, ndist2[d]);
	    }

	    if (mdist2 <= Spr->tg2) {
		for (d = 0; d < Ncdir; d++) {
		    if (ndist2[d] == mdist2)
			set_nnbr(*Cur_map, d);
		}
	    }
	}
    }
}

/*
 *	void
 *	pix_merge( Seg_proc Spr )
 *
 *	Determine which pairs of pixels will merge before forming the region
 *	list.  We separate this pass out so that we create as small a region
 *	list as possible.
 */

static void
pix_merge(Spr)
Seg_proc        Spr;
{
    REG_1 cdir      d;
    REG_2 int       l;
    REG_3 int       s;
    pcoord          cpix;
    pcoord          npix;
    REG_4 cd_map   *Cur_map;
    REG_5 cd_map  **cband;
    REG_6 REGION_ID **rband;
    bool_t          merged;
    bool_t          masked;
    long            nregions;
    cdir            idir;

    cband = Spr->cband;
    rband = Spr->rband;
    masked = sf_get(Spr, SF_MASK);

    nregions = 0;
    idir = 0;
    for (l = 0; l < Spr->nlines; l++) {
	for (s = 0; s < Spr->nsamps; s++) {

	    if ((masked && !pixmask(Spr, l, s)) ||	/* Is this pixel masked
							 * out or */
		rband[l][s] > 0)/* is this pixel already merged? */
		continue;	/* Yes */

	/*
	 * Does this pixel have an appropriate nearest neighbor?
	 */
	    Cur_map = &cband[l][s];
	    if (*Cur_map == CCLEAR) {

	    /*
	     * No.  Assign the pixel a region ID and continue with the next
	     * pixel.
	     */
		rband[l][s] = ++nregions;
		continue;
	    }

	/*
	 * Yes
	 */
	    d = idir;
	    cpix.x = s;
	    cpix.y = l;
	    merged = FALSE;

	/*
	 * Examine the neighboring pixels in every direction.  Note that we
	 * don't have to check if the neighboring pixel is in bounds or not
	 * masked out since the contiguity band now indicates whether it is
	 * accessible.
	 */
	    while (advance_dir(d) != idir) {

	    /*
	     * Is the neighboring pixel a nearest neighbor?
	     */
		if (!has_nnbr(*Cur_map, d))
		    continue;	/* No */

	    /*
	     * Yes.  Is this nearest neighbor already merged?
	     */
		set_pcoord_from_dir(npix, cpix, d);
		if (rband[npix.y][npix.x] > 0)
		    continue;	/* Yes */

	    /*
	     * No.  Is the original pixel a nearest neighbor of this nearest
	     * neighbor?
	     */
		if (has_nnbr(cband[npix.y][npix.x], dir_reverse(d))) {

		/*
		 * Yes.  Assign these two pixels to a new region.
		 */
		    rband[l][s] = rband[npix.y][npix.x] = ++nregions;
		    *Cur_map = cdir_flags[d];
		    cband[npix.y][npix.x] = cdir_flags[dir_reverse(d)];

		/*
		 * And update idir so we start the nearest neighbor search
		 * from another orientation on the next pixel.
		 */
		    idir = d;

		    merged = TRUE;
		    break;

		} else {

		/*
		 * No.  Check the remaining neighboring pixels.
		 */
		    continue;
		}
	    }

	/*
	 * If this pixel has not been able to merge with a nearest neighbor,
	 * assign it a region ID and set its contiguity map to 0, indicating
	 * no contiguous pixels in the same region.
	 */
	    if (!merged) {
		*Cur_map = CMONO;
		rband[l][s] = ++nregions;
	    }
	}
    }

    Spr->maxreg = Spr->nreg = nregions;
}

/*
 *	Make up the initial region list from the image pixels and the
 *	associated contiguity, region, and mask (if present) bands.
 */

void
make_region_list(Spr)
REG_5 Seg_proc  Spr;
{
    REGION_ID       dummy;
    int             l;
    int             s;
    pcoord          cpix;
    pcoord          npix;
    REG_1 REGION_ID rid;
    REG_2 Region    R;
    REG_3 cdir_f    cdf;
    REG_4 cdir      d;

    dummy = Spr->nreg + 1;

    for (l = 0; l < Spr->nlines; l++) {
	for (s = 0; s < Spr->nsamps; s++) {

	    cpix.x = s;
	    cpix.y = l;

	/*
	 * If pixel is masked out (REGION_ID == 0), we are done. 
	 */
	    if (!(rid = pcoord_to_regid(Spr, cpix)))
		continue;
	    R = &regid_to_reg(Spr, rid);

	    if (rf_get(R, RF_ACTIVE))
		continue;
	    else if (cdf = pcoord_to_cdm(Spr, cpix)) {
		set_dir_from_cdf(d, cdf);
		set_pcoord_from_dir(npix, cpix, d);
		region_from_pixel(Spr, rid, cpix);
		region_from_pixel(Spr, dummy, npix);
		pix_check_bounds_and_mask(Spr, cpix);
		pix_check_bounds_and_mask(Spr, npix);
		merge_regions(Spr, rid, dummy);
	    } else {
		region_from_pixel(Spr, rid, cpix);
		pix_check_bounds_and_mask(Spr, cpix);
	    }
	}
    }
}

/*
 *	If the pixel specified by the pcoord pc is on a boundary of the
 *	image, set the appropriate flags in the contiguity band for the pixel
 *	so that the region routines will ignore neighboring pixels that are
 *	out of bounds.  Likewise, if the pixel is on the boundary of a
 *	masked area, set the contiguity band so the region routines will
 *	never attempt to merge with a masked pixel.
 */
 
static void
pix_check_bounds_and_mask(Spr, pc)
REG_2 Seg_proc  Spr;
pcoord          pc;
{
    REG_1 cd_map   *Curmap;
    REG_3 cdir      d;
    pcoord          npix;

    Curmap = &pcoord_to_cdm(Spr, pc);

    if (pc.y == 0)
	*Curmap |= N_EDGE;

    if (pc.y == Spr->nlines - 1)
	*Curmap |= S_EDGE;

    if (pc.x == 0)
	*Curmap |= W_EDGE;

    if (pc.x == Spr->nsamps - 1)
	*Curmap |= E_EDGE;

 /*
  * If there is a mask image, check to see if this pixel is next to any pixel
  * that has been masked out. 
  */
    if (sf_get(Spr, SF_MASK)) {
	for (d = 0; d < Ncdir; d++) {

	/*
	 * For every neighbor that is in bounds but not part of this region, 
	 */
	    if (!has_contig(*Curmap, d)) {
		set_pcoord_from_dir(npix, pc, d);

	    /*
	     * Is it masked out? 
	     */
		if (!pcoord_to_mask(Spr, npix))

		/*
		 * Yes.  Set contiguity band bit so we never try to merge
		 * with it. 
		 */
		    set_nnbr(*Curmap, d);
	    }
	}
    }
}


/*
 *	Return the distance (in pixel space) squared between two pixel
 *	vectors.
 */
static long
pix_dist2(nb, Apixv, Bpixv)
REG_6 int       nb;
REG_1 Ipixel    Apixv;
REG_2 Ipixel    Bpixv;
{
    REG_3 int       b;
    REG_4 long      dist2 = 0;
    REG_5 long      diff;

    for (b = 0; b < nb; b++) {
	diff = Apixv[b] - Bpixv[b];
	dist2 += diff * diff;
    }
    return (dist2);
}
