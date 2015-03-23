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

#ifndef	BIH_H
#define	BIH_H

/*
 * basic image header
 */

typedef struct {
	char           *byteorder;	/* byte order			 */
	int             nlines;		/* # lines			 */
	int             nsamps;		/* # samples / line		 */
	int             nbands;		/* # bands			 */
} BIHI_T;

#define	BIHI_HNAME	"basic_image_i"	/* per-image component name	 */

typedef struct {
	BIHI_T         *img;		/* -> per-image component	 */
	int             nbytes;		/* # bytes / pixel		 */
	int             nbits;		/* # bits / pixel		 */
	STRVEC_T       *history;	/* -> processing history strings */
	STRVEC_T       *annot;		/* -> annotation strings	 */
} BIH_T;

#define	BIH_HNAME	"basic_image"	/* header name within IPW	 */
#define	BIH_VERSION	"$Revision: 1.10 $"	/* RCS revsion #	 */

/* field keywords */
#define	BIH_BYTEORDER	"byteorder"
#define	BIH_NLINES	"nlines"
#define	BIH_NSAMPS	"nsamps"
#define	BIH_NBANDS	"nbands"
#define	BIH_NBYTES	"bytes"
#define	BIH_NBITS	"bits"
#define	BIH_HISTORY	"history"
#define	BIH_ANNOT	"annot"

/* field access macros */
#define bih_byteorder(p)	( (p)->img->byteorder )
#define	bih_nlines(p)		( (p)->img->nlines )
#define	bih_nsamps(p)		( (p)->img->nsamps )
#define	bih_nbands(p)		( (p)->img->nbands )
#define	bih_nbytes(p)		( (p)->nbytes )
#define	bih_nbits(p)		( (p)->nbits )
#define	bih_history(p)		( (p)->history )
#define	bih_annot(p)		( (p)->annot )

extern bool_t   bihcheck();
extern BIH_T  **bihdup();
extern BIH_T   *bihmake();
extern BIH_T  **bihread();
extern int      bihwrite();
extern long     imgsize();
extern void	no_history();
extern char    *hbyteorder();
extern char    *o_byteorder;

/* $Header: /tmp7/jimbo/ipw.v1.0/h/RCS/bih.h,v 1.10 93/02/02 08:21:26 jimbo Exp $ */
#endif
