#ifndef lint
static char     SCCS_ID[] = "region.c 2.10  9/5/94";

#endif

#include "segment.h"
#include "set.h"

#define flip()	(random() & 01)

#define N_DHISTBINS	1000

static float    binwidth2;
static long     dhbin[N_DHISTBINS + 1];

static void     reg_nnbr();
static float    reg_dist2();
static void     clear_d2hist();
static void     get_tp2();
static void     hit_d2hist();
static void	mark_reg_in_image();

/*
 *	void
 *	region_from_pixel(Spr, rid, pc)
 *	REG_5 Seg_proc  Spr;
 *	REGION_ID       rid;
 *	pcoord          pc;
 *
 *	Create a one pixel region from pixel pc at index rid in the region list
 *	associated with Spr.
 */

void
region_from_pixel(Spr, rid, pc)
REG_5 Seg_proc  Spr;
REGION_ID       rid;
pcoord          pc;
{
    REG_1 Region    R;
    REG_2 float    *Ctr;
    REG_3 Ipixel    P;
    REG_4 int       band;

    R = &regid_to_reg(Spr, rid);
    Ctr = regid_to_Ctr(Spr, rid);
    P = pcoord_to_pix(Spr, pc);

    R->uleft = R->lright = pc;
    R->npix = 1;
    R->rflags = RF_ACTIVE;

    for (band = 0; band < Spr->nbands; band++)
	Ctr[band] = P[band];
}

/*
 *	Merge region r2 into region r1 and unset the RF_ACTIVE flag for
 *	region r2.  This involves changing the REGION_ID for r2 in the
 *	region map, and updating the contiguity band for the pixels in
 *	the new merged region.
 */

void
merge_regions(Spr, r1, r2)
REG_1 Seg_proc  Spr;
REGION_ID       r1;
REGION_ID       r2;
{
    REG_2 Region    R1;
    Region          R2;
    long            mpix;

    assert(r1 != r2);

    R1 = &regid_to_reg(Spr, r1);
    R2 = &regid_to_reg(Spr, r2);

 /*
  * Update the centroid vector to combine the weighted data from both
  * regions. 
  */
    {
	float          *Ctr1 = regid_to_Ctr(Spr, r1);
	float          *Ctr2 = regid_to_Ctr(Spr, r2);
	int             band;

	for (band = 0; band < Spr->nbands; band++) {
	    Ctr1[band] = (R1->npix * Ctr1[band] + R2->npix * Ctr2[band])
		/ (R1->npix + R2->npix);
	}
    }

 /*
  * Update the bounding box for the merged region, and make the new pixel
  * count the sum of the previous counts. 
  */
    R1->uleft.x = MIN(R1->uleft.x, R2->uleft.x);
    R1->uleft.y = MIN(R1->uleft.y, R2->uleft.y);
    R1->lright.x = MAX(R1->lright.x, R2->lright.x);
    R1->lright.y = MAX(R1->lright.y, R2->lright.y);
    if ((mpix = R1->npix + R2->npix) > MAX_USHORT) {
	error("Merged region too large (%d pixels) from regions %d and %d\n",
	      mpix, r1, r2);
    } else {
	R1->npix = mpix;
    }

 /*
  * Check every pixel in the bounding box of the merged region.
  * If a pixel belongs to either merging region, if it is a boundary pixel,
  * and if it is contiguous to a pixel of the other merging region, then
  * adjust the contiguity band as necessary. 
  */
    {
	REG_3 int       l;
	REG_4 int       s;
	REG_5 cd_map   *Curmap;
	REG_6 cdir      d;
	REGION_ID       r;

	for (l = R1->uleft.y; l <= R1->lright.y; l++) {
	    for (s = R1->uleft.x; s <= R1->lright.x; s++) {

		if (regid(Spr, l, s) != r1 && regid(Spr, l, s) != r2)
		    continue;
		Curmap = &contig(Spr, l, s);
		if (*Curmap == Cinternal)
		    continue;

		for (d = 0; d < Ncdir; d++) {
		    if (!has_contig(*Curmap, d)) {
			r = regid(Spr,
				  l + cdir_to_delta(d).y,
				  s + cdir_to_delta(d).x);
			if (r == r1 || r == r2) {
			    set_contig(*Curmap, d);
			}
		    }
		}

	    }
	}
    }

 /*
  * Change the REGION_ID for the pixels in r2 to that for r1 
  */
    {
	REG_3 int       l;
	REG_4 int       s;

	for (l = R2->uleft.y; l <= R2->lright.y; l++) {
	    for (s = R2->uleft.x; s <= R2->lright.x; s++) {
		if (regid(Spr, l, s) == r2)
		    regid(Spr, l, s) = r1;
	    }
	}
    }
    R2->rflags = RF_CLEAR;
}

