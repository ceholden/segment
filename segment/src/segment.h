/* segment.h 2.9  6/22/92 */

#ifndef SEGMENT_H
#define SEGMENT_H

#include <values.h>

#include "ipw.h"
#include "bih.h"

#ifndef LINT_CAST
#ifdef  lint
#define LINT_CAST(e)	( (e) ? 0 : 0 )
#else
#define LINT_CAST(e)	( e )
#endif
#endif


typedef uchar_t *Ipixel;
typedef pixel_t REGION_ID;
typedef uchar_t cdir_f;
typedef cdir_f  cd_map;
typedef int     cdir;

typedef struct {
    short           x;
    short           y;
}               pcoord;
typedef pcoord  cdelta;
typedef pcoord  cbounds;

typedef struct {
    pcoord          uleft;
    pcoord          lright;
    ushort_t        npix;
    ushort_t        rflags;
}               region, *Region;

typedef struct {
    REGION_ID       nbr_id;
    float           nbr_d2;
}               neighbor, *Neighbor;

typedef struct {
    int             ntols;		/* # of tolerances (-t) */
    float          *tols;		/* initially tolerance list (-t),
    					 * becomes ptr to current tolerance */
    float           lthr;		/* log threshold (-l) */
    float           lthr2;		/* log threshold squared */
    float           lincr;		/* log increment (-l) */
    float           tg;			/* current (general) tolerance */
    float           tg2;		/* tg squared */
    float           tp2;		/* current pass tolerance squared */
    float           cm;			/* merge coefficient (-m) */
    char           *base;		/* basename for output */
    int             nlines;		/* # image lines */
    int             nsamps;		/* # image samples */
    int             nbands;		/* # image bands */
    int             lbno;		/* log band # */
    int             nbno;		/* "normality" band # */
    int             nabsmin;		/* -n absolute minimum # of pixels
    					 *  per region */
    int             nnormin;		/* -n minimum # of pixels for "normal"
    					 *  regions */
    int             nviable;		/* -n viable region # of pixels */
    int             nmax;		/* -n maximum # of pixels before 
    					 *  auxiliary passes */
    int             nabsmax;		/* -n absolute maximum # of pixels */
    float           nblow;		/* "normality" (-N) lower bound */
    float           nbhigh;		/* "normality" (-N) upper bound */
    BIH_T         **i_bihpp;		/* IPW image headers (input) */
    BIH_T         **l_bihpp;		/* IPW band log file headers
					 * (output) */
    BIH_T          *c_bihpp[1];		/* IPW contiguity band file headers
					 * for hsegment (output) */
    BIH_T          *r_bihpp[1];		/* IPW region map headers (output) */
    ushort_t        sflags;		/* segmentation process flags */
    int             pass;		/* normal pass # */
    int             apass;		/* auxiliary pass # */
    long            nreg;		/* current # of regions */
    long            maxreg;		/* current # of slots in region list */
    long            merge_attempts;	/* # of (normal) merges attempted on 
    					 *  current pass */
    long            special_merge_attempts; /* # (special) merges attempted
    					 *  on current pass */
    long            nnbr_gone;		/* # of uncompleted merges on current
    					 *  pass due to nearest neighbor
    					 *  previously merged */
    long            merging;		/* # of completed merges on current
    					 *  pass */
    long            wrong_partner;	/* # of uncompleted merges on current
    					 *  pass due to lack of commutative
    					 *  nearest neighbor */
    long            nnbr_d2_big;	/* # of uncompleted merges on current
    					 *  pass due to nearest neighbor
    					 *  too distant */
    long            npix_big;		/* # of uncompleted merges on current
    					 *  pass due to resulting region
    					 *  too big */
    long            both_viable;	/* # of uncompleted merges on current
    					 *  pass due to both merging regions
    					 *  already having viable # pixels  */
    long            no_nbr;             /* # of uncompleted merges on current
					 *  pass due to region having no
					 *  neighbors */
    float           dmin2;		/* square of minimum nearest neighbor
    					 *  distance on this pass */
    int             maxpix;		/* max # of pixels in resultant
    					 * region this pass */
    int             absminpix;		/* min # of pixels in special region 
    					 *  during this (auxiliary) pass */
    int             norminpix;		/* min # of pixels in normal region 
    					 *  during this (auxiliary) pass */
    uchar_t       **image;		/* ptr to dope vectored, band
    					 *  interleaved by pixel image */
    uchar_t       **imask;		/* ptr to dope vectored, 1 band
    					 *  mask for image (-M) */
    cd_map        **cband;		/* ptr to dope vectored contiguity
    					 *  band */
    REGION_ID     **rband;		/* ptr to dope vectored region
    					 *  band */
    uchar_t       **lband;		/* ptr to dope vectored log band */
    uchar_t       **aband;		/* ptr to dope vectored auxiliary
    					 *  merge mask (-A) */
    Region          rlist;		/* ptr to region list, indexed by
    					 *  region ID */
    float          *ctrlist;		/* ptr to centroid list, indexed by
    					 *  region ID and band # */
    Neighbor        nnbrlist;		/* ptr to nearest neighbor list,
    					 *  indexed by region ID */
    addr_t          nbrset;		/* ptr to neighbor set (reusable) */
    long            reclaim_trigger;	/* # of unused regions in current
    					 *  region list necessary to trigger
    					 *  garbage collection */
}               seg_proc, *Seg_proc;

/*
 *	struct to transfer region list data to hsegment
 */
