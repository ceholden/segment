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

#ifndef	IPW_H
#define	IPW_H

/*
 * master IPW header file
 */

#include "config.h"

/*
 * move ansi/ and posix/ to PORT/
 * if /usr/include/float.h is missing, include local float.h in config.h
 */

#ifdef   ALT_FLOAT_H
#include ALT_FLOAT_H
#else
#include <float.h>
#endif

#ifdef   ALT_LIMITS_H
#include ALT_LIMITS_H
#else
#include <limits.h>
#endif

#include <unistd.h>
#include <stdio.h>

#include "macro.h"
#include "typedef.h"
#include "extern.h"

#include "retrofit.h"

/* $Header: /share1/ipw-1.1/share/h/RCS/ipw.h,v 1.11 1997/02/17 05:38:31 ceretha Exp $ */
#endif