/*
 *	Garbage collect the region and centroid lists and truncate the
 *	neighbor list so that there are no inactive regions.  Update
 *	the region band with the new IDs.  Use the neighbor list as a
 *	temporary translation table between the old and new IDs.
 */

void
compact_region_list(Spr)
REG_1 Seg_proc  Spr;
{
    long            new_nreg;

    {
	REG_2 REGION_ID nrid;
	REG_3 REGION_ID crid;
	REG_4 Region    R;
	int             band;

    /*
     * For every region, reassign REGION_ID so that all active regions have
     * contiguous REGION_IDs starting with 1.  Rearrange the region and
     * neighbor lists appropriately.  The case where crid == 0, will
     * establish a translation 0->0 for REGION_ID 0 (masked pixels if
     * present).
     */
	for (nrid = 1, crid = 0; crid <= Spr->maxreg; crid++) {
	    R = &regid_to_reg(Spr, crid);
	    if (!rf_get(R, RF_ACTIVE)) {
		regid_to_newid(Spr, crid) = 0;
		continue;
	    } else {
		if (crid != nrid) {
		    regid_to_reg(Spr, nrid) = *R;
		    for (band = 0; band < Spr->nbands; band++) {
			regid_to_Ctr(Spr, nrid)[band] =
			    regid_to_Ctr(Spr, crid)[band];
		    }

		}

	    /*
	     * Use the neighbor list to build a temporary translation table
	     * between the old and new REGION_IDs.
	     */
		regid_to_newid(Spr, crid) = nrid++;
	    }
	}
	new_nreg = nrid - 1;
    }

    {
	REG_2 int       l;
	REG_3 int       s;

    /*
     * For every pixel, insert the new REGION_ID in the region band.
     */
	for (l = 0; l < Spr->nlines; l++) {
	    for (s = 0; s < Spr->nsamps; s++) {
		regid(Spr, l, s) = regid_to_newid(Spr, regid(Spr, l, s));
	    }
	}
    }

 /*
  * Truncate the compacted region and centroid lists, freeing the inactive
  * portions of the lists.  Truncate the neighbor list similarly.
  */

    Spr->rlist = (Region) LINT_CAST(realloc((char *) Spr->rlist,
			       (unsigned) (new_nreg + 1) * sizeof(region)));
    if (Spr->rlist == NULL)
	error("Realloc failed for region list");

    Spr->ctrlist = (float *) LINT_CAST(realloc((char *) Spr->ctrlist,
		  (unsigned) (new_nreg + 1) * Spr->nbands * sizeof(float)));
    if (Spr->ctrlist == NULL)
	error("Realloc failed for centroid list");

    Spr->nnbrlist = (Neighbor) LINT_CAST(realloc((char *) Spr->nnbrlist,
			     (unsigned) (new_nreg + 1) * sizeof(neighbor)));
    if (Spr->nnbrlist == NULL)
	error("Realloc failed for neighbor list");

 /*
  * Record the new length of the lists.
  */
    Spr->maxreg = new_nreg;
}

/*
 *	Perform a general pass over the region list.
 */

