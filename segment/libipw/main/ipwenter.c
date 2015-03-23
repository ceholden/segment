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

#include "getargs.h"

#include "bih.h"

/* 
** NAME
**         ipwenter -- initialize an IPW main program
**  
** 
** SYNOPSIS
**         #include "getargs.h"
** 
**         void ipwenter(argc, argv, optv, descrip)
**         int argc;
**         char **argv, *descrip;
**         OPTION_T *optv[];
**  
** 
** DESCRIPTION
**         ipwenter is called to initialize an IPW main program.  Its
**         chief function is to parse the command-line arguments in
**         {argv} The caller provides, via {optv}, descriptions of each
**         option that the program is prepared to accept, including the
**         number and type of any "optargs" (option arguments) allowed
**         for each option.
** 
**         The string {descrip} should be a one-line description of the
**         function of the program; it is saved externally for future
**         usage message generation.
**  
** 
** ERRORS
**         Too numerous to list.  Basically, any incorrect command line
**         causes ipwenter to terminate program execution with an
**         explanatory diagnostic.
**  
** 
** GLOBALS ACCESSED
**         _argv           set to {argv}
** 
**         _descrip        set to {descrip}
** 
**         _optv           set to {optv}
** 
**         These globals are used by the error handling subsystem to
**         construct standard-format error messages.
**  
** 
** APPLICATION USAGE
**         A call to ipwenter must be the first executable statement in
**         an IPW main program.  Since the calling sequence of ipwenter
**         is rather complicated, the following example is presented.
** 
**         All C programs running in a UNIX environment have access to
**         their command line arguments via the parameters {argc} and
**         {argv}, which are passed to the program's main function:
** 
**                 main(argc, argv)
**                         int             argc;
**                         char          **argv;
**                 {
**                         ...
** 
**         The application programmer specifies each option that the
**         program is prepared to accept with an "option descriptor".
**         An option descriptor is a structure containing the following
**         fields, in order:
** 
**                 field                   type    default
**                 ----------------------  ------  --------------------
**                 option letter           char    (must be specified)
** 
**                 option description      string  (must be specified)
** 
**         (The following fields may be omitted if there are no
**         optargs.)
** 
**                 optarg type             macro   NO_OPTARGS
** 
**                 optarg description      string  (must be specified)
** 
**                 required flag           macro   OPTIONAL
** 
**                 min. number of optargs  int     1
** 
**                 max. number of optargs  int     (unlimited)
** 
**         The option descriptors are declared static so that they may
**         be initialized.  Here are some sample option descriptors:
** 
**         This option descriptor specifies the "-a" option, which
**         takes no arguments:
** 
**                         static OPTION_T opt_a = {
**                                 'a', "(option description)",
**                         };
** 
**         This option descriptor specifies the "-b" option, which
**         takes at least 3 integer arguments:
** 
**                         static OPTION_T opt_b = {
**                                 'b', "(option description)",
**                                 INT_OPTARGS, "(arg description)",
**                                 OPTIONAL, 3,
**                         };
** 
**         This option descriptor specifies the "-c" option, which must
**         be present, and which takes at least 1 real argument,
** 
**                         static OPTION_T opt_c = {
**                                 'c', "(option description)",
**                                 REAL_OPTARGS, "(arg description)",
**                                 REQUIRED,
**                         };
** 
**         This option descriptor specifies the "-d" option, which must
**         be present, and which takes at least 1 and no more than 4
**         string arguments:
** 
**                         static OPTION_T opt_d = {
**                                 'd', "(option description)",
**                                 STR_OPTARGS, "(arg description)",
**                                 REQUIRED, 1, 4
**                         };
** 
**         This option descriptor specifies 0 or more string operands.
**         The "placeholder" is the single word placed on the synopsis
**         line in the usage message.  {REQUIRED,OPTIONAL} and the
**         {min,max} number of "arguments" (i.e. operands) are
**         interpreted analogously to options.
** 
**                         static OPTION_T operands = {
**                                 OPERAND, "(operand description)",
**                                 STR_OPERANDS, "(placeholder)",
**                         };
** 
**         Once all of the options and operands have been specified, a
**         null-terminated array of pointers to the option and operand
**         descriptors must be constructed.  The pointer to the operand
**         descriptor (if any) must be the last non-null pointer in the
**         array.  Like the option descriptors, this array is declared
**         static so that it may be initialized:
** 
**                         static OPTION_T *optv[] = {
**                                 &opt_a,
**                                 &opt_b,
**                                 &opt_c,
**                                 &opt_d,
**                                 &operands,
**                                 0
**                         };
** 
**                         void ipwenter(argc, argv, optv,
**                                       "(program description)")
** 
**         After ipwenter returns, the options are accessible by
**         applying macros to the option descriptors.  These
**         macros include ({opt} is an option descriptor):
** 
**                 got_opt(opt)    TRUE if {opt} was specified on the
**                                 command line.
** 
**                 n_args(opt)     number of optargs associated with
**                                 {opt}.
** 
**                 int_arg(opt, i) integer value of the {i}'th optarg
**                                 (0-relative) associated with {opt}.
** 
**                 int_argp(opt)   pointer to array of integer-valued
**                                 optargs associated with {opt}.
** 
**         There are also str, long, and real versions of the _arg and
**         _argp macros.
**  
** 
** SEE ALSO
**         IPW: gethdrs, ipwexit, opt_check
** 
**         UNIX: getopt
** 
**         K. Hemenway and H. Armitage, 1984.  "Proposed syntax
**                 standard for UNIX system commands".  UNIX World,
**                 vol. 1, no. 3, pp. 54-57.
**  
** 
** NOTES
**         The calling program must NOT call UNIX library function
**         getopt.
** 
**         Non-string operands (i.e., values other than STR_OPERANDS)
**         are not yet supported.
*/  

void
ipwenter(argc, argv, optv, descrip)
	int             argc;		/* argc (from main())	 	 */
	char          **argv;		/* argv (from main())	 	 */
	OPTION_T      **optv;		/* -> option descriptors	 */
	char           *descrip;	/* program description string	 */
{
	if (getargs(argc, argv, optv, descrip) == NULL) {
		usage();
	}
 /*
  * initialize o_byteorder to hostorder() - may be changed after
  * ipwenter if a routine plans to bypass the pixio byte swapping
  * layer (e.g., transpose, flip, and window)
  */
	o_byteorder = hostorder();
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/main/RCS/ipwenter.c,v 1.8 1997/02/17 04:24:53 ceretha Exp $";

#endif
