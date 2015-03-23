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
**	_nohist -- array of basic image headers
**
** SYNOPSIS
**	#include "_nohist.h"
**
**	extern bool_t	  _no_hist[];
**
** DESCRIPTION
**	_no_hist is an array of flags to suppress printing of the
**	history records
**
** RESTRICTIONS
**	The number of (bool_t) values in _no_hist is set at compile
**	time to {OPEN_MAX}.  This imposes a hard limit on the number of
**	image files a process may have open at once.
**
** WARNINGS
**
** APPLICATION USAGE
**	_no_hist should only be accessed by other UIO routines.
**
** FUTURE DIRECTIONS
**	Routines accessing _no_hist will replace the use of direct
**	access at the application program level.
**
** BUGS
*/

bool_t          _no_hist[OPEN_MAX] = {0};

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/ipw-1.1/src/lib/libipw/bih/RCS/_nohist.c,v 1.2 1990/11/11 17:13:36 frew Exp $";

#endif
