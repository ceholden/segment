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
**         ecalloc -- memory allocator
**  
** 
** SYNOPSIS
**         addr_t ecalloc(nelem, elsize)
**         int nelem, elsize;
**  
** 
** DESCRIPTION
**         ecalloc is the IPW interface to the standard C library
**         function calloc.
** 
**         ecalloc returns a pointer to enough contiguous memory to
**         hold {nelem} {elsize}-byte objects.  The memory is
**         initialized to zeros.
**  
** 
** RETURN VALUE
**         success:  pointer to allocated memory.
** 
**         failure:  NULL
**  
** 
** APPLICATION USAGE
**         IPW programs should call ecalloc wherever they would
**         otherwise call malloc or calloc.
**  
** 
** SEE ALSO
**         IPW: allocnd, usrerr
** 
**         UNIX: malloc
**  
** 
** NOTES
**         ecalloc is a "wrapper" around the calloc function that:
** 
**          - verifies that {nelem} and {elsize} are both > 0.
** 
**          - makes system error information available to the IPW error
**            function if the allocation fails (the programmer must
**            still call error explicitly).
*/  

addr_t
ecalloc(nelem, elsize)
	int		nelem;
	int		elsize;
{
	addr_t		rtn;

	assert(nelem > 0);
	assert(elsize > 0);

	rtn = calloc((unsigned) nelem, (unsigned) elsize);
	if (rtn == NULL) {
		syserr();
	}

	return (rtn);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/util/RCS/ecalloc.c,v 1.3 1997/02/17 04:27:12 ceretha Exp $";

#endif
