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

#include <ctype.h>

#include "ipw.h"

/*
** NAME
**	rmlead -- skip leading white space in string
**
** SYNOPSIS
**	char *rmlead(s)
**	char *s;
**
** DESCRIPTION
**	rmlead skips leading white-space characters (space, tab,
**	carriage-return, newline, formfeed) in s.
**
** RESTRICTIONS
**	s must be EOS-terminated.
**
** RETURN VALUE
**	pointer to the first non-white-space character in s
**
** WARNINGS
**	vertical-tab characters may also be skipped, depending on the host's
**	implementation of isspace() (see ctype(3)).
*/

char           *
rmlead(s)
	REG_1 char     *s;
{
	while (isascii(*s) && isspace(*s)) {
		++s;
	}

	return (s);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/util/RCS/rmlead.c,v 1.5 1997/02/17 04:27:40 ceretha Exp $";

#endif
