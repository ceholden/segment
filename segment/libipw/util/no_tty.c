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

#include "getargs.h"

/* 
** NAME
**         no_tty -- exit if file descriptor is a terminal
**  
** 
** SYNOPSIS
**         void no_tty(fd)
**         int fd;
**  
** 
** DESCRIPTION
**         If the file descriptor {fd} is connected to a terminal
**         device, then no_tty terminates program execution, and writes
**         one of the error messages described below.  Otherwise,
**         no_tty simply returns.
**  
** 
** ERRORS
**         (All of the following assume {fd} is connected to a terminal
**         device.)
** 
**         {program's usage message}
** 
**                 {fd} is the standard input.
** 
**         can't write image data to a terminal
** 
**                 {fd} is the standard output.
** 
**         can't do image I/O on a terminal device
** 
**                 {fd} is neither the standard input nor the standard
**                 output.
**  
** 
** APPLICATION USAGE
**         IPW users often forget to redirect the standard input and/or
**         output of IPW programs, leaving these I/O channels attached
**         to their terminals.  IPW application programs should check
**         all image file descriptors with no_tty, to ensure that image
**         data are not accidentally written to a user's terminal
**         screen; or, in the case of the standard input, to remind the
**         user that the program expects image, not keyboard, input:
** 
**                 int   fd;
**                 ...
**                 fd = ustdin();
**                 no_tty();
**  
** 
** SEE ALSO
**         IPW: ipwenter, uropen
** 
**         UNIX: ttyname
*/  

void
no_tty(fd)
	int             fd;		/* file descriptor		 */
{
	if (isatty(fd)) {
		if (fd == FD_STDIN) {
			usage();
		}

		if (fd == FD_STDOUT) {
			error("can't write image data to a terminal");
		}

		error("can't do image I/O on a terminal device");
	}
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/util/RCS/no_tty.c,v 1.5 1997/02/17 04:27:40 ceretha Exp $";

#endif
