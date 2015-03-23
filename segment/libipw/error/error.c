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

// #include <varargs.h>
#include <stdarg.h>

#include "ipw.h"

#include "_error.h"

/* 
** NAME
**         error, warn -- IPW error handlers
**  
** 
** SYNOPSIS
**         void error(format, ...)
**         char *format;
** 
**         void warn(format, ...)
**         char *format;
**  
** 
** DESCRIPTION
**         These functions print the error message described by the
**         printf-style {format}, and optional additional arguments, on
**         the standard error output.  If usrerr has been called, its
**         argument is also printed.  If syserr has been called, a UNIX
**         system error message is also printed.
** 
**         error causes program termination after the error messages
**         have been printed.
**  
** 
** APPLICATION USAGE
**         error is the most common IPW error handler.  A typical usage
**         would be:
** 
**                 fd = uropen(filename);
**                 if (fd == ERROR) {
**                         error("can't open %s", filename);
**                 }
** 
**         which would produce the following standard error output:
** 
**                 {program}: ERROR:
**                         can't open "{filename}"
**                         (UNIX error is: {perror-message})
** 
**         and then cause the program to terminate.
** 
**         warn is used to notify the user of unusual conditions that
**         do not affect program execution, but may produce unexpected
**         output; e.g., an insufficient number of bits per input pixel
**         to satisfy the precision requirements of a particular
**         algorithm.
**  
** 
** SEE ALSO
**         IPW: ipwenter, usrerr
** 
**         UNIX: errno, perror
**  
** 
** NOTES
**         These functions both call the low-level function _error,
**         which does the actual error message formatting and output.
*/  

/* VARARGS */
void
// error(va_alist)
error (char *format, ...)
#ifdef	lint
//	char           *va_alist;

#else
//                va_dcl			/* varargs arg list		 */
#endif
{
	char           *fmt;		/* -> printf-style format	 */
	va_list         ap;		/* -> variable argument list	 */

        fmt = format;

 /* NOSTRICT */
	va_start(ap, format);

 /* NOSTRICT */
	// fmt = va_arg(ap, char *);
	_error("ERROR", fmt, ap);
	va_end(ap);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/error/RCS/error.c,v 1.6 1997/02/17 04:30:11 ceretha Exp $";

#endif
