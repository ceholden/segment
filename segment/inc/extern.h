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

#ifndef	EXTERN_H
#define	EXTERN_H

/*
 * UNIX extern
 *
 * We assume that any UNIX C environment supplies the following header files,
 * and that they declare (or #define) the following symbols:
 *
 * <ctype.h>
 *	isalpha  islower  isxdigit ispunct  isprint  isascii  tolower
 *	isupper  isdigit  isspace  isalnum  iscntrl  toupper  toascii
 *
 * <math.h>
 *	acos  atan2 cos   fabs  gamma j1    log   sin   tan   y1
 *	asin  atof  cosh  floor hypot jn    log10 sinh  tanh  yn
 *	atan  ceil  exp   fmod  j0    ldexp pow   sqrt  y0
 *
 * <stdio.h>
 *	BUFSIZ  NULL    ferror  fopen   getc    putchar stdout
 *	EOF     fdopen  fgets   freopen getchar stderr
 *	FILE    feof    fileno  ftell   putc    stdin
 *
 * <time.h>
 *	struct tm
 *
 * <varargs.h>
 *	va_arg   va_dcl   va_end   va_list  va_start
 *
 * <stdio.h> is automatically included by ipw.h.  <ctype.h>, <math.h>,
 * <time.h>, and <varargs.h> may be optionally included by specific IPW source
 * files.
 *
 * All other non-int-valued UNIX library functions used by IPW are declared
 * here.
 */


#include <errno.h>

/* extern int      errno; */

#if 0
extern char    *sys_errlist[];
#endif
extern int      sys_nerr;

#if 0
/* should be declared already by POSIX */
extern long     lseek();
extern int	write();
extern int	read();
#endif

extern char    *tempnam();

#include <math.h>

#include <stdlib.h>
#include <time.h>

#include "PORT/linux/string.h"

/*
 * IPW init
 */

extern void     ipwenter();
extern void     ipwexit();
extern void     opt_check(int min, int max, int nopts, ...);

/*
 * IPW getopt
 */

extern char    *ipwoptarg;
extern int      ipwopterr;
extern int      ipwoptind;
extern int      ipwoptopt;

/*
 * IPW error
 */

extern void     _bug();
extern void     error(char *format, ...);
extern void     syserr();
extern void     uferr();
extern void     usrerr(char *format, ...);
extern void     warn(char *format, ...);

/*
 * IPW uio
 */

extern bool_t   ubof();
extern long     ucopy();
extern bool_t   ueof();
extern char    *ufilename();
extern char    *ugets();
extern long     urskip();

/*
 * strvec package
 */

extern STRVEC_T *addsv();
extern STRVEC_T *delsv();
extern STRVEC_T *dupsv();
extern char    *walksv();

/*
 * IPW misc
 */

extern addr_t   allocnd(int element_size, int ndimensions, ...);
extern char    *dtoa();
extern addr_t   ecalloc();
extern float    frand();
extern void     frinit();
extern char    *ftoa();
extern addr_t   hdralloc();
extern char    *hostorder();
extern char    *hstrdup();
extern char    *itoa();
extern char    *ltoa();
extern double   ltof();
extern void     no_tty();
extern char    *rmlead();
extern void     rmtrail();
extern char    *strdup();

/* $Header: /home/user4/ceretha/ipw/share/h/RCS/extern.h,v 1.49 1996/05/11 00:25:40 ceretha Exp $ */
#endif
