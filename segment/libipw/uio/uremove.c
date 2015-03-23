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
**         uremove -- delete a file
**  
** 
** SYNOPSIS
**         int uremove(filename)
**         char *filename;
**  
** 
** DESCRIPTION
**         remove causes the file {filename} to be deleted.
**  
** 
** RETURN VALUE
**         success:  OK
** 
**         failure:  ERROR
**  
** 
** APPLICATION USAGE
**         uremove is typically used by application programs to delete
**         scratch files.  It should be always be called instead of the
**         UNIX system call unlink, or the ANSI library function
**         remove.
**  
** 
** SEE ALSO
**         IPW: uclose, uropen
** 
**         UNIX: remove, unlink
**  
** 
** NOTES
**         If the file {filename} is currently open (i.e., is connected
**         to a file descriptor), then the behavior of remove is
**         implementation-defined.
*/  

int
uremove(filename)
	char		*filename;	/* name of file to remove	*/
{
	if (remove(filename) == SYS_ERROR) {
		syserr();
		return (ERROR);
	}

	return (OK);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/uio/RCS/uremove.c,v 1.3 1997/02/17 04:12:09 ceretha Exp $";

#endif
