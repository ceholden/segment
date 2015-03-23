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
**         ucopy -- copy data between files
**  
** 
** SYNOPSIS
**         long ucopy(fdi, fdo, ncopy)
**         int fdi, fdo;
**         long ncopy;
**  
** 
** DESCRIPTION
**         ucopy copies {ncopy} bytes from file descriptor {fdi}
**         to file descriptor {fdo}.
**  
** 
** RETURN VALUE
**         success:  number of bytes copied
** 
**         failure:  ERROR
**  
** 
** APPLICATION USAGE
**         ucopy is useful chiefly to programs that are copying all of
**         the pixel data from an input IPW image to an output non-IPW
**         image (see the documentation for the imgsize function.)
** 
**         For copying pixel data between IPW image files, use the
**         imgcopy function.
**  
** 
** SEE ALSO
**         IPW: imgcopy, imgsize, uropen
*/  

long
ucopy(fdi, fdo, ncopy)
	int             fdi;		/* input file descriptor	 */
	int             fdo;		/* output file descriptor	 */
	REG_3 long      ncopy;		/* # bytes to [try to] copy	 */
{
	REG_1 int       nbytes;		/* # bytes read into buffer	 */
	REG_2 long      ncopied;	/* total # bytes copied		 */

	char            buf[OPT_BUF];	/* I/O buffer			 */

	ncopied = 0;
 /*
  * loop until copy request is satisfied 
  */
	for (; ncopy > 0; ncopy -= nbytes) {
 /*
  * trim current read request to min(copy request, buffer size) 
  */
		if (ncopy < sizeof(buf)) {
 /* NOSTRICT */
			nbytes = ncopy;
		}
		else {
			nbytes = sizeof(buf);
		}
 /*
  * do the I/O 
  */
		nbytes = uread(fdi, buf, nbytes);
		if (nbytes <= 0) {
			if (nbytes < 0) {
				return (ERROR);
			}

			break;
		}

		if (uwrite(fdo, buf, nbytes) == ERROR) {
			return (ERROR);
		}
 /*
  * keep track of total # bytes copied 
  */
		ncopied += nbytes;
	}

	if (nbytes == ERROR) {
		return (ERROR);
	}

	return (ncopied);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/uio/RCS/ucopy.c,v 1.6 1997/02/17 04:12:09 ceretha Exp $";

#endif
