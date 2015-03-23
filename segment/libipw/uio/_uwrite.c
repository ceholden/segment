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
**	_write -- flush UIO input buffer
**
** SYNOPSIS
**	#include "_uio.h"
**
**	int _uwrite(p)
**	UIO_T *p;
**
** DESCRIPTION
**	_uwrite writes the UIO buffer accessed via p to the associated file
**	descriptor.
**
** RESTRICTIONS
**
** RETURN VALUE
**	OK for success, ERROR for failure.
**
** GLOBALS ACCESSED
**
** ERRORS
**	"incomplete write"
**		some number of bytes less than the number requested were
**		written.  This is not necessarily due to a hard I/O error.
**
** WARNINGS
**
** APPLICATION USAGE
**	_uwrite should only be called by other UIO routines.
**
** FUTURE DIRECTIONS
**
** BUGS
*/

int
_uwrite(p)
	UIO_T		*p;		/* -> UIO control block		*/
{
	int		err;		/* write() return value		*/

	err = write(p->fd, p->buf, (xsize_t) p->nbytes);
	if (err != p->nbytes) {
		if (err == SYS_ERROR) {
			syserr();
		}
		else {
			usrerr("incomplete write");
		}

		uferr(p->fd);
		return (ERROR);
	}

	p->flags |= UIO_USED;
 /*
  * reset buffer state to "empty"
  */
	p->bufp = p->buf;
	p->nbytes = 0;

	return (OK);
}

#ifndef	lint
static char     rcsid[] = "$Header: /local/share/pkg/ipw/src/lib/libipw/uio/RCS/_uwrite.c,v 1.5 90/11/11 17:18:37 frew Exp $";

#endif
