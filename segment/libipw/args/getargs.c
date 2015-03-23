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

#include <ctype.h>

#include "ipw.h"

#include "_getargs.h"

/*
** NAME
**	getargs -- get command-line options and arguments
**
** SYNOPSIS
**	#include "getargs.h"
**
**	char **getargs(argc, argv, optv, descrip)
**	int argc;
**	char **argv, *descrip;
**	OPTION_T **optv;
**
** DESCRIPTION
**	Getargs fetches all options from the command-line arguments pointed to
**	by argv, according to the option descriptors pointed to by optv.  The
**	program description string "descrip" is saved externally for future
**	usage message generation.
**
** RESTRICTIONS
**	The calling program must NOT call ipwgetopt().
**
** RETURN VALUE
**	Getargs returns a pointer to the first command-line operand in argv,
**	or NULL if the command line is incorrect.
**
** GLOBALS ACCESSED
**	_argv	set to argv
**
**	_descrip
**		set to descrip
**
**	_optv	set to optv
**
**	Also, several globals defined by ipwgetopt() are accessed; these
**	globals should not be accessed by any other code.
**
** APPLICATION USAGE
**	Getargs is called early in the main program.  Optv is usually a static
**	array, which is in turn initialized with the addresses of static option
**	descriptors, one per possible option.  For example:
**
**		main(argc, argv)
**			int		argc;
**			char		*argv[];
**		{
**
**	"-a" option; no arguments
**
**			static OPTION_T	opt_a = {
**				'a',
**				"(option description)",
**			};
**
**	"-b" option; >= 3 integer arguments
**
**			static OPTION_T	opt_b = {
**				'b',
**				"(option description)",
**				INT_OPTARGS,
**				"(arg description)",
**				OPTIONAL,
**				3,
**			};
**
**	"-c" option; >= 1 real argument; must be specified
**
**			static OPTION_T	opt_c = {
**				'c',
**				"(option description)",
**				REAL_OPTARGS,
**				"(arg description)",
**				REQUIRED,
**			};
**
**	"-d" option; >= 1 and <= 4 string arguments; must be specified
**
**			static OPTION_T	opt_d = {
**				'd',
**				"(option description)",
**				STR_OPTARGS,
**				"(arg description)",
**				REQUIRED,
**				1,
**				4
**			};
**
**	operand.  The "placeholder" is the single word placed on the synopsis
**	line in the usage message.  {REQUIRED,OPTIONAL} and the {min,max}
**	number of "arguments" (i.e. operands) are interpreted analogously to
**	options.
**
**	This example describes the most common situation, 0 or more string
**	operands (e.g. file names).
**
**			static OPTION_T operands = {
**				OPERAND,
**				"(operand description)",
**				STR_OPERANDS,
**				"(operand placeholder)",
**				OPTIONAL
**			};
**
**	vector of option descriptors; must be 0-terminated.  Operand
**	descriptors must FOLLOW all option descriptors.
**
**			static OPTION_T	*optv[] = {
**				&opt_a,
**				&opt_b,
**				&opt_c,
**				&opt_d,
**				&operands,
**				0
**			};
**
**			argv = getargs(argc, argv, optv, descrip);
**			if (argv == NULL) {
**				usage();
**			}
**		}
**
** FUTURE DIRECTIONS
**	It should not be necessary to separately define the option descriptors
**	whose addresses are placed in optv.
**
** BUGS
**	Getargs calls ipwgetopt(), an implemenatation of the commonly
**	available getopt().
**
**	Non-string operands (i.e., values other than STR_OPERANDS) are not
**	supported.
*/

/*
 * is_int_str -- true if arg can be converted to integer
 */

static
int
is_int_str(p)
	REG_1 char     *p;		/* -> string to be tested	 */
{
	SKIP_SPACE(p);
 /*
  * sign
  */
	if (*p == '+' || *p == '-') {
		++p;
	}
 /*
  * mantissa
  */
	SKIP_DIGITS(p);

	SKIP_SPACE(p);

	return (*p == EOS);
}

/*
 * is_real_str -- true if arg can be converted to floating-point
 */

static
int
is_real_str(p)
	REG_1 char     *p;		/* -> string to be tested	 */
{
	SKIP_SPACE(p);
 /*
  * sign
  */
	if (*p == '+' || *p == '-') {
		++p;
	}
 /*
  * mantissa
  */
	SKIP_DIGITS(p);

	if (*p == '.') {
		++p;
	}

	SKIP_DIGITS(p);
 /*
  * exponent
  */
	if (*p == 'e' || *p == 'E') {
		++p;

		if (*p == '+' || *p == '-') {
			++p;
		}

		SKIP_DIGITS(p);
	}

	SKIP_SPACE(p);

	return (*p == EOS);
}

/*
 * cvt_ipwoptargs -- type-convert option arguments
 */

