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
**         uropen, ustdin, uwopen, ustdout -- access files
**  
** 
** SYNOPSIS
**         int uropen(name)
**         char *name;
** 
**         int ustdin()
** 
**         int uwopen(name)
**         char *name;
** 
**         int ustdout()
**  
** 
** DESCRIPTION
**         These functions provide low-level (uio) access to named UNIX
**         files, or to the standard input and output.  They are the
**         ONLY means by which IPW application programs may obtain file
**         descriptors.
** 
**         uropen returns a readable file descriptor connected to the
**         UNIX file {name}.  The name "-" means the standard input.
** 
**         ustdin (equivalent to uropen("-")) returns a file descriptor
**         for the standard input.
** 
**         uwopen returns a writable file descriptor connected to the
**         UNIX file {name}.  The file is created if it does not
**         already exist, or truncated if it does.
** 
**         ustdout returns a file descriptor for the standard output.
**  
** 
** RETURN VALUE
**         success:  uio file descriptor
** 
**         failure:  ERROR
**  
** 
** ERRORS
**         can't initialize standard input
** 
**                 ustdin was unable to allocate a uio buffer for the
**                 standard input.
** 
**         can't initialize standard output
** 
**                 ustdout was unable to allocate a uio buffer for the
**                 standard output.
**  
** 
** APPLICATION USAGE
**         Use uropen wherever you would normally use the UNIX system
**         call open() to open a file for reading.
** 
**         ustdin should be called by an IPW main() if the program will
**         be reading image data from the standard input.  ustdin
**         should NOT be called if the program uses UNIX stdio input
**         functions (e.g., scanf) on the standard input.
** 
**         Use uwopen wherever you would normally use the UNIX system
**         call creat() to open a file for writing.
** 
**         ustdout should be called by an IPW main() if the program
**         will be writing image data to the standard output.  ustdout
**         should NOT be called if the program uses UNIX stdio output
**         functions (e.g., printf) on the standard output.
**  
** 
** SEE ALSO
**         IPW: ugets, uclose, ucopy, ueof, uread, urskip
** 
**         UNIX: creat, open
**  
** 
** NOTES
**         ustdin and ustdout cause program termination if any errors
**         are encountered.
** 
**         A default protection mode (usually -rw-r--r--) is applied to
**         files created by uwopen.
*/  

int
ustdout()
{
	if (_uioinit(FD_STDOUT, "(stdout)", UIO_WRITE) == ERROR) {
		error("can't initialize standard output");
	}

	return (FD_STDOUT);
}

#ifndef	lint
static char	rcsid[]	= "$Header: /share1/ipw-1.1/share/src/lib/libipw/uio/RCS/ustdout.c,v 1.3 1997/02/17 04:12:09 ceretha Exp $";
#endif
