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

#include "_uio.h"

/*
** NAME
**	ubof -- check for UIO beginning-of-file
**
** SYNOPSIS
**	bool_t ubof(fd)
**	int fd;
**
** DESCRIPTION
**	ubof tests whether any I/O has been performed on file descriptor fd
**	since it was last u[rw]opened.
**
** RETURN VALUE
**	TRUE if no I/O has been performed; else FALSE
**
** GLOBALS ACCESSED
**	_uiocb[fd]	UIO control block for file descriptor fd
**
** APPLICATION USAGE
**	ubof exists mainly to allow the hrname to reset itself after a file
**	descriptor has been closed and reopened.  ubof will probably not need
**	to be called by IPW application programs.
*/

bool_t
ubof(fd)
	int             fd;		/* file descriptor		 */
{
	assert(OK_FD(fd));
	return ((_uiocb[fd].flags & UIO_USED) == 0);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/uio/RCS/ubof.c,v 1.3 1997/02/17 04:12:09 ceretha Exp $";

#endif
