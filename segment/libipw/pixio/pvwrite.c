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

#include "pixio.h"

#include "_pixio.h"

/* 
** NAME
**         pvread, pvwrite -- pixel-oriented I/O
**  
** 
** SYNOPSIS
**         #include "pixio.h"
** 
**         int pvread(fd, buf, npixv)
**         int fd, npixv;
**         pixel_t *buf;
** 
**         int pvwrite(fd, buf, npixv)
**         int fd, npixv;
**         pixel_t *buf;
**  
** 
** DESCRIPTION
**         These routines are analogous to uread and uwrite, but
**         operate on "pixel vectors" instead of bytes.  A pixel vector
**         consists of all of the pixel values (one for each band)
**         associated with a single image sample, converted to the
**         unsigned integer type pixel_t.
** 
**         pvread reads {npixv} pixel vectors from the image accessed
**         by file descriptor {fd} to the buffer pointed to by {buf}.
** 
**         pvwrite writes {npixv} pixel vectors from the buffer pointed
**         to by {buf} to the image accessed by file descriptor {fd}.
**  
** 
** RETURN VALUE
**         success:  number of pixel vectors read or written
** 
**         failure:  ERROR
**  
** 
** ERRORS
**         partial pixel vector read
** 
**                 The {npixv} passed to pvread requested more pixel
**                 vectors than were available.
**  
** 
** GLOBALS ACCESSED
**         _piocb[fd]      pixio control block for file descriptor {fd}
**  
** 
** APPLICATION USAGE
**         These functions provide a simple way for application
**         programs to access "raw" image pixel values, without having
**         to worry about the underlying pixel representations (varying
**         numbers of bytes and significant bits per pixel.)
**  
** 
** SEE ALSO
**         IPW: fpvread, uread, uropen
**  
** 
** NOTES
**         It is important to remember that the {npixv} argument to
**         these functions is the number of pixel VECTORS to be read or
**         written, and that the corresponding number of INDIVIDUAL
**         PIXELS is {npixv} * (number of bands per image sample).
** 
**         Calls to fpio, pixio, and uio I/O functions should not be
**         intermixed on the same file descriptor.
*/  

int
pvwrite(fd, buf, npixv)
	int             fd;		/* input file descriptor	 */
	pixel_t        *buf;		/* -> pixel buffer		 */
	int             npixv;		/* # pixel vectors to write	 */
{
	REG_4 pixel_t  *bufp;		/* fast -> buf			 */
	REG_3 int      *ip;		/* fast -> pix{mask,size}	 */
	REG_6 int       n;		/* loop counter			 */
	REG_5 int       nbytes;		/* # bytes / raw output pixel	 */
	int             npixels;	/* total # pixels to write	 */
	PIXIO_T        *p;		/* -> pixel I/O control block	 */
	REG_1 char     *pixp;		/* -> byte within pixel_t	 */
	REG_2 char     *rawp;		/* -> raw output byte		 */

#if ! CC_BIGENDIAN
	int             xnbytes;	/* saved nbytes (littleendians)	 */

#endif

	p = _pioinit(fd, PIXIO_WRITE, npixv);
	if (p == NULL) {
		return (ERROR);
	}

	npixels = npixv * p->nbands;
 /*
  * mask off unused bits in buf
  */
	ip = p->pixmask;
	bufp = buf;

	n = npixels;
	do {
		*bufp++ &= *ip++;
	} while (--n > 0);
 /*
  * convert pixel_t -> raw pixels
  */
	ip = p->pixsiz;
	rawp = p->rawbuf;
 /* NOSTRICT */
	pixp = (char *) buf;

	n = npixels;
	do {
		nbytes = *ip++;
#if CC_BIGENDIAN
		pixp += sizeof(pixel_t);
		pixp -= nbytes;
#else
		xnbytes = nbytes;
#endif
		do {
			*rawp++ = *pixp++;
		} while (--nbytes > 0);
#if ! CC_BIGENDIAN
		pixp += sizeof(pixel_t);
		pixp -= xnbytes;
#endif
	} while (--n > 0);
 /*
  * write raw pixels
  */
	if (uwrite(fd, p->rawbuf, p->nbytes) != p->nbytes) {
		return (ERROR);
	}
 /*
  * return # pixel vectors
  */
	return (npixv);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/pixio/RCS/pvwrite.c,v 1.8 1997/02/17 04:21:25 ceretha Exp $";

#endif
