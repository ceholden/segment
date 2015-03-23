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

#include "hdrio.h"

#include "_hdrio.h"

/*
** NAME
**	hrskip -- skip input header records
**
** SYNOPSIS
**	int hrskip(fdi)
**	int fdi;
**
** DESCRIPTION
**	hrskip skips header records on file descriptor fdi, stopping after a
**	preamble record has been read.
**
** RETURN VALUE
**	OK for success, ERROR for failure
**
** APPLICATION USAGE
**	Call hrskip if you want to skip the (remainder of the) current input
**	header without ingesting it or passing it to another image.
*/

int
hrskip(fdi)
	int             fdi;		/* input file descriptor	 */
{
	int             err;		/* function return code		 */

	char            buf[HREC_MAX + 1];	/* header record buffer	 */

	while ((err = _hrrec(fdi, buf, sizeof(buf))) == HGOT_DATA) {
		continue;
	}

	if (err == HGOT_PRMB) {
		return (OK);
	}

	assert(err == ERROR);
	return (ERROR);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/hdrio/RCS/hrskip.c,v 1.3 1997/02/17 04:34:48 ceretha Exp $";

#endif
