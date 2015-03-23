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
**         ipwexit -- terminate an IPW program
**  
** 
** SYNOPSIS
**         void ipwexit(status)
**         int status;
**  
** 
** DESCRIPTION
**         ipwexit terminates the execution of an IPW program.  Open
**         uio files are flushed and closed.  {status} is passed to the
**         operating system via the UNIX system call exit.
**  
** 
** APPLICATION USAGE
**         All IPW application programs should call ipwexit as their
**         last executable statement.
**  
** 
** SEE ALSO
**         IPW: ipwenter, uclose
** 
**         UNIX: exit
**  
** 
** NOTES
**         Ipwexit never returns.
*/  

void
ipwexit(status)
	int             status;		/* exit status			 */
{
	static bool_t   called = FALSE;	/* ? called already		 */

	if (!called) {
		REG_1 int       fd;	/* UNIX file descriptor		 */

 /*
  * prevent recursion (e.g. if we call error())
  */
		called = TRUE;
 /*
  * do per-file-descriptor cleanup actions
  */
		for (fd = 0; fd < OPEN_MAX; ++fd) {
 /*
  * flush pending UIO output
  */
			if (uwflush(fd) == ERROR) {
				error("can't flush UIO buffer");
			}
		}
	}
 /*
  * terminate execution
  */
	exit(status);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/main/RCS/ipwexit.c,v 1.8 1997/02/17 04:24:53 ceretha Exp $";

#endif
