/*
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that: (1) source distributions retain this entire copyright
 * notice and comment, and (2) distributions including binaries display
 * the following acknowledgement:  ``This product includes software
 * developed by the Computer Systems Laboratory, University of
 * California, Santa Barbara and its contributors'' in the documentation
 * or other materials provided with the distribution and in all
 * advertising materials mentioning features or use of this software.
 *
 * Neither the name of the University nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

/* LINTLIBRARY */

#include "ipw.h"

#include "bih.h"
#include "hdrio.h"

/* 
** NAME
**         bihmake -- make a basic image (BI) header
**  
** 
** SYNOPSIS
**         #include "bih.h"
** 
**         BIH_T *bihmake(nbytes, nbits, history, annot, bihp,
**                        nlines, nsamps, nbands)
** 
**         int nbytes, nbits, nlines, nsamps, nbands;
**         STRVEC_T *history, *annot;
**         BIH_T *bihp;
**  
** 
** DESCRIPTION
**         bihmake allocates a single-band basic image (BI) header.
**         The header is initialized from bihmake's arguments:
** 
**         {nbytes}        number of bytes per pixel.  If {nbytes} is
**                         zero, it will be made just large enough to
**                         contain {nbits}.
** 
**         {nbits}         number of bits per pixel.  If {nbits} is
**                         zero, it will be set to ({nbytes} *
**                         CHAR_BIT).
** 
**         (i.e., {nbytes} and {nbits} cannot BOTH be zero.)
** 
**         {history}       pointer to a vector of strings to be used as
**                         history records in this header.
** 
**         {annot}         pointer to a vector of strings to be used as
**                         annotation in this header.
** 
**         {bihp}          pointer to another BI header.  If {bihp} is
**                         non-null, then the per-image component of
**                         *{bihp} will be linked into the new header.
** 
**         {nlines}, {nsamps}, and {nbands} are ignored if {bihp} is
**         non-null.
** 
**         {nlines}        number of lines per image
** 
**         {nsamps}        number of samples per image line
** 
**         {nbands}        number of bands per image sample
**  
** 
** RETURN VALUE
**         success:  pointer to new BI header
** 
**         failure:  NULL
**  
** 
** SEE ALSO
**         IPW: addsv, mkbih, xxhmake
**  
** 
** NOTES
**         The sharing of a "per-image" component is unique to the BI
**         header -- all other IPW headers are strictly per-band.
*/  

/* VARARGS3 */
BIH_T          *
bihmake(nbytes, nbits, history, annot, oldp, nlines, nsamps, nbands)
	int             nbytes;		/* # bytes / pixel		 */
	int             nbits;		/* # bits / pixel		 */
	STRVEC_T       *history;	/* -> history strings	 	 */
	STRVEC_T       *annot;		/* -> annotation strings	 */
	BIH_T          *oldp;		/* ? get per-img stuff here	 */
	int             nbands;		/* # bands			 */
	int             nlines;		/* # lines			 */
	int             nsamps;		/* # samples / line		 */
{
	BIH_T          *bihp;		/* -> new BIH			 */

	assert(nbytes > 0 || nbits > 0);
	assert(nbytes >= 0 && nbits >= 0);
 /*
  * nbits given, derive nbytes
  */
	if (nbytes == 0) {
		nbytes = (nbits + CHAR_BIT - 1) / CHAR_BIT;
	}
 /*
  * nbytes given, derive nbits
  */
	else if (nbits == 0) {
		nbits = nbytes * CHAR_BIT;
	}
 /*
  * nbytes,nbits given
  */
	else {
		assert(nbits <= nbytes * CHAR_BIT);
	}
 /*
  * allocate new BIH
  */
 /* NOSTRICT */
	bihp = (BIH_T *) hdralloc(1, sizeof(BIH_T), ERROR, BIH_HNAME);
	if (bihp == NULL) {
		return (NULL);
	}
 /*
  * initialize per-band stuff
  */
	bihp->nbytes = nbytes;
	bihp->nbits = nbits;

	if (history != NULL) {
		bihp->history = dupsv(history);
		if (bihp->history == NULL) {
			usrerr("can't duplicate history strings");
		}
	}

	if (annot != NULL) {
		bihp->annot = dupsv(annot);
		if (bihp->annot == NULL) {
			usrerr("can't duplicate annotation strings");
		}
	}
 /*
  * if old header provided then link to its per-image stuff
  */
	if (oldp != NULL) {
		assert(oldp->img != NULL);
		bihp->img = oldp->img;
	}
 /*
  * else if no old header then initialize per-image stuff
  */
	else {
		assert(nlines > 0);
		assert(nsamps > 0);
		assert(nbands > 0);
 /* NOSTRICT */
		bihp->img = (BIHI_T *) hdralloc(1, sizeof(BIHI_T),
						ERROR, BIHI_HNAME);
		if (bihp->img == NULL) {
			return (NULL);
		}

		bihp->img->nlines = nlines;
		bihp->img->nsamps = nsamps;
		bihp->img->nbands = nbands;
 /*
  * set byteorder to the global o_byteorder
  */
		if ((bihp->img->byteorder = hstrdup(o_byteorder,
				      BIHI_HNAME, NO_BAND)) == NULL) {
			return (NULL);
		}
	}

	return (bihp);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/ipw-1.1/src/lib/libipw/bih/RCS/bihmake.c,v 1.19 1997/02/17 04:16:14 ceretha Exp $";

#endif
