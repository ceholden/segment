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

#ifndef	_UIO_H
#define	_UIO_H

/* per-fd I/O control block */
typedef struct {
	int             fd;		/* UNIX file descriptor		 */
	char           *name;		/* file name			 */
	unsigned        flags;		/* bit flags			 */
	char           *buf;		/* -> base of I/O buffer	 */
	xsize_t         bufsiz;		/* size (# bytes) of I/O buffer	 */
	char           *bufp;		/* -> current pos. in buffer	 */
	char           *eobuf;		/* -> 1 past last pos. in buffer */
	int             nbytes;		/* # bytes in buffer		 */
} UIO_T;

#define	UIO_READ	bit(0)		/* initialized for reading	 */
#define	UIO_WRITE	bit(1)		/* initialized for writing	 */
#define	UIO_USED	bit(2)		/* I/O done since u?open	 */
#define	UIO_EOF		bit(3)		/* ? EOF from read()		 */
#define	UIO_CANSEEK	bit(4)		/* ? OK to lseek()		 */

extern UIO_T    _uiocb[];

extern int      _uioinit();
extern int      _uread();
extern int      _uwrite();

/* $Header: /local/share/pkg/ipw/src/lib/libipw/uio/RCS/_uio.h,v 1.10 90/11/11 17:18:27 frew Exp $ */
#endif
