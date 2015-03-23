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
**         uclose -- close UNIX file descriptor
**  
** 
** SYNOPSIS
**         int uclose(fd)
**         int fd;
**  
** 
** DESCRIPTION
**         uclose forces any buffered uio output pending on file
**         descriptor {fd} to be written, and then closes the file.
**         All uio internal state information pertaining to {fd} is
**         reset (cleared or deallocated).
**  
** 
** RETURN VALUE
**         success:  OK
** 
**         failure:  ERROR
**  
** 
** GLOBALS ACCESSED
**         _uiocb[fd]      uio control block for file descriptor {fd}
**  
** 
** APPLICATION USAGE
**         Application programs normally do not need to explicitly call
**         uclose, since it is called automatically for all open files
**         by ipwexit.  However, it may be necessary to explicitly
**         close a file that has been opened for writing, if you
**         subsequently want to reopen it for reading, since uio does
**         not allow simultaneous read/write access.
**  
** 
** SEE ALSO
**         IPW: ipwexit, ugets, uread, uropen
** 
**         UNIX: close
**  
** 
** NOTES
**         Any buffered INPUT pending on {fd} is discarded.
** 
**         Passing a uclose'd file descriptor to any uio routine will
**         cause an assertion violation.
*/  

int
uclose(fd)
	int             fd;		/* file descriptor		 */
{
	UIO_T          *p;		/* -> UIO control block		 */

	assert(OK_FD(fd));
	p = &_uiocb[fd];
 /*
  * if already closed then do nothing
  */
	if (p->flags == 0) {
		return (OK);
	}
 /*
  * flush pending output
  *
  * NB: should we drain input too?
  */
	if (uwflush(fd) == ERROR) {
		return (ERROR);
	}
 /*
  * do a UNIX close
  */
	if (close(fd) == SYS_ERROR) {
		syserr();
		uferr(fd);
		return (ERROR);
	}
 /*
  * free dynamic arrays, then clear UIO control block
  */
	SAFE_FREE(p->name);
	SAFE_FREE(p->buf);

 /* NOSTRICT */
	bzero((char *) p, sizeof(*p));

	return (OK);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/uio/RCS/uclose.c,v 1.6 1997/02/17 04:12:09 ceretha Exp $";

#endif
