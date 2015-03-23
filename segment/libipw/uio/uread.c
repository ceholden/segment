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
**         uread, uwrite -- byte-oriented I/O
**  
** 
** SYNOPSIS
**         int uread(fd, buf, nbytes)
**         int fd, nbytes;
**         addr_t buf;
** 
**         int uwrite(fd, buf, nbytes)
**         int fd, nbytes;
**         addr_t buf;
**  
** 
** DESCRIPTION
**         uread is a reliable interface to the UNIX read function.  It
**         reads {nbytes} bytes from file descriptor
**         {fd} into the buffer pointed to by {buf}, unless end-of-file
**         is reached, or an I/O error occurs.
** 
**         uwrite is a reliable interface to the UNIX write function.
**         It writes {nbytes} bytes from the buffer pointed to by {buf}
**         to file descriptor {fd}.  The various types of write errors
**         are detected and folded into a single error return.
** 
**         These functions provide transparent internal buffering,
**         which conceals the effects of I/O through UNIX pipes.
**  
** 
** RETURN VALUE
**         success:  number of bytes read/written
** 
**         failure:  ERROR
**  
** 
** ERRORS
**         incomplete write
** 
**                 Some number of bytes less than the number requested
**                 were written.  This is not necessarily due to a hard
**                 I/O error.
**  
** 
** GLOBALS ACCESSED
**         _uiocb[fd]      uio control block for file descriptor {fd}
**  
** 
** APPLICATION USAGE
**         uread and uwrite are intended to replace the direct use of
**         the UNIX read and write system calls in IPW application
**         code.
**  
** 
** SEE ALSO
**         IPW: uropen
** 
**         UNIX: read, write
**  
** 
** NOTES
**         The {fd} passed to uread must have originally been obtained
**         from either uropen or ustdin.
** 
**         The {fd} passed to uwrite must have originally been obtained
**         from either uwopen or ustdout.
** 
**         Calls to fpio, pixio, and uio I/O functions should not be
**         intermixed on the same file descriptor.
*/  

int
uread(fd, buf, nbytes)
	int             fd;		/* input file descriptor	 */
	addr_t          buf;		/* -> input buffer		 */
	int             nbytes;		/* #bytes to read		 */
{
	REG_1 UIO_T    *p;		/* -> uio control block		 */
	REG_2 int       ngot;		/* #bytes got this time around	 */
	REG_3 int       nwant;		/* #bytes still needed		 */

	assert(OK_FD(fd));
	p = &_uiocb[fd];

	assert(p->flags & UIO_READ);
 /*
  * loop until we've obtained # bytes requested
  */
	for (nwant = nbytes; nwant > 0; nwant -= ngot) {
		if (p->nbytes <= 0) {
			if (_uread(p) <= 0) {
				break;
			}
		}
 /*
  * copy # bytes available or # bytes requested, whichever is smaller
  */
		ngot = MIN(p->nbytes, nwant);
		bcopy(p->bufp, buf, ngot);
 /*
  * update buffer pointers and UIO byte count
  */
		buf += ngot;
		p->bufp += ngot;
		p->nbytes -= ngot;
	}
 /*
  * return actual # bytes obtained (or ERROR if most recent read failed)
  */
	if (p->nbytes < 0) {
		return (ERROR);
	}

	return (nbytes - nwant);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/uio/RCS/uread.c,v 1.5 1997/02/17 04:12:09 ceretha Exp $";

#endif
