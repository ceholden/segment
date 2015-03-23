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
**	hostorder -- get host byte order
**
** SYNOPSIS
**	char *hostorder()
**
** DESCRIPTION
**	Hostorder determines the order in which the bytes of a 4-byte integer
**	are stored in memory.  The order is represented by placing the
**	decimal digits '0', '1', ... in a null-terminated string in the order
**	in which the corresponding bytes of a 4-byte integer would be placed
**	at ascending memory addresses ('0' is the least-significant byte).
**	For example, the string "0123" indicates that the bytes of a 4-byte
**	integer are stored in the order:
**
**		+--------+--------+--------+--------+
**		| byte 0 | byte 1 | byte 2 | byte 3 |
**		+--------+--------+--------+--------+
**		|<- lowest address         |<- highest address
**
** RESTRICTIONS
**	Decimal digits must be contiguously encoded in the host character
**	set.
**
** RETURN VALUE
**	A pointer to a null-terminated string containing the byte order is
**	returned.
**
** WARNINGS
**	The return value points to static data which is overwritten by each
**	call.
**
** APPLICATION USAGE
**	Hostorder is useful for placing warnings in binary data to be exported
**	to other machines, where a corresponding call to hostorder may
**	determine whether or not the data are portable.
*/

char           *
hostorder()
{
	static union {
		char            c[sizeof(int4_t) + 1];
		int4_t             i;
	}               u;

	if (u.i == 0) {
		REG_1 int4_t      byteno;

 /*
  * Load each byte of u.i with a decimal digit corresponding to its
  * significance (0 == least-significant).  The union guarantees that u.i and
  * u.c[] begin at the same address, so resulting sequence of digits in u.c[]
  * will give us the actual byte order in memory.
  * 
  * Assumptions: decimal digits are contiguous in the host character set;
  * sizeof(int4_t) <= 10
  */
		u.i = 0;
		for (byteno = 0; byteno < sizeof(int4_t); ++byteno) {
 /* NOSTRICT */
			u.i |= ((int4_t) '0' + byteno) <<
				((int4_t) CHAR_BIT * byteno);
		}

		u.c[sizeof(int4_t)] = EOS;
	}

	return (u.c);
}

#ifdef	TEST_MAIN
main()
{
	printf("%s\n", hostorder());
}

#endif

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/ipw-1.1/src/lib/libipw/bih/RCS/hostorder.c,v 1.5 1997/02/17 04:19:00 ceretha Exp $";

#endif
