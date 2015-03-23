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

#include <math.h>

#include "ipw.h"

/* 
** NAME
**         frand, frinit -- uniform random number generator
**  
** 
** SYNOPSIS
**         float frand()
** 
**         void frinit()
**  
** 
** DESCRIPTION
**         frand returns a uniform random number from the range [0,1).
** 
**         frinit initializes frand's random number generator with a
**         "seed" derived from the current system time-of-day.  If
**         frinit is not called, then successive calls to frand will
**         return the same "random" sequence for each program
**         invocation.
**  
** 
** RETURN VALUE
**         the next random number
**  
** 
** SEE ALSO
**         UNIX: random
*/  

static float    denom = (float) LONG_MAX;

void
frinit()
{
 /* NOSTRICT */
	srandom((unsigned) time((xtime_t *) NULL));
}

float
frand()
{
 /* NOSTRICT */
	return ((float) random() / denom);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/util/RCS/frand.c,v 1.7 1997/02/17 04:27:12 ceretha Exp $";

#endif
