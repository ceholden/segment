/*
** NAME
**	segment -- segment an IPW image by region growing
**
** SYNOPSIS
**	segment -t t1[,...,tn] -o basename [-l base,incr] [-m merge_coeff]
**		 [-n Nabsmin,Nnormin,Nviable,Nmax,Nabsmax]
**		 [-B band -N low,high] [-b band] [-M maskfile] [-A] [-h]
**		 [infile]
**
** DESCRIPTION
**	This program segments an IPW image by region growing.  Each ti
**	specifies a tolerance such that all neighbors of each region
**	in the final segmented image will be at least ti distance from
**	it in spectral space according to the segmenting metric (in this
**	version, Euclidean distance).  The basename specifies the
**	non-extension portion of the output file names.  If an input
**	filename is not specified, the program will read the input file
**	from standard input.
**
**	For each ti following the -t option, segment will output a region
**	map image showing the state of the segmentation at that tolerance.
**	Each pixel of a region map image contains the REGION_ID of the
**	region containing the pixel.  The pixel size is either 1, 2, or 4
**	bytes, and in any case is the smallest size that will hold all
**	the REGION_IDs.  The name for each region map file consists of the
**	basename, "rmap", and the final pass number for the tolerance,
**	concatenated from left to right with separating periods, e.g.,
**	sierra.rmap.112.
**
**	In addition, if the -b option is specified, the program will output
**	a single band IPW image file at the end of each pass.  The argument
**	to the -b option specifies which band in the input image will be
**	used as data for this log band.  Each pixel in the output log
**	image will hold the average value of the band specified in the
**	b option for the region of the pixel.  The name for each log file
**	consists of the basename, "log", and the current pass number,
**	concatenated from left to right with separating periods, e.g.,
**	sierra.log.11.  The previous band file is deleted when the next
**	band file is written unless it corresponds to one of the final
**	tolerances specified with the t option or unless its pass
**	triggered the condition specified by the optional -l flag.
**	The first argument to follow the l flag specifies the base
**	threshold from which to start saving bandfiles and the second
**	argument specifies the increment.  Thus,
**
**		-l 2,0.5
**
**	instructs the program to wait until a pass where no merges
**	remain between regions 2 units or less distant in spectral
**	space before saving a band file.  Files will then be saved
**	at threshold intervals of 0.5 with the next saved band file
**	corresponding to 2.5.
**
**	The -l and -t options use different methods for limiting
**	allowed merges.  As the algorithm approaches a tolerance specified
**	via the -t option, the algorithm will not allow ANY merges
**	between regions more than the current (ti) tolerance apart until
**	a pass has occurred in which there were no allowed merges.
**	Then that band file is saved and the tolerance is advanced
**	to the next tolerance specified in the -t option.  With the
**	-l option, the program looks at the minimum interregion
**	nearest neighbor distance encountered during a pass.  If
**	that distance is greater than the current -l threshold, then
**	the previous band file is saved.  Merges are allowed between
**	regions that are more than the threshold distance apart, provided
**	they are still closer than the next -t tolerance.  The -l option
**	gives a less accurate degree of merging than the -t tolerances, but
**	it is also cheaper.  When used in conjunction with the optional merge
**	coefficient, it should give comparable results.  The merge
**	coefficient restricts the degree of region merging on each pass.
**	It should be a real number Cm, such that 0 < Cm <= 1.
**	If Nr represents the number of regions present at the beginning
**	of a pass of the algorithm, no more than the Cm*Nr closest regions
**	will be merged during the pass.
**
**	The algorithm normally only allows merges with 4-way neighbors.
**	The -8 option forces the algorithm to consider 8-way neighbors.
**
**	The -n option also governs merging behaviour.  The option
**	flag should be followed by one to five positive integers.
**	The first represents the minimum number of pixels required in
**	all final regions after the last -t tolerance has been
**	reached, Nabsmin.  The default is 1.  The second specifies
**	the minimum number of pixels required in all "normal" regions
**	after the last -t tolerance has been reached, Nnormin.  The
**	meaning of "normal" will be explained below.  The default is
**	the value of Nabsmin.  The third number accompanying -n
**	represents Nviable.  Two regions are not allowed to merge
**	if they both contain Nviable or more pixels.  The default
**	for Nviable is 65535 or the maximum allowed value for an unsigned
**	short integer.  The fourth integer following -n specifies
**	Nmax, the maximum number of pixels allowed in all regions
**	during the normal passes up to the last -t tolerance.  The fifth
**	integer, Nabsmax, represents the absolute maximum number of pixels
**	allowed in any region during any pass.  The default for both Nmax
**	and Nabsmax is once again 65535.  If fewer than five integers
**	accompany -n, they are assigned to the just described program
**	parameters left to right.  That is,
**
**		-n 10,16,100
**
**	would be interpreted as
**
**		Nabsmin = 10
**		Nnormin = 16
**		Nviable = 100
**		Nmax    = 65535
**		Nabsmax = 65535.
**
**	If a 0 value is supplied for any of the parameters the default
**	value is used.  The following relation must hold between the
**	parameters values or an error message will be issued:
**
**	0 < Nabsmin <= Nnormin <= Nviable <= Nmax <= Nabsmax <= 65535
**
**	If Nnormin is > 1, then auxiliary passes will be required after
**	the last -t tolerance has been reached.  In these passes all
**	regions with fewer than Nnormin pixels are forced to merge
**	with their nearest neighbors but with no maximum allowable
**	distance requirements.  As with the regular passes, no region
**	is allowed more than one merge per pass, and on a given pass
**	each region with fewer than Nnormin pixels is allowed to merge only
**	with its nearest neighbor.  If that neighbor has already
**	merged, then the first region must await its oportunity
**	on the next pass.  During these auxiliary passes the maximum size
**	restriction for regions is relaxed, and regions may be merged to
**	produce a new region with more than Nmax pixels but no more than
**	Nabsmax pixels.  These passes produce log and region map files
**	analogous to the regular passes, but with the suffixes alog and
**	armap rather than log and rmap.  If the -A option has been specified
**	a mask image will be output with each auxiliary region map.  In this
**	mask image pixels will be marked with a one if they belong to regions
**	that have not merged during an auxiliary pass, or to regions
**	that belonged to the larger of two merging regions during
**	an auxiliary pass.  These mask images are named just like
**	the associated region map image, but use the suffix armask
**	instead of armap.
**
**	The user has the option of specifying a smaller minimum size,
**	Nabsmin, for regions that meet do not fall within a pixel value
**	interval for a user specified band.  That is, if the user
**	specifies a band through the -B option and an interval via
**	two, possibly floating point numbers, following the -N (for
**	"normal") option, then the program considers all regions
**	whose average value for the specified band is within the
**	the -N interval to be normal.  Any regions that do not qualify as
**	normal need only have Nabsmin pixels at the end of auxiliary
**	merging.  If the -B and -N options are not supplied, then all
**	regions are considered to be "normal".
**
**	The -M option allows the user to specify a single band IPW image
**	with exactly the same dimensions as the input image.  This band is
**	used to mask the input image during segmentation.  That is, no pixel
**	whose value is 0 in the mask band is allowed to merge with any
**	region, and all such pixels are recorded in the region map image
**	as belonging to an artificial region with REGION_ID 0.
**
**      The -h option asks segment to output a contiguity band file
**      (suffixes .[a]cband.) and region table (.[a]rlist) for the hsegment
**      program whenever it outputs an rmap or armap file.
**
** OPTIONS
**	-t	final segmentation tolerances
**	-o	basename for output files
**	-l	base threshold, threshold increment
**	-m	merge coefficient
**	-n	Nabsmin, Nnormin, Nviable, Nmax, Nabsmax
**	-B	band for normality criterion
**	-N	nromality crierion expressed as a pixel value interval
**	-b	band for output log files
**	-8	consider 8-way neighbors
**	-A	produce a region map mask image to mark auxiliary merges
**	-M	mask image
**      -h      output contiguity band and region table for hsegment
**
** EXAMPLES
**
** FILES
**
** DIAGNOSTICS
**
** RESTRICTIONS
**	The input and mask images should contain only 8-bit bands.
**
** FUTURE DIRECTIONS
**
** BUGS
**
** AUTHOR
**	Jud Harward
**	Department of Geography and
**	  Center for Remote Sensing
**	Boston University
*/


