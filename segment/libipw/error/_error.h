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

#ifndef	_ERROR_H
#define	_ERROR_H

#define	MSG_MAX		256		/* max #chars in usrerr message	 */

extern char   **_argv;
extern int      _erno;
extern int      _fderr;
extern char     _usrerr[];

extern void     _error();

/* $Header: /nfs/home/share1/ipw-1.1/ipw-1.1/src/lib/libipw/error/RCS/_error.h,v 1.10 1997/04/09 02:42:40 ceretha Exp $ */
#endif
