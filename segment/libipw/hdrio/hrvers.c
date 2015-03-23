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

#include "_hdrio.h"

/*
** NAME
**	hrvers -- get current header version string
**
** SYNOPSIS
**	#include "hdrio.h"
**
**	char *hrvers(fd)
**	int fd;
**
** DESCRIPTION
**	Hrvers returns the version string of the image header currently being
**	read from file descriptor fd, or NULL if there is no current image
**	header.
**
** RETURN VALUE
**	Current header version string,, or NULL.
**
** GLOBALS ACCESSED
**	_hdriocb[fd]
**
** APPLICATION USAGE
**	Call hrvers after you call hrname, if you need to know the header's
**	RCS revision level.
*/

char           *
hrvers(fd)
	int             fd;		/* input file descriptor	 */
{
	assert(OK_FD(fd));
	return (_hdriocb[fd].version);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/hdrio/RCS/hrvers.c,v 1.3 1997/02/17 04:34:48 ceretha Exp $";

#endif
