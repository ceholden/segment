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

/*
** NAME
**	bihcheck -- validate components of IPW basic image header
**
** SYNOPSIS
**	bool_t bihcheck(hpp)
**	BIH_T **hpp;
**
** DESCRIPTION
**	Bihcheck checks its arguments to see if it points to a valid IPW basic
**	image header.
**
** RETURN VALUE
**	TRUE if the argument points to a valid IPW BIH, else FALSE.
**
** ERRORS
**	If any component of the BIH is invalid, then an explanatory message is
**	saved via usrerr().
**
** APPLICATION USAGE
**	Bihcheck is not meant to be called by IPW applications programs.
*/

bool_t
bihcheck(hpp)
	BIH_T         **hpp;		/* -> array of -> BIH		 */
{
	int             band;		/* loop counter			 */
	BIH_T          *hp0;		/* -> band 0 BIH		 */
	int             nbands;		/* # bands			 */

 /*
  * must have at least 1 complete header
  */
	assert(hpp != NULL);
	hp0 = hpp[0];
	assert(hp0 != NULL);
	assert(hp0->img != NULL);
 /*
  * check per-image fields
  */
	nbands = hp0->img->nbands;
	assert(nbands >= 1);

 /*
  * This old byteorder checking code is disabled because
  * IPW now supports automatic byteswapping at the pixio
  * layer.  Routines which bypass pixio will want to
  * set o_byteorder = hbyteorder(i_fd) so the output byte
  * order will match the byteorder of the input image
  */
#if 0
	if (hp0->img->byteorder == NULL) {
		usrerr("\"%s\" header: missing byteorder", BIHI_HNAME);
		return (FALSE);
	}

	if (strdiff(hostorder(), hp0->img->byteorder)) {
		for (band = 0; band < nbands; ++band) {
			if (hpp[band]->nbytes > 1) {
 /*
  * byte order only matters if there are multi-byte pixels
  * 
  * instead of barfing, this code SHOULD set a flag that triggers byte swapping
  * in pvread()
  */
				usrerr("image byte order (%s) != host (%s)",
				       hp0->img->byteorder, hostorder());
				return (FALSE);
			}
		}
	}
#endif

	if (hp0->img->nlines < 1) {
		usrerr("\"%s\" header: %d: bad # lines / image",
		       BIHI_HNAME, hp0->img->nlines);
		return (FALSE);
	}

	if (hp0->img->nsamps < 1) {
		usrerr("\"%s\" header: %d: bad # samples / line",
		       BIHI_HNAME, hp0->img->nsamps);
		return (FALSE);
	}
 /*
  * check per-band fields
  */
	for (band = 0; band < nbands; ++band) {
		int             nbits;
		int             nbytes;

		assert(hpp[band]->img == hp0->img);

		nbytes = hpp[band]->nbytes;
		nbits = hpp[band]->nbits;

		if (nbytes < 1) {
			usrerr(
			    "\"%s\" header, band %d: %d: bad # bytes / band",
			       BIH_HNAME, nbytes, band);
			return (FALSE);
		}

		if (nbits < 1 || nbits > nbytes * CHAR_BIT) {
			usrerr(
			     "\"%s\" header, band %d: %d: bad # bits / band",
			       BIH_HNAME, nbits, band);
			return (FALSE);
		}
#if CC_SAMESIZE
		if (nbytes != hp0->nbytes) {
			usrerr(
			 "\"%s\" header: bands 0, %d: # bytes / band differ",
			       BIH_HNAME, band);
			return (FALSE);
		}
#endif
	}
 /*
  * all legal components
  */
	return (TRUE);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/ipw-1.1/src/lib/libipw/bih/RCS/bihcheck.c,v 1.8 1997/02/17 04:16:14 ceretha Exp $";

#endif