typedef struct {
    int             nlines;	/* # image lines */
    int             nsamps;	/* # image samples */
    int             nbands;	/* # image bands */
    long            nreg;	/* current # of regions */
}               reglist;

 /*
  * Table of flags used to code the contiguity band 
  */

#define DF_N	1<<0
#define DF_NE	1<<1
#define DF_E	1<<2
#define DF_SE	1<<3
#define DF_S	1<<4
#define DF_SW	1<<5
#define DF_W	1<<6
#define DF_NW	1<<7

#define CINTERNAL4 ((cd_map) DF_N|DF_E|DF_S|DF_W)
#define CINTERNAL8 ((cd_map) DF_N|DF_NE|DF_E|DF_SE|DF_S|DF_SW|DF_W|DF_NW)
#define CMONO	   ((cd_map) 0)
#define CCLEAR	   CMONO
#define MAX_CDIR   8

#define RF_CLEAR		((short) 0)
#define RF_ACTIVE		((short) 01)
#define RF_MERGE		((short) 01<<1)
#define RF_SPECIAL		((short) 01<<2)

#define rf_get(R,ff)		((R)->rflags & ff)
#define rf_set(R,ff)		(R)->rflags |= (ff)
#define rf_unset(R,ff)		(R)->rflags &= ~(ff)

#define SF_CLEAR		((short) 0)
#define SF_HIST			((short) 01)
#define SF_THRESH		((short) 01<<1)
#define SF_LOGB			((short) 01<<2)
#define SF_MASK			((short) 01<<3)
#define SF_ARMM			((short) 01<<4)
#define SF_NORB			((short) 01<<5)
#define SF_HSEG			((short) 01<<6)

#define sf_get(S,ff)		((S)->sflags & ff)
#define sf_set(S,ff)		(S)->sflags |= (ff)
#define sf_unset(S,ff)		(S)->sflags &= ~(ff)

#define set_contig(map, dir)	map |= cdir_flags[dir]
#define unset_contig(map, dir)	map &= ~cdir_flags[dir]
#define set_nnbr(map, dir)	set_contig(map, dir)
#define set_pcoord_from_delta(result, org, delta) \
	result.x = org.x + delta.x; result.y = org.y + delta.y
#define set_pcoord_from_dir(result, org, dir) \
	set_pcoord_from_delta(result, org, cdir_to_delta(dir))
#define set_dir_from_cdf(dir, cdf) \
	for ( dir = 0; cdf != cdir_flags[dir]; dir++)

#define has_contig(map, dir)	(map & cdir_flags[dir])
#define has_nnbr(map, dir)	has_contig(map, dir)
#define pcoord_in_bounds(coord, bounds) \
	(coord.x>=0 && coord.x<bounds.x && coord.y>=0 && coord.y<bounds.y)
#define in_bounds(l, s, bounds) \
	(s>=0 && s<bounds.x && l>=0 && l<bounds.y)

#define cdir_to_delta(dir)	(cdir_cdeltas[dir])
#define cdir_to_f(dir)		(cdir_flags[dir])

#define regid_to_reg(S,r)	(S->rlist[r])
#define regid_to_Ctr(S,r)	(&S->ctrlist[r*(S->nbands)])
#define regid_to_nnbr(S,r)	(S->nnbrlist[r])
#define regid_to_newid(S,r)	(regid_to_nnbr(S,r).nbr_id)

#define pcoord_to_regid(S,p)	(S->rband[p.y][p.x])
#define regid(S,l,s)		(S->rband[l][s])
#define pcoord_to_reg(S,p)	(regid_to_reg(S,pcoord_to_regid(S,p)))
#define pcoord_to_cdm(S,p)	(S->cband[p.y][p.x])
#define contig(S,l,s)		(S->cband[l][s])
#define pcoord_to_pix(S,p)	(S->image[p.y]+(p.x)*(S->nbands))
#define pixvec(psegp,l,s)	((psegp)->image[l]+s*(psegp)->nbands)
#define pcoord_to_mask(S,p)	(S->imask[p.y][p.x])
#define pixmask(S,l,s)		(S->imask[l][s])

/*
 *  Note that advance_dir advances the cdir in its argument as well as
 *  evaluating to the next orientation, while dir_reverse just evaluates
 *  to the reversed orientation.
 */
#define advance_dir(dir)	(++dir == Ncdir ? (dir = 0) : dir)
#define dir_reverse(dir)	((d + Ncdir/2 ) % Ncdir)


// BITS macro appears to be missing and this is the best guess 
// as to how it should look like
#define BITS(X) (sizeof(X) * 8)

#define MAX_NEIGHBORS	5000
#define MAX_USHORT	((1<<BITS(short)) - 1)
#define MIN_RECLAIM_PAGES	8
#define SMERGE_COEFF	0.001


void            segment();
void            do_headers();
uchar_t       **read_image();
void            pixel_pass();
void            make_region_list();
void            region_from_pixel();
void            merge_regions();
void            free_image();
void            seg_pass();
void            seg_apass();
void            init_d2hist();
void            rband_to_lband();
void            check_region_band();
void            compact_region_list();

/*
 *	Globals for neighbor manipulation
 */
extern int      Ncdir;
extern cdelta  *cdir_cdeltas;
extern cdelta   cd4_cdelta[];
extern cdelta   cd8_cdelta[];
extern cdir_f  *cdir_flags;
extern cdir_f   cd4_flag[];
extern cdir_f   cd8_flag[];
extern cd_map   Cinternal;

#endif