static
int
cvt_ipwoptargs(optp)
	REG_2 OPTION_T *optp;		/* -> option descriptor		 */
{
	aint_t         *aint_p;		/* -> integer option-argument	 */
	along_t        *along_p;	/* -> long int option-argument	 */
	areal_t        *areal_p;	/* -> real option-argument	 */
	astr_t         *astr_p;		/* -> string option-argument	 */
	REG_4 char     *my_ipwoptargs;	/* -> private copy of ipwoptargs	 */
	REG_3 int       n_ipwoptargs;	/* # ipwoptargs found		 */
	REG_1 char     *raw_p;		/* -> raw ipwoptarg string		 */

	assert(ipwoptarg != NULL);
 /*
  * make own copy of ipwoptargs
  */
	my_ipwoptargs = (char *) ecalloc((int) strlen(ipwoptarg) + 1, sizeof(char));
	if (my_ipwoptargs == NULL) {
		usrerr("can't allocate private copy of ipwoptargs");
		return (ERROR);
	}

	(void) strcpy(my_ipwoptargs, ipwoptarg);
 /*
  * count and EOS-terminate ipwoptargs
  */
	n_ipwoptargs = 1;

	for (raw_p = my_ipwoptargs; *raw_p != EOS; ++raw_p) {
		if (*raw_p == ',') {
			*raw_p = EOS;
			++n_ipwoptargs;
		}
	}
 /*
  * too few ipwoptargs? (min_nargs == 0 means don't care)
  */
	if (optp->min_nargs > 0) {
		if (n_ipwoptargs < optp->min_nargs) {
			usrerr("missing argument(s) for option \"-%c\"",
			       optp->option);
			return (ERROR);
		}
	}
 /*
  * too many ipwoptargs? (max_nargs == 0 means don't care)
  */
	if (optp->max_nargs > 0) {
		assert(optp->max_nargs >= optp->min_nargs);

		if (n_ipwoptargs > optp->max_nargs) {
			usrerr("too many arguments for option \"-%c\"",
			       optp->option);
			return (ERROR);
		}
	}

	optp->nargs = n_ipwoptargs;
 /*
  * allocate space for type-converted ipwoptargs; do type conversion
  */
	switch (optp->type) {

	case STR_OPTARGS:
 /* NOSTRICT */
		astr_p = (astr_t *) ecalloc(n_ipwoptargs, sizeof(astr_t));
		if (astr_p == NULL) {
			usrerr("can't allocate ipwoptarg array");
			return (ERROR);
		}

		(optp->args).astr_p = astr_p;

		raw_p = my_ipwoptargs;

		do {
			*astr_p++ = a_to_astr(raw_p);

			while (*raw_p++ != EOS) {
				continue;
			}
		} while (--n_ipwoptargs > 0);

		break;

	case INT_OPTARGS:
 /* NOSTRICT */
		aint_p = (aint_t *) ecalloc(n_ipwoptargs, sizeof(aint_t));
		if (aint_p == NULL) {
			usrerr("can't allocate ipwoptarg array");
			return (ERROR);
		}

		(optp->args).aint_p = aint_p;

		raw_p = my_ipwoptargs;

		do {
			if (!is_int_str(raw_p)) {
				usrerr("bad argument \"%s\": not an integer",
				       raw_p);
				return (ERROR);
			}

			*aint_p++ = a_to_aint(raw_p);

			while (*raw_p++ != EOS) {
				continue;
			}
		} while (--n_ipwoptargs > 0);

		break;

	case LONG_OPTARGS:
 /* NOSTRICT */
		along_p = (along_t *) ecalloc(n_ipwoptargs, sizeof(along_t));
		if (along_p == NULL) {
			usrerr("can't allocate ipwoptarg array");
			return (ERROR);
		}

		(optp->args).along_p = along_p;

		raw_p = my_ipwoptargs;

		do {
			if (!is_int_str(raw_p)) {
				usrerr("bad argument \"%s\": not an integer",
				       raw_p);
				return (ERROR);
			}

			*along_p++ = a_to_along(raw_p);

			while (*raw_p++ != EOS) {
				continue;
			}
		} while (--n_ipwoptargs > 0);

		break;

	case REAL_OPTARGS:
 /* NOSTRICT */
		areal_p = (areal_t *) ecalloc(n_ipwoptargs, sizeof(areal_t));
		if (areal_p == NULL) {
			usrerr("can't allocate ipwoptarg array");
			return (ERROR);
		}

		(optp->args).areal_p = areal_p;

		raw_p = my_ipwoptargs;

		do {
			if (!is_real_str(raw_p)) {
				usrerr("bad argument \"%s\": not a number",
				       raw_p);
				return (ERROR);
			}

			*areal_p++ = a_to_areal(raw_p);

			while (*raw_p++ != EOS) {
				continue;
			}
		} while (--n_ipwoptargs > 0);

		break;

	default:
		bug("bad option type");
	}
 /*
  * if STR_OPTARGS, then caller gets pointers to the EOS-terminated ipwoptargs;
  * otherwise, throw them away
  */
	if (optp->type != STR_OPTARGS) {
		free(my_ipwoptargs);
	}

	return (OK);
}


