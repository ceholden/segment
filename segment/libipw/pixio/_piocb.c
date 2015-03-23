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

#include "_pixio.h"

/*
** NAME
**	_piocb -- pixel I/O control block array
**
** SYNOPSIS
**	#include "_pixio.h"
**
**	extern PIXIO_T *_piocb[];
**
** DESCRIPTION
**	_piocb is an array of pointers to pixel I/O control blocks,
**	indexed by the corresponding UNIX file descriptor.
**
** RESTRICTIONS
**	The number of pointers to pixel I/O control blocks is set at
**	compile time to {OPEN_MAX}.  This imposes a hard limit on the
**	number of pixel I/O streams a process may access
**	simultaneously.
**
** WARNINGS
**
** APPLICATION USAGE
**	_piocb is not meant to be accessed by IPW applications programs.
**
** FUTURE DIRECTIONS
**
** BUGS
*/

PIXIO_T         *_piocb[OPEN_MAX] = {NULL};

#ifndef	lint
static char     rcsid[] = "$Header: /local/share/pkg/ipw/src/lib/libipw/pixio/RCS/_piocb.c,v 1.4 90/11/11 17:17:14 frew Exp $";

#endif
