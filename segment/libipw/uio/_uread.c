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
**	_uread -- fill UIO input buffer
**
** SYNOPSIS
**	#include "_uio.h"
**
**	int _uread(p)
**	UIO_T *p;
**
** DESCRIPTION
**	_uread reads one buffer's worth of data from the associated file
**	descriptor into the UIO buffer accessed via p.
**
** RESTRICTIONS
**
** RETURN VALUE
**	whatever was returned by the UNIX read() system call
**
** GLOBALS ACCESSED
**
** ERRORS
**
** WARNINGS
**	The current contents of the input buffer are overwritten.
**
** APPLICATION USAGE
**	_uread should only be called by other UIO routines.
**
** FUTURE DIRECTIONS
**
** BUGS
*/

int
_uread(p)
        UIO_T           *p;             /* -> UIO control block         */
{
	p->bufp = p->buf;

	p->nbytes = read(p->fd, p->buf, p->bufsiz);
	if (p->nbytes == 0) {
		p->flags |= UIO_EOF;
	}
	else if (p->nbytes < 0) {
		syserr();
		uferr(p->fd);
	}

	p->flags |= UIO_USED;

	return (p->nbytes);
}

#ifndef	lint
static char     rcsid[] = "$Header: /local/share/pkg/ipw/src/lib/libipw/uio/RCS/_uread.c,v 1.5 90/11/11 17:18:35 frew Exp $";

#endif
