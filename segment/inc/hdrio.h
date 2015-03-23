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

#ifndef	HDRIO_H
#define	HDRIO_H

/*
 * header file for IPW image header I/O subsystem
 */

#define	BOIMAGE		"image"		/* name for image data preamble	 */
#define	HGOT_DATA	1		/* got a header data record	 */
#define	HGOT_PRMB	0		/* got a header preamble record	 */
/*
 * NB: setting HREC_MAX to MAX_CHAR is kind of a shot in the dark; at least it
 *     guarantees that no header record will be generated that couldn't be
 *     entered by a human at the keyboard.  The real limit is the host's
 *     maximum string length.
 */
#define	HREC_MAX	MAX_CHAR	/* max hdr rec size (incl '\n')	 */
#define	HREC_SIZ	(HREC_MAX + 1)	/* (backward compatibility)	 */
#define	NO_BAND		(-1)
#define	hwid		hwprmb		/* (backward compatibility)	 */

/*
 * '\f' at end of version string will stop "more" before image data
 */
#define	boimage(fd)	hwprmb(fd, BOIMAGE, NO_BAND, "$Revision: 1.5 $\f")

extern int      hcopy();
extern int      hgetrec();
extern int      hpass();
extern int      hputrec();
extern int      hrband();
extern char    *hrname();
extern int      hrskip();
extern char    *hrvers();
extern int      hwprmb();

/* $Header: /local/share/pkg/ipw/h/RCS/hdrio.h,v 1.5 90/11/11 16:59:39 frew Exp $ */
#endif
