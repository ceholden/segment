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

#include "_getargs.h"

/*
** NAME
**	_descrip -- global pointer to program description string
**
** SYNOPSIS
**	extern char *_descrip;
**
** DESCRIPTION
**	_descrip is a pointer to string containing a brief (i.e. 1 line)
**	description of the current program.  _descrip is initialized by
**	ipwenter(), and is used by usage() to generate usage messages.
**
** APPLICATION USAGE
**	_descrip is not meant to be referenced by IPW applications programs.
*/

char           *_descrip = NULL;

#ifndef	lint
static char     rcsid[] = "$Header: /local/share/pkg/ipw/src/lib/libipw/args/RCS/_descrip.c,v 1.4 90/11/11 17:13:03 frew Exp $";

#endif
