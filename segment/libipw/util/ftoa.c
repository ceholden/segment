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
**         dtoa, ftoa, itoa, ltoa -- convert numbers to strings
**  
** 
** SYNOPSIS
**         char *dtoa(s, n)
**         char *s;
**         double n;
** 
**         char *ftoa(s, n)
**         char *s;
**         float n;
** 
**         char *itoa(s, n)
**         char *s;
**         int n;
** 
**         char *ltoa(s, n)
**         char *s;
**         long n;
**  
** 
** DESCRIPTION
**         These functions write the ASCII representation of {n},
**         followed by an EOS ('\0'), into the character array pointed
**         to by {s}.
**  
** 
** RETURN VALUE
**         {s}
**  
** 
** SEE ALSO
**         UNIX: sprintf
**  
** 
** NOTES
**         The array pointed to by {s} must be large enough to hold the
**         ASCII representation of {d}, plus a trailing EOS.  The
**         consequences of overflowing {s} are unpredictable.
*/  

char           *
ftoa(s, d)
	char           *s;		/* -> output string		 */
	float           d;		/* input float-precision number */
{
	assert(s != NULL);

	(void) sprintf(s, "%.*g", FLT_DIG, d);
	return (s);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/util/RCS/ftoa.c,v 1.7 1997/02/17 04:27:12 ceretha Exp $";

#endif
