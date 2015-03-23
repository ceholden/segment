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

extern long     imgsize();

/* 
** NAME
**         imgcopy -- copy all pixel data between image files
**  
** 
** SYNOPSIS
**         int imgcopy(i_fd, o_fd)
**         int i_fd, o_fd;
**  
** 
** DESCRIPTION
**         imgcopy copies all of the pixel data from the image accessed
**         by file descriptor {i_fd} to the image accessed by file
**         descriptor {o_fd}.
**  
** 
** RETURN VALUE
**         success:  OK
** 
**         failure:  ERROR
**  
** 
** ERRORS
**         can't calculate input image size
** 
**                 The total number of bytes in the input image won't
**                 fit in a signed long integer.
** 
**         input image larger than header indicates
** 
**                 An end-of-file was NOT detected on {i_fd} after the
**                 appropriate number of bytes were copied.
**  
** 
** APPLICATION USAGE
**         imgcopy is typically called by programs that add, delete, or
**         modify an image's headers, but just pass image data through
**         unaltered.
**  
** 
** SEE ALSO
**         IPW: imgsize, ucopy
**  
** 
** NOTES
**         imgcopy will cause an assertion violation if it is called
**         before a BIH is read from {i_fd} or written to {o_fd}.
*/  

int
imgcopy(i_fd, o_fd)
	int             i_fd;		/* input image file descriptor	 */
	int             o_fd;		/* output image file descriptor	 */
{
	long            ncopy;		/* # bytes to copy		 */

 /*
  * ncopy <- # bytes of output image data
  */
	ncopy = imgsize(o_fd);
	if (ncopy == (long) ERROR) {
		uferr(i_fd);
		usrerr("can't calculate input image size");
		return (ERROR);
	}
 /*
  * copy ncopy bytes from i_fd to o_fd
  */
	if (ucopy(i_fd, o_fd, ncopy) != ncopy) {
		return (ERROR);
	}
 /*
  * we should be at EOF on i_fd
  */
	if (!ueof(i_fd)) {
		uferr(i_fd);
		usrerr("input image larger than header indicates");
		return (ERROR);
	}

	return (OK);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/util/RCS/imgcopy.c,v 1.4 1997/02/17 04:27:12 ceretha Exp $";

#endif
