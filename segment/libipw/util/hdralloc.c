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

/* 
** NAME
**         hdralloc -- allocate memory for image header components
**  
** 
** SYNOPSIS
**         addr_t hdralloc(nelem, elsize, fd, hname)
**         int n, size, fd;
**         char *hname;
**  
** 
** DESCRIPTION
**         hdralloc is a special interface to the ecalloc function, for
**         allocating the components of an image header data structure.
** 
**         Like ecalloc, hdralloc returns a pointer to enough
**         contiguous memory to hold {nelem} {elsize}-byte objects.
**         The memory is initialized to zeros.
** 
**         Unlike ecalloc, hdralloc terminates program execution if it
**         is unable to allocate the memory requested.  Prior to
**         termination, {hname} is passed to the usrerr function, and
**         {fd} (if it is a valid file descriptor) is passed to the
**         uferr function.  {hname} is usually the macro XXH_HNAME, as
**         defined in the header's xxh.h file, which {fd} is the file
**         descriptor of the image to which the header will be
**         written.  Pass ERROR as the value of {fd} if you want
**         hdralloc to ignore it.
** 
**         If hdralloc returns successfully, then {fd} and {hname} are
**         ignored.
**  
** 
** RETURN VALUE
**         pointer to allocated memory
**  
** 
** ERRORS
**         can't allocate "{name}" header
**         can't allocate array of "{name}" header pointers
**  
** 
** APPLICATION USAGE
**         In application programs, hdralloc is usually called to
**         allocate an array of pointers to the headers associated with
**         each image band.  See the documentation for xxhmake for an
**         example.
**  
** 
** SEE ALSO
**         IPW: ecalloc, xxhmake
*/  

addr_t
hdralloc(n, size, fd, name)
	int             n;
	int             size;
	int             fd;
	char           *name;
{
	addr_t          p;

	assert(n != 0);

	p = ecalloc(n, size);
	if (p == NULL) {
		if (fd != ERROR) {
			uferr(fd);
		}

		usrerr(n == 1 ?
		       "can't allocate \"%s\" header" :
		       "can't allocate array of \"%s\" header pointers",
		       name);
	}

	return (p);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/util/RCS/hdralloc.c,v 1.4 1997/02/17 04:27:12 ceretha Exp $";

#endif
