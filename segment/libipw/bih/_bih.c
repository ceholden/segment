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

/*
** NAME
**	_bih -- array of basic image headers
**
** SYNOPSIS
**	#include "_bih.h"
**
**	extern BIH_T	**_bih[];
**
** DESCRIPTION
**	_bih is an array of double-indirect pointers to basic image headers,
**	indexed by the corresponding UNIX file descriptor.
**
** RESTRICTIONS
**	The number of (BIH_T **) pointers in _bih is set at compile time to
**	{OPEN_MAX}.  This imposes a hard limit on the number of image files a
**	process may have open at once.
**
** WARNINGS
**
** APPLICATION USAGE
**	_bih should only be accessed by other UIO routines.
**
** FUTURE DIRECTIONS
**	Routines accessing _bih will replace the use of (BIH_T **) pointers
**	at the application program level.
**
** BUGS
*/

BIH_T          **_bih[OPEN_MAX] = {NULL};

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/ipw-1.1/src/lib/libipw/bih/RCS/_bih.c,v 1.2 1990/11/11 17:13:30 frew Exp $";

#endif
