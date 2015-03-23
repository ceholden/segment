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

#include "_error.h"

/*
** NAME
**	_fderr -- file descriptor on which error occurred
**
** SYNOPSIS
**	#include "_error.h"
**
**	extern int _fderr;
**
** DESCRIPTION
**	_fderr is set to a UNIX file descriptor by calling uferr().  The
**	error handler _error() uses _fderr to determine which (if any) UNIX
**	filename to print.
**
** APPLICATION USAGE
**	_fderr is not meant to be referenced by applications programs.
*/

int             _fderr = ERROR;

#ifndef	lint
static char     rcsid[] = "$Header: /local/share/pkg/ipw/src/lib/libipw/error/RCS/_fderr.c,v 1.4 90/11/11 17:14:26 frew Exp $";

#endif
