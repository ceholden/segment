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

/* 
** NAME
**         ugets, uputs -- text-oriented I/O
**  
** 
** SYNOPSIS
**         char *ugets(fd, buf, nbytes)
**         int fd, nbytes;
**         char *buf;
** 
**         int uputs(fd, buf)
**         int fd;
**         char *buf;
**  
** 
** DESCRIPTION
**         ugets reads bytes from file descriptor {fd} into the buffer
**         pointed to by {buf}, until any of the following are true:
** 
**          - {nbytes}-1 bytes have been read and transferred to {buf};
**            or
** 
**          - a newline character ('\n') has been read and transferred
**            to {buf}; or
** 
**          - an end-of-file condition is encountered on {fd}.
** 
**         An EOS character ('\0') is placed after the last byte
**         transferred to {buf} (i.e., on return, {buf} will point to a
**         null-terminated C string).
** 
**         uputs writes the EOS-terminated string pointed to by {buf}
**         to file descriptor {fd}.  The terminating EOS is NOT
**         written.
**  
** 
** RETURN VALUE
**         success:  {buf} (ugets); number of bytes written (uputs)
** 
**         failure:  NULL (ugets); ERROR (uputs)
**  
** 
** GLOBALS ACCESSED
**         _uiocb[fd]      uio control block for file descriptor fd
**  
** 
** APPLICATION USAGE
**         Use ugets and uputs for text-oriented input and output on
**         uio files.
**  
** 
** SEE ALSO
**         IPW: uropen, uread
** 
**         UNIX: fgets, fputs
**  
** 
** NOTES
**         ugets and uputs are analogous to the UNIX stdio routines
**         fgets and fputs, respectively.
** 
**         Calls to ugets and uread, or uputs and uwrite, may be freely
**         intermixed.
*/  

int
uputs(fd, buf)
	int             fd;		/* input file descriptor	 */
	char           *buf;		/* -> input buffer		 */
{
	int             nbytes;		/* #bytes to read		 */

	assert(buf != NULL);

	nbytes = strlen(buf);
	if (nbytes > 0) {
		nbytes = uwrite(fd, buf, nbytes);
	}

	return (nbytes);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/uio/RCS/uputs.c,v 1.4 1997/02/17 04:12:09 ceretha Exp $";

#endif
