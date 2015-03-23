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

#ifndef	TYPEDEF_H
#define	TYPEDEF_H

/*
 * basic types
 */

typedef int     bool_t;			/* TRUE or FALSE		 */

/*
 * configuration dependent
 */

typedef PIXEL   pixel_t;		/* integer pixel		 */
typedef FPIXEL  fpixel_t;		/* floating-point pixel		 */
typedef XSIZE   xsize_t;		/* sizeof(anything)		 */
typedef XTIME   xtime_t;		/* returned by time()		 */

typedef INT2    int2_t;                 /* 2-byte integer                */
typedef INT4    int4_t;                 /* 4-byte integer                */

typedef struct {			/* vector of strings:		 */
	int             n;		/* -- # element in v		 */
	int             curr;		/* -- v[curr] is current string	 */
	char          **v;		/* -- -> strings		 */
} STRVEC_T;

/*
 * conditional since may already be defined in some environments
 */

#ifdef ALT_ADDR_T
typedef ALT_ADDR_T addr_t;              /* (void *)                      */
#endif

#ifdef ALT_OFF_T
typedef ALT_OFF_T off_t;                /* lseek offset                  */
#endif

#ifdef ALT_UCHAR_T
typedef ALT_UCHAR_T uchar_t;            /* unsigned char                 */
#endif

#ifdef ALT_USHORT_T
typedef ALT_USHORT_T ushort_t;           /* unsigned short                 */
#endif

/* $Header: /share1/ipw-1.1/share/h/RCS/typedef.h,v 1.10 1997/02/17 05:40:19 ceretha Exp $ */
#endif