#ifndef lint
static char     SCCS_ID[] = "main.c 2.11  6/22/92";

#endif

#include "segment.h"

#include "getargs.h"

main(argc, argv)
int             argc;
char          **argv;
{
    static OPTION_T opt_t = {
	't',
	"segmentation tolerances",
	REAL_OPTARGS,
	"toler",
	REQUIRED,
    };

    static OPTION_T opt_l = {
	'l',
	"threshold base and increment",
	REAL_OPTARGS,
	"real",
	OPTIONAL, 2, 2
    };

    static OPTION_T opt_o = {
	'o',
	"basename for output files",
	STR_OPTARGS,
	"basename",
	REQUIRED, 1, 1
    };

    static OPTION_T opt_b = {
	'b',
	"index of band for output",
	INT_OPTARGS,
	"band",
	OPTIONAL, 1, 1
    };

    static OPTION_T opt_m = {
	'm',
	"merge coefficient",
	REAL_OPTARGS,
	"mrg_coef",
	OPTIONAL, 1, 1
    };

    static OPTION_T opt_n = {
	'n',
	"region size restrictions",
	INT_OPTARGS,
	"int",
	OPTIONAL, 1, 5
    };

    static OPTION_T opt_B = {
	'B',
	"index of band for normality criterion",
	INT_OPTARGS,
	"band",
	OPTIONAL, 1, 1
    };

    static OPTION_T opt_N = {
	'N',
	"normality interval",
	REAL_OPTARGS,
	"real",
	OPTIONAL, 2, 2
    };

    static OPTION_T opt_8 = {
	'8',
	"8-way neighbors",
    };

    static OPTION_T opt_A = {
	'A',
	"auxiliary region map mask",
    };

    static OPTION_T opt_h = {
	'h',
	"output hsegment files",
    };

    static OPTION_T opt_M = {
	'M',
	"mask image",
	STR_OPTARGS,
	"maskfile",
	OPTIONAL, 1, 1
    };

    static OPTION_T operands = {
	OPERAND,
	"input image file",
	STR_OPERANDS,
	"image",
	OPTIONAL, 1, 1
    };

    static OPTION_T *optv[] = {
	&opt_t,
	&opt_l,
	&opt_o,
	&opt_b,
	&opt_m,
	&opt_n,
	&opt_B,
	&opt_N,
	&opt_8,
	&opt_A,
	&opt_h,
	&opt_M,
	&operands,
	0
    };


    seg_proc        sproc;	/* struct that maintains segmentation state */
    int             fdi = -1;	/* input image file descriptor	 */
    int             fdm = -1;	/* mask image file descriptor	 */
    int             i;		/* index */


 /*
  * begin
  */
    ipwenter(argc, argv, optv, "segment an image");

 /*
  * Establish default values.
  */

    sproc.i_bihpp = NULL;
    sproc.l_bihpp = NULL;
    sproc.r_bihpp[0] = NULL;
    sproc.c_bihpp[0] = NULL;

    sproc.nreg = sproc.maxreg = sproc.maxpix = 0;
    sproc.absminpix = sproc.norminpix = 1;
    sproc.dmin2 = 0.0;
    sproc.image = NULL;
    sproc.imask = NULL;
    sproc.aband = sproc.cband = sproc.lband = NULL;
    sproc.rband = NULL;
    sproc.ctrlist = NULL;
    sproc.nnbrlist = NULL;
    sproc.nbrset = NULL;
    sproc.sflags = SF_CLEAR;
    sproc.pass = sproc.apass = 0;
    sproc.merging = 0;
    sproc.nabsmin = sproc.nnormin = 1;
    sproc.nviable = sproc.nmax = sproc.nabsmax = MAX_USHORT;
    sproc.nblow = 0.0;
    sproc.nbhigh = 255.0;

 /*
  * 4-way or 8-way neighbors?
  */
    if (got_opt(opt_8)) {
	Ncdir = 8;
	cdir_cdeltas = cd8_cdelta;
	cdir_flags = cd8_flag;
	Cinternal = CINTERNAL8;
    } else {
	Ncdir = 4;
	cdir_cdeltas = cd4_cdelta;
	cdir_flags = cd4_flag;
	Cinternal = CINTERNAL4;
    }

 /*
  * Check other arguments
  */
    sproc.ntols = n_args(opt_t);
    if (sproc.ntols <= 0)
	error("at least one final tolerance (-t tol) required");
    sproc.tols = (float *) LINT_CAST(ecalloc(sproc.ntols, sizeof(float)));
    if (sproc.tols == NULL) {
	error("can't allocate space for tolerance list");
    }
    for (i = 0; i < sproc.ntols; i++) {
	if ((sproc.tols[i] = real_arg(opt_t, i)) < 0)
	    error("segmentation tolerance must be > 0");
    }

    if (got_opt(opt_l)) {
	sproc.lthr = real_arg(opt_l, 0);
	sproc.lincr = real_arg(opt_l, 1);
	if (sproc.lthr < 0.0)
	    error("base threshold cannot be < 0");
	if (sproc.lincr <= 0.0)
	    error("threshold increment must be > 0");
    } else {
	sproc.lthr = sproc.lincr = 0.0;
    }

    if (got_opt(opt_m))
	sproc.cm = real_arg(opt_m, 0);
    if (sproc.cm <= 0. || sproc.cm > 1.)
	error("merge coefficient must be > 0 and <= 1");

    if (n_args(opt_n) >= 1) {
	sproc.nabsmin = int_arg(opt_n, 0);
	if (sproc.nabsmin < 0)
	    error("Nabsmin (-n Nabsmin)\n\tmust be >= 0");
	else if (sproc.nabsmin == 0)
	    sproc.nabsmin = 1;
    }
    if (n_args(opt_n) >= 2) {
	sproc.nnormin = int_arg(opt_n, 1);
	if (sproc.nnormin == 0)
	    sproc.nnormin = sproc.nabsmin;
	else if (sproc.nnormin < sproc.nabsmin)
	    error("Nnormin (-n Nabsmin,Nnormin)\n\tmust be >= Nabsmin");
    }
    if (n_args(opt_n) >= 3) {
	sproc.nviable = int_arg(opt_n, 2);
	if (sproc.nviable == 0)
	    sproc.nviable = MAX_USHORT;
	else if (sproc.nviable > MAX_USHORT || sproc.nviable < sproc.nnormin)
	    error("Nviable (-n Nabsmin,Nnormin,Nviable)\n\tmust be Nnormin<= Nviable <= 65535");
    }
    if (n_args(opt_n) >= 4) {
	sproc.nmax = int_arg(opt_n, 3);
	if (sproc.nmax == 0)
	    sproc.nmax = MAX_USHORT;
	if (sproc.nmax < sproc.nviable || sproc.nmax > MAX_USHORT)
	    error("Nmax (-n Nabsmin,Nnormin,Nviable,Nmax)\n\tmust be Nviable <= Nmax <= 65535");
    }
    if (n_args(opt_n) == 5) {
	sproc.nabsmax = int_arg(opt_n, 4);
	if (sproc.nabsmax == 0)
	    sproc.nabsmax = MAX_USHORT;
	if (sproc.nabsmax < sproc.nmax || sproc.nabsmax > MAX_USHORT)
	    error("Nabsmax (-n Nabsmin,Nnormin,Nviable,Nmax,Nabsmax)\n\tmust be Nmax <= Nabsmax <= 65535");
    }
    if (got_opt(opt_B)) {
	sproc.nbno = int_arg(opt_B, 0);
	sf_set(&sproc, SF_NORB);
    }
    if (got_opt(opt_N)) {
	sproc.nblow = real_arg(opt_N, 0);
	sproc.nbhigh = real_arg(opt_N, 1);
	if (!sf_get(&sproc, SF_NORB))
	    error("Normality interval (-N) but no normality band (-B) specified");
	if (sproc.nblow < 0.0
	    || sproc.nblow >= sproc.nbhigh
	    || sproc.nbhigh > 255.0)
	    error("Normality interval must obey 0.0 <= low < high <= 255.0");
    } else if (sf_get(&sproc, SF_NORB))
	error("Normality band (-B) specifed but no normality interval (-N)");

    sproc.base = str_arg(opt_o, 0);

    if (got_opt(opt_b)) {
	sproc.lbno = int_arg(opt_b, 0);
	sf_set(&sproc, SF_LOGB);
    } else {
	sproc.lbno = 0;
    }

    if (got_opt(opt_A)) {
	sf_set(&sproc, SF_ARMM);
    }
    if (got_opt(opt_h)) {
	sf_set(&sproc, SF_HSEG);
    }

 /*
  * Establish initial values in the segment process structure
  */
    sproc.tg = *sproc.tols;
    sproc.tp2 = sproc.tg2 = sproc.tg * sproc.tg;
    sproc.lthr2 = sproc.lthr * sproc.lthr;
    if (sproc.lincr > 0.0)
	sf_set(&sproc, SF_THRESH);
    if (sproc.cm < 1.0) {
	sf_set(&sproc, SF_HIST);
    }

 /*
  * access input file
  */
    if (!got_opt(operands)) {
	fdi = ustdin();
    } else {
	fdi = uropen(str_arg(operands, 0));
	if (fdi == ERROR) {
	    error("can't open input image file \"%s\"", str_arg(operands, 0));
	}
    }

    no_tty(fdi);

 /*
  * Access mask file if there is one
  */
    if (got_opt(opt_M)) {
	fdm = uropen(str_arg(opt_M, 0));
	if (fdm == ERROR) {
	    error("can't open mask image file \"%s\"", str_arg(opt_M, 0));
	}
	sf_set(&sproc, SF_MASK);
    }

 /*
  * Process image headers
  */
    do_headers(&sproc, fdi, fdm);

 /*
  * Check that the log band specification is legal
  */
    if (sf_get(&sproc, SF_LOGB) &&
	(sproc.lbno < 0 || sproc.lbno >= sproc.nbands))
	error("can't log band %d", sproc.lbno);

 /*
  * Set the trigger for garbage collecting memory
  */

    sproc.reclaim_trigger = (getpagesize() * MIN_RECLAIM_PAGES) /
	(sizeof(region) + sizeof(neighbor) + sizeof(float) * sproc.nbands) + 1;


 /*
  * do it
  */

    segment(&sproc, fdi, fdm);

 /*
  * end
  */
    ipwexit(EX_OK);
    
    return(0);
}
