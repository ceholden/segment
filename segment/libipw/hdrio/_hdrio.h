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

#ifndef	_HDRIO_H
#define	_HDRIO_H

/* per-fd hdrio control block */
typedef struct {
	char           *name;		/* header name			 */
	int             band;		/* header band #		 */
	char           *version;	/* header version #		 */
} HDRIO_T;

#define	BEGIN_COMMENT	'#'		/* beginning of comment		 */
#define	KEY_VAL_SEP	'='		/* key-value separator		 */
#define	PRMB_MAG	"!<header>"	/* preamble "magic string"	 */
#define	RCS_KDELIM	'$'		/* RCS keyword delimiter	 */

#define	ISPRMB(r)	( (r)[0] == '!' && \
			  (streqn(r, PRMB_MAG, kstrlen(PRMB_MAG)) || \
			   streqn(r, "!<preamble>", kstrlen("!<preamble>"))) )

extern HDRIO_T  _hdriocb[];
extern int      _hrrec();
extern int      _hwrec();

/* $Header: /local/share/pkg/ipw/src/lib/libipw/hdrio/RCS/_hdrio.h,v 1.5 90/11/11 17:15:36 frew Exp $ */
#endif
