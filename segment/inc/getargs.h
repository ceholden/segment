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

#ifndef	GETARGS_H
#define	GETARGS_H

/*
 * getargs() command-line argument handler
 */

#ifndef	IPW_H
#define	FALSE		0
#define	TRUE		1
typedef int     bool_t;

#endif

typedef int     aint_t;
typedef long    along_t;
typedef double  areal_t;
typedef char   *astr_t;

typedef union {
	aint_t         *aint_p;
	along_t        *along_p;
	areal_t        *areal_p;
	astr_t         *astr_p;
} ARGS_T;

typedef struct {
	char            option;
	char           *descrip;
	int             type;
	char           *arg_descrip;
	bool_t          required;
	int             min_nargs;
	int             max_nargs;
	int             nargs;
	ARGS_T          args;
} OPTION_T;

/* values of "option" */
#define	OPERAND		0		/* DON'T CHANGE!		 */

/* values of "type" */
#define	NO_OPTARGS	0		/* DON'T CHANGE!		 */
#define	INT_OPTARGS	1
#define	REAL_OPTARGS	2
#define	STR_OPTARGS	3
#define	LONG_OPTARGS	4

#define	INT_OPERANDS	INT_OPTARGS
#define	REAL_OPERANDS	REAL_OPTARGS
#define	STR_OPERANDS	STR_OPTARGS
#define	LONG_OPERANDS	LONG_OPTARGS

/* values of "required" */
#define	OPTIONAL	FALSE		/* DON'T CHANGE			 */
#define	REQUIRED	TRUE		/* DON'T CHANGE			 */

#define	a_to_aint(p)		atoi(p)
#define	a_to_along(p)		atol(p)
#define	a_to_areal(p)		atof(p)
#define	a_to_astr(p)		(p)

#define	n_args(opt)		( (opt).nargs )
#define	got_opt(opt)		( n_args(opt) > 0 )

#define	int_argp(opt)		( (opt).args.aint_p )
#define	long_argp(opt)		( (opt).args.along_p )
#define	real_argp(opt)		( (opt).args.areal_p )
#define	str_argp(opt)		( (opt).args.astr_p )

#define	int_arg(opt, i)		( ((opt).args.aint_p)[i] )
#define	long_arg(opt, i)	( ((opt).args.along_p)[i] )
#define	real_arg(opt, i)	( ((opt).args.areal_p)[i] )
#define	str_arg(opt, i)		( ((opt).args.astr_p)[i] )

#define	n_opnds(opt)		n_args(opt)
#define	got_opnds(opt)		got_opt(opt)

#define	str_opnd(opt, i)	str_arg(opt, i)

extern char    *cmdline();
extern char   **getargs();
extern void     usage();

/* $Header: /local/share/pkg/ipw/h/RCS/getargs.h,v 1.8 90/11/11 16:59:34 frew Exp $ */
#endif
