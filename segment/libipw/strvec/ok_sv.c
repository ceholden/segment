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
**	ok_sv -- validate a strvec
**
** SYNOPSIS
**	#include "file.h"
**
**	type name(arg)
**	type arg;
**
** DESCRIPTION
**	ok_sv checks whether p points to a valid strvec.
**
** RESTRICTIONS
**
** RETURN VALUE
**	true if p points to a valid strvec, else false.
**
** GLOBALS ACCESSED
**
** ERRORS
**
** WARNINGS
**
** APPLICATION USAGE
**	ok_sv is not meant to be called by IPW application programs.
**
** FUTURE DIRECTIONS
**
** BUGS
*/

bool_t
ok_sv(p)
	REG_1 STRVEC_T *p;
{
	return (p != NULL &&
		p->n > 0 &&
		p->curr >= 0 && p->curr < p->n &&
		p->v != NULL);
}

#ifndef	lint
static char     rcsid[] = "$Header: /local/share/pkg/ipw/src/lib/libipw/strvec/RCS/ok_sv.c,v 1.2 90/11/11 17:18:01 frew Exp $";

#endif
