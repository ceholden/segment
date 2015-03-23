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

// #include <varargs.h>

#include <stdarg.h>

#include "ipw.h"

#include "getargs.h"

/*
** NAME
**	opt_check -- check for conflicting command-line options
**
** SYNOPSIS
**	#include "getargs.h"
**
**	void opt_check(n_min, n_max, n_opts, opt[, ...])
**	int n_min, n_max, n_opts;
**	OPTION_T *opt;
**
** DESCRIPTION
**	opt_check checks that at least n_min and at most n_max of the
**	following n_opts option descriptors point to options that were
**	specified on the command line.  If the check fails, an error message
**	is printed and execution terminates.
**
** ERRORS
**	must specify at least {n_min} of: {options}
**	may specify no more than {n_max} of: {options}
** 
**                 These messages are printed on the standard error
**                 output if less than {n_min} or more than {n_max} of
**                 the options were specified.
**  
** APPLICATION USAGE
**	opt_check is typically called immediately after ipwenter to ensure
**	that conflicting options were not specified.  For example:
**
**		opt_check(1, 2, 2, &opt_c, &opt_i);
**
**	will check that at least 1 of the options described by 
**	{opt_c} and {opt_i} was specified.
**
** SEE ALSO
**	IPW: ipwenter
**  
** 
** NOTES
**	opt_check causes program termination if an error occurs.
**
*/

/* VARARGS */
void
// opt_check(va_alist)
opt_check (int min, int max, int nopts, ...)
#ifdef	lint
//	int             va_alist;

#else
//                 va_dcl			/* varargs arg list		 */
#endif
{
	va_list         ap;		/* -> current arg		 */
	int             n_got;		/* # options set		 */
	int             n_max;		/* max # options that may be set */
	int             n_min;		/* min # options that may be set */
	int             n_opts;		/* # option descriptors supplied */
	char           *opts;		/* option names			 */

        n_min = min;
        n_max = max;
        n_opts = opts;

 /* NOSTRICT */
	va_start(ap, opts);

 /* NOSTRICT */
//	n_min = va_arg(ap, int);
  	assert(n_min >= 0);
 /* NOSTRICT */
//	n_max = va_arg(ap, int);
        assert(n_max >= n_min);
 /* NOSTRICT */
//	n_opts = va_arg(ap, int);
	assert(n_opts > 1);
 /*
  * allocate space for error message string.  3 chars per option, for "-x,"
  * or "-x\0"
  */
 /* NOSTRICT */
	opts = ecalloc(n_opts, 3);
	if (opts == NULL) {
		error("can't allocate opt_check error message string");
	}

	n_got = 0;

	do {
		OPTION_T       *p;	/* -> current option descriptor	 */

 /* NOSTRICT */
		p = va_arg(ap, OPTION_T *);
		assert(p != NULL);
 /*
  * count # of set options
  */
		if (p->nargs > 0) {
			++n_got;
		}
 /*
  * accumulate option letters in error message string
  */
		if (opts[0] != 0) {
			(void) strcat(opts, ",");
		}

		(void) strcat(opts, "-");
		(void) strncat(opts, &p->option, 1);
	} while (--n_opts > 0);

	va_end(ap);
 /*
  * check # options against bounds
  */
	if (n_got < n_min) {
		error("must specify at least %d of: %s", n_min, opts);
	}

	if (n_got > n_max) {
		error("may specify no more than %d of: %s", n_max, opts);
	}
 /*
  * OK: discard error message string
  */
	free(opts);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/args/RCS/opt_check.c,v 1.4 1997/02/17 04:23:06 ceretha Exp $";

#endif
