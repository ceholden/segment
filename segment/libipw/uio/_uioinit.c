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
**	_uioinit -- initialize buffered UNIX I/O
**
** SYNOPSIS
**	#include "_uio.h"
**
**	int _uioinit(fd, name, mode)
**	int fd, mode;
**	char *name;
**
** DESCRIPTION
**	_uioinit initializes buffered I/O on file descriptor fd.  mode is
**	either UIO_READ or UIO_WRITE.  name is the UNIX file name associated
**	with fd.
**
** RESTRICTIONS
**
** RETURN VALUE
**	OK for success, ERROR for failure
**
** GLOBALS ACCESSED
**	_uiocb[fd]	UIO control block for file descriptor fd
**
** ERRORS
**	"can't allocate uio buffer"
**
** WARNINGS
**
** APPLICATION USAGE
**	_uioinit is called only by uropen and uwopen.
**
** FUTURE DIRECTIONS
**	Dynamically determine optimal I/O buffer size.
**
** BUGS
*/

int
_uioinit(fd, name, mode)
	int             fd;		/* file descriptor		 */
	char           *name;		/* file name			 */
	int             mode;		/* UIO_{READ,WRITE}		 */
{
	UIO_T          *p;		/* -> UIO control block		 */

 /*
  * allow read-only or write-only, but not read-write access 
  */
	assert(mode == UIO_READ || mode == UIO_WRITE);

	assert(OK_FD(fd));
	p = &_uiocb[fd];
 /*
  * barf if already initialized 
  */
	assert(p->flags == 0);
 /*
  * set I/O buffer size
  */
	p->bufsiz = OPT_BUF;
 /*
  * allocate I/O buffer
  */
	p->buf = ecalloc((int) p->bufsiz, sizeof(char));
	if (p->buf == NULL) {
		uferr(fd);
		usrerr("can't allocate uio buffer");

		return (ERROR);
	}
 /*
  * remaining initializations 
  */
	p->eobuf = p->buf + p->bufsiz;
	p->bufp = p->buf;
	p->nbytes = 0;

	p->fd = fd;
	p->flags = mode;

	if (lseek(fd, 0L, SEEK_CUR) != (long) SYS_ERROR) {
		p->flags |= UIO_CANSEEK;
	}

	p->name = strdup(name);
	if (p->name == NULL) {
		return (ERROR);
	}

	return (OK);
}

#ifndef	lint
static char     rcsid[] = "$Header: /local/share/pkg/ipw/src/lib/libipw/uio/RCS/_uioinit.c,v 1.7 90/11/11 17:18:32 frew Exp $";

#endif
