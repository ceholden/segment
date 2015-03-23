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
**         ipow2 -- compute integral powers of 2
**  
** 
** SYNOPSIS
**         int ipow2(i)
**         int i;
**  
** 
** DESCRIPTION
**         ipow2 computes 2 to the power {i}, checking for out-of-range
**         results.
**  
** 
** RETURN VALUE
**         success:  2**{i}
** 
**         failure:  0
**  
** 
** ERRORS
**         2**{i} is not an integer
** 
**                 {i} is negative.
** 
**         2**{i} won't fit in an "int"
** 
**                 {i} is too large.
**  
** 
** APPLICATION USAGE
**         ipow2 is typically called to determine the range of values
**         which may be assumed by an {i}-bit integer:
** 
**                 int     npixvals;
**                 BIH_T **bihp;
**                 ...
**                 npixvals = ipow2(ltoi(bih_nbits(bihpp[0])));
*/  

int
ipow2(expo)
	int             expo;		/* exponent			 */
{
	REG_2 int       i;
	REG_1 int       result;

	if (expo < 0) {
		usrerr("2**%d is not an integer", expo);
		return (0);
	}

	if (expo > sizeof(int) * CHAR_BIT) {
		usrerr("2**%d won't fit in an \"int\"", expo);
		return (0);
	}

	result = 1;
	for (i = 0; i < expo; ++i) {
		result <<= 1;
	}

	if (result < 1) {
		usrerr("2**%d won't fit in an \"int\"", expo);
		return (0);
	}

	return (result);
}

#ifdef	TEST_MAIN
main()
{
	int             i;
	int             j;

	printf("(1) There should be no errors or warnings until (2):\n");

	i = sizeof(int) * CHAR_BIT - 2;
	j = ipow2(i);
	if (j == 0) {
		warn("ipow2(%d) failed", i);
	}

	printf("(2) There should be a non-integral warning before (3):\n");

	i = (-1);
	j = ipow2(i);
	if (j == 0) {
		warn("ipow2(%d) failed", i);
	}

	printf("(3) There should be a won't-fit warning before (4):\n");

	i = sizeof(int) * CHAR_BIT;
	j = ipow2(i);
	if (j == 0) {
		warn("ipow2(%d) failed", i);
	}

	printf("(4) done\n");
}

#endif

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/ipw-1.1/src/lib/libipw/util/RCS/ipow2.c,v 1.1 1997/02/22 21:35:20 ceretha Exp $";

#endif
