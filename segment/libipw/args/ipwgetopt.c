/* LINTLIBRARY */

#ifdef	IPW
#include "ipw.h"
#else
#include <stdio.h>
extern char    *strchr();
extern int      strcmp();

#define	REG_1	register
#define	REG_2	register
#define	REG_3	register
#endif

/*
** NAME
**	ipwgetopt -- get option letter from argument vector
**
** SYNOPSIS
**	#include <stdio.h>
**
**	int ipwgetopt(argc, argv, optstring)
**	int argc;
**	char *argv[], *optstring;
**
** DESCRIPTION
**	"Ipwgetopt" is a (crude) command-line parser.  It returns the next
**	option letter in "argv" that matches a letter in "optstring".
**
**	"Argc" and "argv" are the command-line argument count and vector,
**	respectively, obtained as arguments to main().
**
**	"Optstring" is a string of recognized option letters.  If a letter is
**	followed by a colon, then the option is expected to have an argument
**	that may or may not be separated from it by white space.
**
** RETURN VALUE
**	EOF is returned when all options have been processed.  '?' is returned
**	if an unrecognized (i.e., not present in "optstring") option letter is
**	encountered.
**
** GLOBALS ACCESSED
**	ipwoptind	initially 1; set to the "argv" index of the next
**			argument to be processed
**
**	ipwoptarg	initially NULL; set to point to the current
**			option-argument
**
** NOTES
**	"ipwgetopt()" is derived from the "getopt()" posted to USENET on 03
**	November 1985 by John Quarterman, which was asserted to be
**	public-domain code from AT&T.
**
**	IPW uses "ipwgetopt()" because the common "getopt()" function
**	is broken on some Unix systems.
*/

int             ipwoptind = 1;		/* argv index of next argument	 */
char           *ipwoptarg = NULL;	/* -> current option-argument	 */
int             ipwoptopt = 0;		/* current option letter	 */
/* LINT: ipwopterr not used but included for SVID compatibility */
int             ipwopterr = 0;		/* ? print error message	 */

int
ipwgetopt(argc, argv, optstring)
	int             argc;		/* # of command-line arguments	 */
	REG_1 char    **argv;		/* -> command-line arguments	 */
	char           *optstring;	/* legal option letters		 */
{
 /*
  * argv[ipwoptind][sp] is current option letter
  */
	static int      sp = 1;


	REG_2 int       c;		/* current option letter	 */
	REG_3 char     *cp;		/* -> option letter in "optstring" */

	if (sp == 1) {
		if (ipwoptind >= argc
		    || argv[ipwoptind][0] != '-' || argv[ipwoptind][1] == '\0') {
			return (EOF);
		}

		if (strcmp(argv[ipwoptind], "--") == 0) {
			++ipwoptind;
			return (EOF);
		}
	}

	c = argv[ipwoptind][sp];
	ipwoptopt = c;

	if (c == ':' || (cp = strchr(optstring, c)) == (char *) NULL) {
		if (argv[ipwoptind][++sp] == '\0') {
			++ipwoptind;
			sp = 1;
		}

		return ('?');
	}

	if (*++cp == ':') {
		if (argv[ipwoptind][sp + 1] != '\0') {
			ipwoptarg = &argv[ipwoptind++][sp + 1];
		}
		else if (++ipwoptind >= argc) {
			sp = 1;
			return ('?');
		}
		else {
			ipwoptarg = argv[ipwoptind++];
		}

		sp = 1;
	}
	else {
		if (argv[ipwoptind][++sp] == '\0') {
			sp = 1;
			++ipwoptind;
		}

		ipwoptarg = NULL;
	}

	return (c);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/args/RCS/ipwgetopt.c,v 1.1 1993/02/02 08:30:15 jimbo Exp $";

#endif