void
seg_pass(Spr)
REG_1 Seg_proc  Spr;
{
    REG_2 REGION_ID r;
    REG_3 Region    R;
    bool_t          d2hist;	/* TRUE ==> Use histogram and merge
				 * coefficient to compute pass tolerance */
    float           nnbr_d2;
    REG_5 REGION_ID nnbr_id;
    REG_6 Region    Rnnbr;

    d2hist = sf_get(Spr, SF_HIST);
    if (d2hist) {

    /*
     * Clear nearest neighbor distance squared (NND2) histogram if we have to
     * calculate a pass tolerance.
     */
	clear_d2hist();
    }

 /*
  * For every region,
  */
    for (r = 1; r <= Spr->maxreg; r++) {
	R = &regid_to_reg(Spr, r);
	if (!rf_get(R, RF_ACTIVE))	/* Is this region inactive? */
	    continue;		/* Yes. Skip it. */
	rf_unset(&regid_to_reg(Spr, r), RF_MERGE);	/* Clear merge flag */
	reg_nnbr(Spr, r);	/* Find region's nearest neighbor */
	if (d2hist)		/* Enter dist2 in NND2 histogram if needed */
	    hit_d2hist(regid_to_nnbr(Spr, r).nbr_d2);
    }

 /*
  * Calculate the pass tolerance if we are using the NND2 histogram
  */
    if (d2hist)
	get_tp2(Spr);

    Spr->dmin2 = MAXFLOAT;
    Spr->maxpix = 0;
    Spr->merge_attempts = 0;
    Spr->nnbr_gone = 0;
    Spr->merging = 0;
    Spr->wrong_partner = 0;
    Spr->nnbr_d2_big = 0;
    Spr->npix_big = 0;
    Spr->both_viable = 0;
    Spr->no_nbr = 0;

 /*
  * For every region,
  */
    for (r = 1; r <= Spr->maxreg; r++) {
	R = &regid_to_reg(Spr, r);

    /*
     * Is this region inactive?  Nb, we do not have to check if the region
     * has merged since two regions always merge into the one with the lower
     * REGION_ID.  The other becomes inactive.
     */
	if (!rf_get(R, RF_ACTIVE))
	    continue;		/* Yes, skip it */

	nnbr_id = regid_to_nnbr(Spr, r).nbr_id;
    /*
     * If the region has no nearest neighbors, continue.
     */
        if (nnbr_id == 0) {
	    Spr->no_nbr++;
	    continue;
	}
	nnbr_d2 = regid_to_nnbr(Spr, r).nbr_d2;
	Rnnbr = &regid_to_reg(Spr, nnbr_id);
	Spr->dmin2 = MIN(Spr->dmin2, nnbr_d2);
	Spr->merge_attempts++;

    /*
     * Is the nearest neighbor region inactive or has it already merged?
     */
	if (!rf_get(Rnnbr, RF_ACTIVE) || rf_get(Rnnbr, RF_MERGE)) {
	    Spr->maxpix = MAX(Spr->maxpix, R->npix);
	    Spr->nnbr_gone++;
	    continue;		/* Yes, leave it for the next pass */
	}

    /*
     * If the nearest neighbor is less than the pass tolerance distant and
     * the current region is also a nearest neighbor of its nearest neighbor,
     * then merge the two regions into the region with the lower REGION_ID.
     * Set the merge flag on this region.  merge_regions() will unset the
     * RF_ACTIVE flag on the discarded region.
     * 
     * Nb. if a region has more than one nearest neighbor, its nnbr_id is
     * chosen at random from the candidate regions. During early passes in the
     * algorithm, we might expect multiple neighbors to be equidistant.  To
     * avoid many missed merges at this stage, we don't require the nnbr_id
     * of the nearest neighbor to point back to the original region, only
     * that the nnbr_d2 be the same within a tolerance that allows for
     * floating point roundoff error.
     */
	if (nnbr_d2 <= Spr->tp2) {
	    if (fabs(regid_to_nnbr(Spr, nnbr_id).nbr_d2 - nnbr_d2)
		<= FLT_EPSILON) {
		if (R->npix < Spr->nviable || Rnnbr->npix < Spr->nviable) {
		    if (R->npix + Rnnbr->npix <= Spr->nmax) {
			Spr->merging++;
			if (r < nnbr_id) {
			    merge_regions(Spr, r, nnbr_id);
			    rf_set(R, RF_MERGE);
			    Spr->maxpix = MAX(Spr->maxpix, R->npix);
			} else {
			    merge_regions(Spr, nnbr_id, r);
			    rf_set(Rnnbr, RF_MERGE);
			    Spr->maxpix = MAX(Spr->maxpix, Rnnbr->npix);
			}
			Spr->nreg--;
		    } else {
			Spr->npix_big++;
			Spr->maxpix = MAX(Spr->maxpix, R->npix);
		    }
		} else {
		    Spr->both_viable++;
		    Spr->maxpix = MAX(Spr->maxpix, R->npix);
		}
	    } else {
		Spr->wrong_partner++;
		Spr->maxpix = MAX(Spr->maxpix, R->npix);
	    }
	} else {
	    Spr->nnbr_d2_big++;
	    Spr->maxpix = MAX(Spr->maxpix, R->npix);
	}
    }
}

