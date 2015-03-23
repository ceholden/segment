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

/* 
** NAME
**         hbit -- find highest-order 1 bit in an integer
**  
** 
** SYNOPSIS
**         int hbit(arg)
**         unsigned arg;
**  
** 
** DESCRIPTION
**         hbit finds the index (1-relative) of the highest-order 1 bit
**         in {arg}.
**  
** 
** RETURN VALUE
**         the index of the highest-order 1 bit in {arg}.  If {arg} is
**         0 then 0 is returned.
**  
** 
** APPLICATION USAGE
**         The value of hbit may be interpreted as the minimum number
**         of bits needed to represent {arg}.
*/  

int
hbit(i)
	unsigned        i;
{
	int             b;		/* bit counter	 */

	for (b = 0; i != 0; i >>= 1) {
		++b;
	}

	return (b);
}

#ifdef TEST_MAIN
#include <stdio.h>

main(argc, argv)
	int             argc;
	char          **argv;
{
	unsigned        x;

	for (;;) {
		printf("Enter an unsigned integer: ");
		if (scanf("%u", &x) == EOF)
			break;
		printf("%u, nbits = %d\n", x, hbit(x));
	}
	exit(0);
}

#endif

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/util/RCS/hbit.c,v 1.3 1997/02/17 04:27:12 ceretha Exp $";

#endif
