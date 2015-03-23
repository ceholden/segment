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

/*
 * config.h for Dec Ultrix 4.0
 */

#define	CC_UCHAR	1
#define	CC_ULONG	1
#define	CC_USHORT	1

#define	CC_NOBCOPY	1
#define	CC_NOBZERO	1

#define	CC_INDEX	0
#define	CC_RINDEX	0

#define	CC_BIGENDIAN	0
#define	CC_SAMESIZE	0

#define	OPT_BUF		4096
#define	PIXEL		unsigned
#define	FPIXEL		float
#define	FPIXEL_DIG	FLT_DIG
#define	XSIZE		unsigned
#define	XTIME		long

#define	REG_1		register
#define	REG_2		register
#define	REG_3		register
#define	REG_4		register
#define	REG_5		register
#define	REG_6		register

#define	FREG_1		register
#define	FREG_2		register
#define	FREG_3		register
#define	FREG_4		register
#define	FREG_5		register
#define	FREG_6		register

#define INT2            short
#define INT4            long            
         
#define ALT_UCHAR_T     unsigned char
#define ALT_USHORT_T    unsigned short

#define ALT_FLOAT_H     "PORT/linux/float.h"

#define ALT_ADDR_T   char *

#define OPEN_MAX	100