/*
 *	Perform an auxiliary pass over the region list to eliminate
 *	regions with fewer than Spr->nmin pixels.
 */

void
seg_apass(Spr)
REG_1 Seg_proc  Spr;
{
    REG_2 REGION_ID r;
    REG_3 Region    R;
    float           nnbr_d2;
    REG_5 REGION_ID nnbr_id;
    REG_6 Region    Rnnbr;

 /*
  * For every region, clear the neighbor distance squared field (nbr_d2) so
  * that it can be used to validate merges between small regions (R->npix <
  * Spr->nnormin or Spr->nabsmin) and larger regions for which they are not
  * necessarily the nearest neighbor.
  */
    for (r = 1; r <= Spr->maxreg; r++) {
	regid_to_nnbr(Spr, r).nbr_d2 = MAXFLOAT;
    }

 /*
  * For every region,
  */
    for (r = 1; r <= Spr->maxreg; r++) {
	R = &regid_to_reg(Spr, r);
	if (!rf_get(R, RF_ACTIVE))	/* Is this region inactive? */
	    continue;		/* Yes. Skip it. */
	rf_unset(&regid_to_reg(Spr, r), RF_MERGE);	/* Clear merge flag */

    /*
     * If the user has specified a band with a normality criterion, then
     * check if this region is special.
     */
	if (sf_get(Spr, SF_NORB)) {
	    if (regid_to_Ctr(Spr, r)[Spr->nbno] < Spr->nblow
		|| regid_to_Ctr(Spr, r)[Spr->nbno] > Spr->nbhigh)
		rf_set(R, RF_SPECIAL);
	    else
		rf_unset(R, RF_SPECIAL);
	}

    /*
     * Is the region smaller than the minimum for its type (normal or
     * special)?
     */
	if ((rf_get(R, RF_SPECIAL) && R->npix < Spr->nabsmin)
	    || (!rf_get(R, RF_SPECIAL) && R->npix < Spr->nnormin)) {

	/*
	 * Yes, find the region's nearest neighbor.
	 */
	    reg_nnbr(Spr, r);

	/*
	 * If the region has no nearest neighbors, continue.
	 */
	    nnbr_id = regid_to_nnbr(Spr, r).nbr_id;
	    if (nnbr_id == 0)
		continue;
	    Rnnbr = &regid_to_reg(Spr, nnbr_id);

	/*
	 * Set the nbr_d2 field for the nearest neighbor to be the smaller of
	 * its current value or its distance from the current subminimal
	 * region.  Remember that we initialize the nbr_d2 field for all
	 * regions to be MAXFLOAT.
	 */
	    regid_to_nnbr(Spr, nnbr_id).nbr_d2 =
		MIN(regid_to_nnbr(Spr, nnbr_id).nbr_d2,
		    regid_to_nnbr(Spr, r).nbr_d2);
	}
    }

    Spr->dmin2 = MAXFLOAT;
    Spr->maxpix = 0;
    Spr->absminpix = Spr->norminpix = MAXINT;
    Spr->merge_attempts = Spr->special_merge_attempts = 0;
    Spr->nnbr_gone = 0;
    Spr->merging = 0;
    Spr->wrong_partner = 0;
    Spr->npix_big = 0;
    Spr->no_nbr = 0;


 /*
  * For every region,
  */
    for (r = 1; r <= Spr->maxreg; r++) {
	R = &regid_to_reg(Spr, r);

    /*
     * Is this region inactive?  Nb, we do not have to check if the region
     * has merged since two regions always merge into the one with the lower
     * REGION_ID.  The other becomes inactive.
     */
	if (!rf_get(R, RF_ACTIVE))
	    continue;		/* Yes, skip it */

    /*
     * Follow the same procedure for the two types of regions, normal and
     * special.  Each type has its own minimal size criteria, and the minimum
     * resulting region size must be tallied separately.
     */

	if (rf_get(R, RF_SPECIAL)) {

	/* CASE: Special region */

	/*
	 * If this region is not smaller than the minimum size for special
	 * regions, we don't have to worry about it.
	 */
	    if (R->npix >= Spr->nabsmin) {
		Spr->absminpix = MIN(Spr->absminpix, R->npix);
		Spr->maxpix = MAX(Spr->maxpix, R->npix);
		continue;
	    }
	    
	    nnbr_id = regid_to_nnbr(Spr, r).nbr_id;
	/*
	 * If the region has no nearest neighbors, continue.
	 */
	    if (nnbr_id == 0) {
		Spr->no_nbr++;
		continue;
	    }

	    nnbr_d2 = regid_to_nnbr(Spr, r).nbr_d2;
	    Rnnbr = &regid_to_reg(Spr, nnbr_id);
	    Spr->dmin2 = MIN(Spr->dmin2, nnbr_d2);
	    Spr->special_merge_attempts++;

	/*
	 * Is the nearest neighbor region inactive or has it already merged?
	 */
	    if (!rf_get(Rnnbr, RF_ACTIVE) || rf_get(Rnnbr, RF_MERGE)) {
		Spr->absminpix = MIN(Spr->absminpix, R->npix);
		Spr->maxpix = MAX(Spr->maxpix, R->npix);
		Spr->nnbr_gone++;
		continue;	/* Yes, leave it for the next pass */
	    }
	    if (fabs(regid_to_nnbr(Spr, nnbr_id).nbr_d2 - nnbr_d2)
		<= FLT_EPSILON) {
		if (R->npix + Rnnbr->npix <= Spr->nabsmax) {
		    Spr->merging++;
		    if (sf_get(Spr, SF_ARMM)) {
			if (R->npix > Rnnbr->npix ||
			    (R->npix == Rnnbr->npix && flip()))
			    mark_reg_in_image(Spr, Spr->aband, nnbr_id, 0);
			else
			    mark_reg_in_image(Spr, Spr->aband, r, 0);
		    }
		    if (r < nnbr_id) {
			merge_regions(Spr, r, nnbr_id);
			rf_set(R, RF_MERGE);
			Spr->absminpix = MIN(Spr->absminpix, R->npix);
			Spr->maxpix = MAX(Spr->maxpix, R->npix);
		    } else {
			merge_regions(Spr, nnbr_id, r);
			rf_set(Rnnbr, RF_MERGE);
			Spr->absminpix = MIN(Spr->absminpix, Rnnbr->npix);
			Spr->maxpix = MAX(Spr->maxpix, Rnnbr->npix);
		    }
		    Spr->nreg--;
		} else {
		    Spr->npix_big++;
		    Spr->absminpix = MIN(Spr->absminpix, R->npix);
		    Spr->maxpix = MAX(Spr->maxpix, R->npix);
		}
	    } else {
		Spr->wrong_partner++;
		Spr->absminpix = MIN(Spr->absminpix, R->npix);
		Spr->maxpix = MAX(Spr->maxpix, R->npix);
	    }

	} else {

	/* CASE: Normal region */

	/*
	 * If this region is not smaller than the minimum size for normal
	 * regions, we don't have to worry about it.
	 */
	    if (R->npix >= Spr->nnormin) {
		Spr->norminpix = MIN(Spr->norminpix, R->npix);
		Spr->maxpix = MAX(Spr->maxpix, R->npix);
		continue;
	    }

	    nnbr_id = regid_to_nnbr(Spr, r).nbr_id;
	/*
	 * If the region has no nearest neighbors, continue.
	 */
	    if (nnbr_id == 0) {
		Spr->no_nbr++;
		continue;
	    }

	    nnbr_d2 = regid_to_nnbr(Spr, r).nbr_d2;
	    Rnnbr = &regid_to_reg(Spr, nnbr_id);

	    Spr->dmin2 = MIN(Spr->dmin2, nnbr_d2);
	    Spr->merge_attempts++;

	/*
	 * Is the nearest neighbor region inactive or has it already merged?
	 */
	    if (!rf_get(Rnnbr, RF_ACTIVE) || rf_get(Rnnbr, RF_MERGE)) {
		Spr->norminpix = MIN(Spr->norminpix, R->npix);
		Spr->maxpix = MAX(Spr->maxpix, R->npix);
		Spr->nnbr_gone++;
		continue;	/* Yes, leave it for the next pass */
	    }
	    if (fabs(regid_to_nnbr(Spr, nnbr_id).nbr_d2 - nnbr_d2)
		<= FLT_EPSILON) {
		if (R->npix + Rnnbr->npix <= Spr->nabsmax) {
		    Spr->merging++;
		    if (sf_get(Spr, SF_ARMM)) {
			if (R->npix > Rnnbr->npix ||
			    (R->npix == Rnnbr->npix && flip()))
			    mark_reg_in_image(Spr, Spr->aband, nnbr_id, 0);
			else
			    mark_reg_in_image(Spr, Spr->aband, r, 0);
		    }
		    if (r < nnbr_id) {
			merge_regions(Spr, r, nnbr_id);
			rf_set(R, RF_MERGE);
			Spr->norminpix = MIN(Spr->norminpix, R->npix);
			Spr->maxpix = MAX(Spr->maxpix, R->npix);
		    } else {
			merge_regions(Spr, nnbr_id, r);
			rf_set(Rnnbr, RF_MERGE);
			Spr->norminpix = MIN(Spr->norminpix, Rnnbr->npix);
			Spr->maxpix = MAX(Spr->maxpix, Rnnbr->npix);
		    }
		    Spr->nreg--;
		} else {
		    Spr->npix_big++;
		    Spr->norminpix = MIN(Spr->norminpix, R->npix);
		    Spr->maxpix = MAX(Spr->maxpix, R->npix);
		}
	    } else {
		Spr->wrong_partner++;
		Spr->norminpix = MIN(Spr->norminpix, R->npix);
		Spr->maxpix = MAX(Spr->maxpix, R->npix);
	    }
	}
    }
}