char          **
getargs(argc, argv, optv, descrip)
	int             argc;		/* # command-line argumnents	 */
	char          **argv;		/* -> command-line arguments	 */
	REG_2 OPTION_T **optv;		/* -> option descriptors	 */
	char           *descrip;	/* -> program descripton string	 */

{

	bool_t          get_operands;	/* ? process operands		 */
	REG_1 int       i;		/* loop counter			 */
	REG_3 int       n_opts;		/* # option descriptors		 */
	REG_4 int       option;		/* current option		 */
	REG_6 char     *optstring;	/* legal option letters		 */
	REG_5 char     *optstr_p;	/* -> optstring			 */

	assert(argc > 0);
	assert(argv != NULL);
 /*
  * set globals
  */
	_argv = argv;
	_descrip = descrip;
	_optv = optv;
 /*
  * special case: "pgm -H" means print usage message
  */
	if (argc == 2 && streq(argv[1], "-H")) {
		return (NULL);
	}
 /*
  * turn off error messages from ipwgetopt()
  */
	ipwopterr = 0;

	if (optv == NULL) {
 /*
  * trivial case: no options requested; so barf if any supplied
  */
		if (ipwgetopt(argc, argv, "") != EOF) {
			usrerr("no options allowed");
			return (NULL);
		}

		return (&argv[ipwoptind]);
	}
 /*
  * count option requests
  */
	get_operands = FALSE;
	for (n_opts = 0; optv[n_opts] != 0; ++n_opts) {
		if (optv[n_opts]->option == OPERAND) {
			get_operands = TRUE;
		}
	}

	if (get_operands && n_opts <= 1) {
		goto operands;
	}
 /*
  * allocate an "optstring" for ipwgetopt(): leave room for ':' suffixes, plus
  * EOS
  */
	optstring = (char *) ecalloc(2 * n_opts + 1, sizeof(char));
	if (optstring == NULL) {
		usrerr("can't allocate optstring for ipwgetopt()");
		return (NULL);
	}
 /*
  * load opstring with key-letters of requested options
  */
	optstr_p = optstring;

	for (i = 0; i < n_opts; ++i) {
		*optstr_p++ = optv[i]->option;

		if (optv[i]->type != NO_OPTARGS) {
 /*
  * add suffix indicating option with arguments
  */
			*optstr_p++ = ':';
		}
	}

	*optstr_p = EOS;
 /*
  * loop through command-line options
  */
	while ((option = ipwgetopt(argc, argv, optstring)) != EOF) {
 /*
  * look for matching option request
  */
		for (i = 0; i < n_opts; ++i) {
			if (optv[i]->option == option) {
				break;
			}
		}

		if (i >= n_opts) {
 /*
  * non-requested option
  */
			usrerr("bad option \"-%c\"", ipwoptopt);
			return (NULL);
		}
 /*
  * found a match, now decode ipwoptargs
  */
		if (optv[i]->type == NO_OPTARGS) {
 /*
  * no ipwoptargs, just note presence of option
  */
			optv[i]->nargs = 1;
		}
		else if (cvt_ipwoptargs(optv[i]) == ERROR) {
 /*
  * ipwoptarg type-conversion failed
  */
			return (NULL);
		}
	}
 /*
  * make sure there aren't any more options on the command line
  */
	if (ipwgetopt(argc, argv, "") != EOF) {
		usrerr("bad option \"-%c\"", ipwoptopt);
		return (NULL);
	}
 /*
  * make sure that all required options were supplied
  */
	for (i = 0; i < n_opts; ++i) {
		if (optv[i]->required
		    && optv[i]->nargs < 1
		    && optv[i]->option != OPERAND) {
			usrerr("required option \"-%c\" missing",
			       optv[i]->option);
			return (NULL);
		}
	}

	free(optstring);
operands:
 /*
  * process operand requests, if any
  * 
  * NB: should expand this someday to accommodate non-string operands
  */
	for (i = 0; i < n_opts; ++i) {
		if (optv[i]->option == OPERAND) {
			int             n_operands;	/* # operands	 */

			if (optv[i]->type != STR_OPERANDS) {
				bug("only STR_OPERANDS allowed");
			}

			n_operands = argc - ipwoptind;

			if (optv[i]->required &&
			    n_operands < optv[i]->min_nargs) {
				usrerr("required operand(s) missing");
				return (NULL);
			}

			if (optv[i]->max_nargs > 0 &&
			    n_operands > optv[i]->max_nargs) {
				usrerr("too many operands");
				return (NULL);
			}

			optv[i]->nargs = n_operands;
			(optv[i]->args).astr_p = &argv[ipwoptind];

			break;
		}
	}
 /*
  * return pointer to first operand
  */
	return (&argv[ipwoptind]);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/args/RCS/getargs.c,v 1.21 1993/02/02 08:29:30 jimbo Exp $";

#endif
