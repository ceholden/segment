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

/*
** NAME
**	hstrdup -- duplicate image header string
**
** SYNOPSIS
**	char *hstrdup(s, name, band)
**	char *s, *name;
**	int band;
**
** DESCRIPTION
**	hstrdup duplicates the EOS-terminated string pointed to by s,
**	which must not be NULL.  If a duplicate string cannot be
**	allocated, then name and band are used to construct an error
**	message and the NULL is returned.  If band is less than 0, it
**	is ignored.
**
** RETURN VALUE
**	pointer to duplicate string; NULL if duplicate string cannot be
**	allocated
**
** ERRORS
**	Dies with bug message if string to be duplicated is NULL.
**
** APPLICATION USAGE
**	hstrdup is meant to be called by IPW header routines, to
**	standardize strdup() error handling.
*/

char           *
hstrdup(s, name, band)
	char           *s;		/* string to duplicate		 */
	char           *name;		/* header name			 */
	int             band;		/* header band #		 */
{
	char           *rtn;		/* duplicate string		 */

	if (s == NULL) {
		bug("called with NULL string to be duplicated");
	}

	rtn = strdup(s);
	if (rtn == NULL) {
		usrerr(band >= 0 ?
		"'%s' header: can't duplicate string '%s' (band %d)" :
		       "'%s' header: can't duplicate string '%s'",
		       name, s, band);
		return (NULL);
	}

	return (rtn);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/util/RCS/hstrdup.c,v 1.7 1997/02/17 04:27:12 ceretha Exp $";

#endif