/*
 *	Calculate the nearest neighbor of region r and set the associated
 *	entry in the nearest neighbor table, Spr->nnbrlist[r].
 */

static void
reg_nnbr(Spr, rid)
REG_1 Seg_proc  Spr;
REG_2 REGION_ID rid;
{
    Region          R;
    REG_3 int       l;
    REG_4 int       s;
    REGION_ID       nbr;
    addr_t          Nset;

    Nset = Spr->nbrset;
    R = &regid_to_reg(Spr, rid);

    init_set(Nset);

 /*
  * For every pixel in the bounding box of region rid 
  */
    {
	REG_5 cd_map    cmap;
	REG_6 cdir      d;

	for (l = R->uleft.y; l <= R->lright.y; l++) {
	    for (s = R->uleft.x; s <= R->lright.x; s++) {

	    /*
	     * Is this pixel part of region rid? 
	     */
		if (regid(Spr, l, s) != rid)
		    continue;	/* No */

	    /*
	     * Is this pixel an internal pixel of region rid? 
	     */
		cmap = contig(Spr, l, s);
		if (cmap == Cinternal)

		/*
		 * Yes 
		 */
		    continue;

	    /*
	     * No.  Then it must be a boundary pixel of region rid.  For each
	     * adjacent pixel that does not belong to region rid, add the
	     * pixel's region ID to the region set.  Note we don't have to
	     * check if the surrounding pixels are in bounds because if they
	     * weren't, they would be flagged as contiguous/out of bounds in
	     * the pixel cd_map. See the comment at the head of pixel.c. 
	     */
		else {

		    for (d = 0; d < Ncdir; d++) {
			if (!has_contig(cmap, d)) {
			    nbr = regid(Spr,
					l + cdir_to_delta(d).y,
					s + cdir_to_delta(d).x);
			    if (!add_to_set(Nset, (addr_t) & nbr))
				error("more than %d neighbors of region %d",
				      MAX_NEIGHBORS,
				      rid);
			}
		    }
		}

	    }
	}
    }

 /*
  * Now go through the neighbor set, and for each region calculate the
  * distance squared between it and region rid.  The region with the minimum
  * squared distance is region rid's nearest neighbor. 
  */
    begin_set(Nset);
    {
	float           mdist2 = MAXFLOAT;
	float           ndist2;
        REGION_ID       nnbr = 0;

	while (get_from_set(Nset, (addr_t) & nbr)) {
	    ndist2 = reg_dist2(Spr->nbands,
			       regid_to_Ctr(Spr, rid),
			       regid_to_Ctr(Spr, nbr));
	    if (ndist2 > mdist2)
		continue;
	    else if (ndist2 < mdist2) {
		mdist2 = ndist2;
		nnbr = nbr;
	    } else {

	    /*
	     * This is biased, but it does give some randomness to nnbr
	     * selection. 
	     */
		if (flip()) {
		    nnbr = nbr;
		}
	    }
	}

    /*
     * Store this region's ID and the squared distance in region rid's entry
     * in the nearest neighbor list. 
     */
	regid_to_nnbr(Spr, rid).nbr_id = nnbr;
	regid_to_nnbr(Spr, rid).nbr_d2 = mdist2;
    }
}

