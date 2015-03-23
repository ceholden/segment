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

#ifndef	MACRO_H
#define	MACRO_H

/*
 * constants
 */

#define	BLANK		' '
#define	CREAT_MODE	0666		/* default mode for creat()	 */
#define	DEC_DIGITS	"0123456789"	/* legal decimal digits		 */
#define	EOS		'\0'
#define	ERROR		(-2)		/* integer function error return */
#define	EX_ERROR	1		/* error exit() code		 */
#define	EX_OK		0		/* normal exit() code		 */
#define	FALSE		0
#define	FD_STDERR	2		/* stderr file desriptor	 */
#define	FD_STDIN	0		/* stdin file desriptor		 */
#define	FD_STDOUT	1		/* stdout file desriptor	 */
#define FLT_TOL		(5e-6)		/* 5e-FLT_DIG */
#define	HDR_VERSIZ	64		/* #chars in img hdr version string */
#define	OK		1		/* integer function normal return */
#ifndef	PI
#define	PI		3.141592653589793	/* pi			 */
#endif
#define	PORT_NAMELEN	14		/* #chars in a "portable" filename */
#define	SYS_EOF		0		/* system call EOF return	 */
#define	SYS_ERROR	(-1)		/* system call error return	 */
#define	SYS_OK		0		/* system call normal return	 */
#define	TRUE		1
#define	WHITE_SPACE	" \t\n"

/*
 * functions
 */

#define	DTR(d)			( (d) * (PI / 180.0) )
#define	MAX(a, b)		( (a) > (b) ? (a) : (b) )
#define	MIN(a, b)		( (a) < (b) ? (a) : (b) )
#define	OK_FD(fd)		( (fd) >= 0  &&  (fd) < OPEN_MAX )
#define	ROUND(x)		( (x) < 0 ? (x) - 0.5 : (x) + 0.5 )
#define	SAFE_FREE(p)		if ((p) != NULL) free(p)
#define	SKIP_DIGITS(p)		( (p) += strspn(p, DEC_DIGITS) )
#define	SKIP_SPACE(p)		( (p) += strspn(p, WHITE_SPACE) )
#define	STRDIFF(s1, s2)		( (s1)[0] != (s2)[0] && strdiff(s1, s2) )
#define	STRDIFFN(s1, s2, n)	( (s1)[0] != (s2)[0] && strdiffn(s1, s2, n) )
#define	STREQ(s1, s2)		( (s1)[0] == (s2)[0] && streq(s1, s2) )
#define	STREQN(s1, s2, n)	( (s1)[0] == (s2)[0] && streqn(s1, s2, n) )
#if __STDC__ == 1
#define	assert(expr)		if (! (expr)) \
					bug("assertion \""#expr"\" failed")
#else
#define	assert(expr)		if (! (expr)) bug("assertion \"expr\" failed")
#endif
#if CC_NOBCOPY
#define	bcopy(from, to, n)	( (void) memcpy(to, from, n) )
#else
extern void     bcopy();
#endif
#define	bfext(i, o, w)		( ((i) >> (o)) & mask(w) )
#define	bfins(i, o, w)		( ((i) & mask(w)) << (o) )
#define	bit(i)			( 1 << (i) )
#define	bug(s)			_bug(s, __FILE__, __LINE__)
#if CC_NOBZERO
#define	bzero(p, n)		( (void) memset(p, 0, n) )
#else
extern void     bzero();
#endif
#define	kstrlen(s)		( sizeof(s) - 1 )
#define	_mask(n)		( ~(~0 << (n)) )
#define	mask(n)			( (_mask((n) - 1) << 1) | 1 )
#define	pixidx(n, s, b)		( (s) * (n) + (b) )
#if CC_INDEX
#define	strchr			index
#endif
#define	strdiff(s1, s2)		( strcmp(s1, s2) != 0 )
#define	strdiffn(s1, s2, n)	( strncmp(s1, s2, n) != 0 )
#define	streq(s1, s2)		( strcmp(s1, s2) == 0 )
#define	streqn(s1, s2, n)	( strncmp(s1, s2, n) == 0 )
#if CC_RINDEX
#define	strrchr			rindex
#endif
#if CC_UCHAR
#define	uchar(i)		(i)
#else
#define	uchar(i)		( (i) & mask(CHAR_BIT) )
#endif
#if CC_ULONG
#define	ulong(i)		(i)
#else
#define	ulong(i)		( (i) & mask(CHAR_BIT * sizeof(ulong)) )
#endif
#if CC_USHORT
#define	ushort(i)		(i)
#else
#define	ushort(i)		( (i) & mask(CHAR_BIT * sizeof(ushort_t)) )
#endif

/* $Header: /tmp7/jimbo/ipw.v1.0/h/RCS/macro.h,v 1.21 93/02/02 08:24:03 jimbo Exp $ */
#endif
