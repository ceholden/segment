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
**	hcopy -- copy headers
**
** SYNOPSIS
**	#include "hdrio.h"
**
**	int hcopy(fdi, fdo)
**	int fdi, fdo;
**
** DESCRIPTION
**	Hcopy copies header records from "fdi" to "fdo", stopping after a
**	preamble record has been read from "fdi".
**
** RETURN VALUE
**	OK for success, ERROR for failure
**
** GLOBALS ACCESSED
**	_hdriocb[fdi]
**	_hdriocb[fdo] (via hwprmb)
**
** APPLICATION USAGE
**	Call hcopy if you need to copy an entire header, including the
**	preamble record (you will still be able to ADD records at the END).
**	If you need to INSERT records at the BEGINNING of a header, call
**	hwprmb, then insert the records with hputrec, then call hpass.
*/

int
hcopy(fdi, fdo)
	int             fdi;		/* input file descriptor	 */
	int             fdo;		/* output file descriptor	 */
{
	assert(OK_FD(fdi));
	assert(_hdriocb[fdi].name != NULL);
 /*
  * write preamble
  */
	if (hwprmb(fdo, _hdriocb[fdi].name, _hdriocb[fdi].band,
		   _hdriocb[fdi].version)
	    != OK) {
		return (ERROR);
	}
 /*
  * copy header records
  */
	if (hpass(fdi, fdo) != OK) {
		return (ERROR);
	}

	return (OK);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/hdrio/RCS/hcopy.c,v 1.3 1997/02/17 04:34:04 ceretha Exp $";

#endif
