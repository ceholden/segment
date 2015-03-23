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
**	_erno -- most recent UNIX "errno" value
**
** SYNOPSIS
**	#include "_error.h"
**
**	extern int _erno;
**
** DESCRIPTION
**	_erno is set to the current UNIX "errno" value by calling syserr()
**	after a UNIX system call returns an error indication.  The error
**	handler _error() uses _error to determine which (if any) UNIX system
**	error message to print.
**
** APPLICATION USAGE
**	_erno is not meant to be referenced by applications programs.
*/

int             _erno = 0;

#ifndef	lint
static char     rcsid[] = "$Header: /nfs/home/share1/ipw-1.1/ipw-1.1/src/lib/libipw/error/RCS/_errno.c,v 1.5 1997/04/09 02:42:40 ceretha Exp $";

#endif
