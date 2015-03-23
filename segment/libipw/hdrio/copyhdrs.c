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

#include "hdrio.h"

/* 
** NAME
**         skiphdrs, copyhdrs, fphdrs -- process groups of image headers
**  
** 
** SYNOPSIS
**         #include "gethdrs.h"
** 
**         void skiphdrs(fdi)
**         int fd;
** 
**         void copyhdrs(fdi, nbands, fdo)
**         int fdi, nbands, fdo;
** 
**         #include "fpio.h"
** 
**         void fphdrs(fdi, nbands, fdo)
**         int fdi, nbands, fdo;
**  
** 
** DESCRIPTION
**         These functions read all remaining image headers from the
**         input image accessed by file descriptor {fdi}.
** 
**         skiphdrs skips (i.e. does nothing) with the input headers.
** 
**         copyhdrs copies any input headers associated with band
**         numbers less than {nbands} to the output image accessed by
**         file descriptor {fdo}.  Headers associated with band numbers
**         greater than or equal to {nbands} are skipped.
** 
**         fphdrs behaves identically to copyhdrs, except that any
**         input LQ headers are made available to the fpio subsystem.
**  
** 
** APPLICATION USAGE
**         These functions are used by programs which do not require
**         direct access to any of the input image's optional headers.
**         The image data streams involved are left positioned at the
**         first pixel in the image.
** 
**         Use gethdrs to ingest specific optional input image
**         headers.  Use the appropriate xxhwrite routines to write
**         specific optional output image headers.
**  
** 
** SEE ALSO
**         IPW: fpvread, gethdrs, xxhread, xxhwrite
**  
** 
** NOTES
**         These functions terminate program execution if any errors
**         are encountered.
** 
**         bihread must have been called on {fdi} before calling any of
**         these functions.  bihwrite must have been called on {fdo}
**         before calling copyhdrs or fphdrs.
*/  

void
copyhdrs(fdi, nbands, fdo)
	int             fdi;		/* input image file descriptor	 */
	int             nbands;		/* # bands in output image	 */
	int             fdo;		/* output image file descriptor	 */
{
	REG_1 char     *hname;		/* name of current header	 */

	assert(nbands > 0);

	while ((hname = hrname(fdi)) != NULL && strdiff(hname, BOIMAGE)) {
		if (hrband(fdi) < nbands) {
			if (hcopy(fdi, fdo) == ERROR) {
				error("can't copy \"%s\" header", hname);
			}
		}
		else {
			if (hrskip(fdi) == ERROR) {
				error("can't skip \"%s\" header", hname);
			}
		}
	}

	if (hname == NULL) {
		error("header read error");
	}
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/hdrio/RCS/copyhdrs.c,v 1.3 1997/02/17 04:34:04 ceretha Exp $";

#endif
