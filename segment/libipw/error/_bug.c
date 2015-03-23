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

// #include <varargs.h>
#include <stdarg.h>

#include "ipw.h"

#include "_error.h"

/*
** NAME
**	_bug -- print bug message
**
** SYNOPSIS
**	void _bug(msg, file, line)
**	char *msg, *file;
**	int line;
**
** DESCRIPTION
**	_bug prints the bug message "msg", along with filename "file" and line
**	number "line", on the standard error output.  Program execution is
**	then terminated.
**
**	_bug is called by the "assert" and "bug" macros, which obtain the file
**	name and line number from the predefined macros __FILE__ and __LINE__,
**	respectively.
**
** WARNINGS
**	never returns
**
** APPLICATION USAGE
**	_bug is not meant to be called by applications programs.
*/

/* VARARGS */
void
_bug(va_list ap)
#ifdef	lint
	char           *va_alist;

#else
//                va_dcl			/* varargs arg list		 */
#endif
{
//	va_list         ap;		/* -> current arg		 */

 /* NOSTRICT */
	// va_start(ap);
	_error("BUG", "%s\n\t(file \"%s\", line %d)", ap);
	// va_end(ap);
}

#ifndef	lint
static char     rcsid[] = "$Header: /local/share/pkg/ipw/src/lib/libipw/error/RCS/_bug.c,v 1.5 90/11/11 17:14:17 frew Exp $";

#endif
