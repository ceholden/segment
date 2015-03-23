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
#include "_bih.h"

/* 
** NAME
**         imgsize, sampsize -- size of entire image or single sample
**  
** 
** SYNOPSIS
**         #include "bih.h"
** 
**         long imgsize(fd)
**         int fd;
** 
**         int sampsize(fd)
**         int fd;
**  
** 
** DESCRIPTION
**         imgsize computes the total number of image data (pixel)
**         bytes in the image accessed by file descriptor {fd}.
** 
**         sampsize computes the total number of bytes in a single
**         sample of the image accessed by the file descriptor {fd}.
**  
** 
** RETURN VALUE
**         total number of pixel bytes in the image, or in a single
**         image sample
**  
** 
** GLOBALS ACCESSED
**         _bih[{fd}]      BIH associated with file descriptor {fd}
**  
** 
** APPLICATION USAGE
**         imgsize is useful only to programs that are copying all of
**         the pixel data from an input IPW image to an output non-IPW
**         image, in which case imgsize can provide the {ncopy}
**         argument required by the ucopy function.  If the output
**         image has an IPW BIH, then imgcopy should be used instead of
**         ucopy.
** 
**         sampsize is useful to programs that do not need to interpret
**         image pixel values, but do need access to individual samples
**         (e.g. flip, transpose, window, etc.)
**  
** 
** SEE ALSO
**         IPW: hnlines, hnbytes, imgcopy, ucopy, xxhread, xxhwrite
**  
** 
** NOTES
**         These functions will cause an assertion violation if they
**         are called before a BIH is read from or written to {fd}.
** 
**         The value returned by imgsize does NOT include the bytes
**         occupied by header data; e.g., for an image with 512 lines,
**         512 samples, 1 band, and 1 byte per pixel, the value
**         returned by imgsize will be 262,144 (512*512).
*/  

long
imgsize(fd)
	int             fd;		/* image file descriptor	 */
{
	BIH_T         **bihpp;		/* -> BIH array			 */
	long            nbytes;		/* # bytes in image		 */

	bihpp = _bih[fd];
	assert(bihpp != NULL);

	nbytes = sampsize(fd);
	nbytes *= bih_nsamps(bihpp[0]);
	nbytes *= bih_nlines(bihpp[0]);

	return (nbytes);
}

#ifdef TEST
#include "getargs.h"

main(argc, argv)
	int             argc;
	char          **argv;
{
	int             fdi;

	ipwenter(argc, argv, (OPTION_T **) NULL, "test imgsize() function");

	fdi = ustdin();
	no_tty(fdi);

	if (bihread(fdi) == NULL) {
		error("can't read BIH");
	}

	(void) printf("%ld\n", imgsize(fdi));
}

#endif

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/ipw-1.1/src/lib/libipw/bih/RCS/imgsize.c,v 1.7 1997/02/17 04:19:00 ceretha Exp $";

#endif