/*
 *	Return the distance (in pixel space) squared between the centroids
 *	of two regions.
 */
static float
reg_dist2(nb, Ctr1, Ctr2)
REG_6 int       nb;		/* Number of bands */
REG_1 float    *Ctr1;
REG_2 float    *Ctr2;
{
    REG_3 int       b;
    float           dist2 = 0;
    float           diff;

    for (b = 0; b < nb; b++) {
	diff = Ctr1[b] - Ctr2[b];
	dist2 += diff * diff;
    }
    return (dist2);
}


/*
 *	Initialize the bin width for the nearest neighbor distance squared
 *	(NND2) histogram.
 */

void
init_d2hist(tg2)
float           tg2;
{
    binwidth2 = tg2 / N_DHISTBINS;
}


/*
 *	Clear the bins of the NND2 histogram.
 */

static void
clear_d2hist()
{
    REG_1 int       i;

    for (i = 0; i <= N_DHISTBINS; i++)
	dhbin[i] = 0;
}


/*
 *	Increment the appropriate bin of the NND2 histogram to register
 *	a nearest neighbor distance.
 */

static void
hit_d2hist(dist2)
float           dist2;
{
    REG_1 int       index;

    index = dist2 / binwidth2;
    if (index > N_DHISTBINS)
	index = N_DHISTBINS;
    dhbin[index]++;
}



