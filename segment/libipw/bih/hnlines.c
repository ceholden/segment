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
**         hnlines, hnsamps, hnbands -- per-image parameters
**  
** 
** SYNOPSIS
**         int hnlines(fd)
**         int fd;
** 
**         int hnsamps(fd)
**         int fd;
** 
**         int hnbands(fd)
**         int fd;
**  
** 
** DESCRIPTION
**         These functions return the number of lines, samples per
**         line, or bands per sample in the image accessed by file
**         descriptor {fd}.
**  
** 
** RETURN VALUE
**         number of lines, samples, or bands in the image on {fd}
**  
** 
** GLOBALS ACCESSED
**         _bih[{fd}]      BIH associated with file descriptor {fd}
**  
** 
** APPLICATION USAGE
**         In an application program, an image's file descriptor
**         typically has a much broader scope than the image's BIH
**         pointer.  Therefore, these functions allow the per-image
**         parameters normally accessed via the BIH pointer to be
**         accessed via the file descriptor.
**  
** 
** SEE ALSO
**         IPW: hnbytes, imgsize, xxhread, xxhwrite
**  
** 
** NOTES
**         These functions will cause an assertion violation if they
**         are called before a BIH is read from or written to {fd}.
*/  

int
hnlines(fd)
	int             fd;		/* image file descriptor	 */
{
	assert(OK_FD(fd));
	assert(_bih[fd] != NULL);

	return (bih_nlines(_bih[fd][0]));
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/ipw-1.1/src/lib/libipw/bih/RCS/hnlines.c,v 1.4 1997/02/17 04:16:14 ceretha Exp $";

#endif
