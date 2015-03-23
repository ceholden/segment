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

#include "hdrio.h"

/*
** NAME
**	_hwrec -- write an image header record
**
** SYNOPSIS
**	int _hwrec(fd, buf)
**	int fd;
**	char *buf;
**
** DESCRIPTION
**	_hwrec writes an image header record from buf to file descriptor fd.
**
** RESTRICTIONS
**	The end of the header record must be delimited by a newline-EOS
**	sequence.
**
** RETURN VALUE
**	OK for success, ERROR for failure.
**
** GLOBALS ACCESSED
**
** ERRORS
**
** WARNINGS
**
** APPLICATION USAGE
**	_hwrec is not meant to be called by IPW applications programs.
**
** FUTURE DIRECTIONS
**	_hwrec could be implemented as a macro.
**
** BUGS
*/

int
_hwrec(fd, buf)
	int             fd;		/* input file descriptor	 */
	char         *buf;		/* -> header record		 */
{
	if (uputs(fd, buf) == ERROR) {
		return (ERROR);
	}

	return (OK);
}

#ifndef	lint
static char     rcsid[] = "$Header: /local/share/pkg/ipw/src/lib/libipw/hdrio/RCS/_hwrec.c,v 1.2 90/11/11 17:15:44 frew Exp $";

#endif
