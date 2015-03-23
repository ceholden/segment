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
**	_usrerr -- most recent user-supplied error message
**
** SYNOPSIS
**	#include "_error.h"
**
**	extern char _usrerr[];
**
** DESCRIPTION
**	_usrerr is set to a user-suppiled error message string by calling
**	usrerr().  The error handler _error() prints the message pointed to by
**	_usrerr.
**
** APPLICATION USAGE
**	_usrerr is not meant to be referenced by applications programs.
*/

char            _usrerr[MSG_MAX] = {EOS};

#ifndef	lint
static char     rcsid[] = "$Header: /local/share/pkg/ipw/src/lib/libipw/error/RCS/_usrerr.c,v 1.5 90/11/11 17:14:28 frew Exp $";

#endif
