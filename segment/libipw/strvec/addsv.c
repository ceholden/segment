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

/* LINTLIBRARY */

#include "ipw.h"

#include "_strvec.h"

/* 
** NAME
**         addsv, delsv, dupsv, walksv -- manipulate string vectors
**  
** 
** SYNOPSIS
**         STRVEC_T *addsv(p, s)
**         STRVEC_T *p;
**         char *s;
** 
**         STRVEC_T *delsv(p, i)
**         STRVEC_T *p;
**         int i;
** 
**         STRVEC_T *dupsv(p)
**         STRVEC_T *p;
** 
**         char *walksv(p, reset)
**         STRVEC_T *p;
**         bool_t reset;
**  
** 
** DESCRIPTION
**         These routines manipulate string vectors.  A string vector
**         is a linked list of arbitrary-length character strings.
** 
**         addsv appends a copy of the EOS-terminated string pointed to
**         by {s} to the string vector pointed to by {p}.  If {p} is
**         NULL, then a new string vector is created with {s} as its
**         first string.
** 
**         delsv deletes the {i}'th string from the string vector
**         pointed to by {p}.
** 
**         dupsv duplicates the string vector pointed to by {p}.
** 
**         walksv sequentially accesses the strings in the string
**         vector pointed to by {p}.  If {reset} is TRUE, then the
**         first string is accessed.  If {reset} is FALSE, then the
**         next string is accessed.
**  
** 
** RETURN VALUE
**         success:  pointer to the new, current, or duplicate string
**                   vector, or to the current string.
** 
**         failure:  NULL
**  
** 
** APPLICATION USAGE
**         The most frequent use of string vectors in application
**         programs is to pass an arbitrary number of strings via a
**         single argument to a library function.  For example, the
**         following string vector:
** 
**                 STRVEC_T *annot;
**                 ...
**                 annot = addsv((STRVEC_T *) NULL,
**                               "image analyzed by A. Hacker");
**                 annot = addsv(annot,
**                               "showing extent of medfly damage");
** 
**         could be passed to the bihmake function, causing both
**         strings to appear as separate annotation strings in the
**         newly created basic image header.
**  
** 
** SEE ALSO
**         IPW: bihmake
**  
** 
** NOTES
**         delsv does not return an error indication if the {i}'th
**         string doesn't exist.
** 
**         Interspersing calls to addsv or delsv with calls to walksv
**         may confuse walksv's notion of what the "next" string is.
**         Calling walksv with {reset} == TRUE always works.
*/  

STRVEC_T       *
addsv(p, s)
	REG_1 STRVEC_T *p;
	char           *s;
{
	assert(s != NULL);

 /*
  * validate existing strvec
  */
	if (p != NULL) {
		assert(ok_sv(p));
	}
 /*
  * allocate new strvec
  */
	else {
 /* NOSTRICT */
		p = (STRVEC_T *) ecalloc(1, sizeof(STRVEC_T));
		if (p == NULL) {
			return (NULL);
		}

		p->n = STRVEC_DSIZE;
		p->curr = 0;
 /* NOSTRICT */
		p->v = (char **) ecalloc(p->n, sizeof(char *));
		if (p->v == NULL) {
			return (NULL);
		}
	}
 /*
  * find first free slot in string vector
  */
	while (p->v[p->curr] != NULL) {
		++p->curr;
 /*
  * last slot MUST be NULL
  */
		if (p->curr >= p->n) {
			bug("corrupted string vector");
		}
	}
 /*
  * if only 1 slot left then grow the string vector
  */
	if (p->curr == p->n - 1) {
		p->n += STRVEC_DSIZE;
 /* NOSTRICT */
		p->v = (char **) realloc((char *) p->v,
					 (unsigned) p->n * sizeof(char *));
		if (p->v == NULL) {
			return (NULL);
		}
	}
 /*
  * link copy of string into string vector
  */
	p->v[p->curr] = strdup(s);
	if (p->v[p->curr] == NULL) {
		return (NULL);
	}
 /*
  * add trailing NULL pointer
  */
	++p->curr;
	p->v[p->curr] = NULL;

	return (p);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/strvec/RCS/addsv.c,v 1.3 1997/02/17 04:39:41 ceretha Exp $";

#endif