/*
 *	Calculate the pass tolerance based on the NND2 histogram and the
 *	merge coefficient.  If the pass tolerance for this pass is equal to
 *	the general tolerance, unset the use histogram flag in the process
 *	structure since we should never get a smaller pass tolerance at
 *	this tolerance.
 */

static void
get_tp2(Spr)
Seg_proc        Spr;
{
    REG_1 int       index;
    REG_2 long      cfreq;
    REG_3 long      maxmerge;	/* maximum # of regions allowed to merge on
				 * this pass */

    maxmerge = Spr->nreg * Spr->cm;

    for (index = 0, cfreq = 0;
	 cfreq <= maxmerge && index <= N_DHISTBINS;
	 index++)
	cfreq += dhbin[index];

    if (index > N_DHISTBINS) {
	Spr->tp2 = Spr->tg2;
	sf_unset(Spr, SF_HIST);
    } else {
	Spr->tp2 = binwidth2 * index;
    }
}


/*
 *	Generate a one byte per pixel band where the pixel value is the
 *	fixed point version of the current value of the centroid at band
 *	Seg_proc->lbno (specified by the b option on the command line).
 */

void
rband_to_lband(S)
REG_1 Seg_proc  S;
{
    REG_2           l;
    REG_3           s;

    for (l = 0; l < S->nlines; l++) {
	for (s = 0; s < S->nsamps; s++) {
	    S->lband[l][s] = regid_to_Ctr(S, regid(S, l, s))[S->lbno] + 0.5;
	}
    }
}


/*
 *	Insert value val in every pixel location corresponding to region rid
 *	in the 1 byte band band.
 */

static void
mark_reg_in_image(S, band, rid, val)
REG_1 Seg_proc  S;
uchar_t       **band;
REGION_ID       rid;
uchar_t         val;
{
    REG_2           l;
    REG_3           s;
    Region          R;

    R = &regid_to_reg(S, rid);

    for (l = R->uleft.y; l <= R->lright.y; l++) {
	for (s = R->uleft.x; s <= R->lright.x; s++) {
	    if (regid(S, l, s) == rid)
		band[l][s] = val;
	}
    }
}


/*
 *	Check the region band to make sure that all REGION_IDs corrspond
 *	to active regions.
 */

#ifdef DEBUG
void
check_region_band(Spr)
REG_1 Seg_proc  Spr;
{
    REG_2           l;
    REG_3           s;
    REG_4 REGION_ID rid;
    REG_5 Region    R;
    bool_t          BadId = FALSE;

    for (l = 0; l < Spr->nlines; l++) {
	for (s = 0; s < Spr->nsamps; s++) {
	    rid = regid(Spr, l, s);
	    R = &regid_to_reg(Spr, rid);
	    if (!rf_get(R, RF_ACTIVE)) {
		printf("REGION_ID %d at (%d,%d), %d pixels, is not active\n",
		       rid,
		       s,
		       l,
		       R->npix);
		BadId = TRUE;
	    }
	}
    }
    if (BadId)
	error("Terminating with inconsistent region map");
}
#endif DEBUG

