/* LINTLIBRARY */

// #include <varargs.h>
#include <stdarg.h>

#include "ipw.h"

/* 
** NAME
**         allocnd --- allocate a multidimensional array
**  
** 
** SYNOPSIS
**         addr_t allocnd(elsize, ndim, dim1, ..., dimn)
**         int elsize, ndim, dim1, ..., dimn;
**  
** 
** DESCRIPTION
**         allocnd dynamically allocates an {ndim}-dimensional array,
**         where sizeof(each element) is {elsize}.  The particular
**         dimensions are given by the {dim1}, ..., {dimn} arguments.
**         The returned address should be cast into a pointer to the
**         proper type of object.
** 
**         The dynamically allocated array is indexed via dope vectors
**         (i.e., all but the rightmost dimension are implemented by
**         arrays of pointers).
** 
**         All space allocated by allocnd is initialized to 0.  The
**         memory occupied by array data (as opposed to the dope
**         vectors) will be logically contiguous, so the address of the
**         first element in the array may also be treated as the base
**         address of a single 1-dimensional array.
**  
** 
** RETURN VALUE
**         success:  a pointer to the newly-allocated array
** 
**         failure:  NULL
**  
** 
** ERRORS
**         size overflow during allocation
** 
**                 The total number of data bytes in the array could
**                 not be expressed as an int.
**  
** 
** APPLICATION USAGE
**         Use allocnd() as a general-purpose multidimensional array
**         allocator.  For example:
** 
**                 double  a[3][4];
**                 double  **b;
** 
**                 b = (double **)allocnd(sizeof(double), 2, 3, 4);
** 
**         References of the form b[i][j] will behave equivalently to
**         a[i][j].
**  
** 
** SEE ALSO
**         IPW: ecalloc
** 
**         UNIX: malloc
**  
** 
** NOTES
**         Because of the extra space required for the dope vectors,
**         arrays allocated by allocnd() will consume more memory than
**         equivalently-dimensioned arrays allocated statically.
*/  

/* VARARGS */
addr_t
allocnd (int element_size, int ndimensions, ...)
// allocnd(va_alist)
// va_dcl
{
	va_list         ap;		/* -> variable argument list	 */
	addr_t          array;		/* -> base of allocated memory	 */
	int             bsize;		/* total # bytes allocated	 */
	REG_6 int       dim;		/* current array dimension	 */
	REG_4 int       elsize;		/* sizeof(array element)	 */
	REG_3 int       i;		/* loop counter			 */
	int             ndim;		/* # array dimensions		 */
	int             size;		/* current dimension product	 */

 /*
  * collect and validate the arguments; calculate total # bytes we need to
  * allocate
  */
 /* NOSTRICT */
	va_start(ap, ndimensions);
 /* NOSTRICT */
	// elsize = va_arg(ap, int);
        elsize = element_size;
	assert(elsize > 0);
 /* NOSTRICT */
	// ndim = va_arg(ap, int);
        ndim = ndimensions;
	assert(ndim > 0);

	bsize = 0;
	size = 1;

	i = ndim;
	do {
 /* NOSTRICT */
		dim = va_arg(ap, int);
		assert(dim > 0);

		size *= dim;
		bsize += size * (i == 1 ? elsize : sizeof(addr_t));
	} while (--i > 0);

	va_end(ap);

	if (bsize <= 0) {
		usrerr("size overflow during allocation");
		return (NULL);
	}
 /*
  * allocate one big hunk of memory for data + dope vectors
  */
	array = ecalloc(bsize, sizeof(char));
	if (array == NULL) {
		return (NULL);
	}

	bzero((char *) array, bsize);
 /*
  * if 1-D array then all done
  */
	if (ndim < 2) {
		return (array);
	}
 /*
  * initialize the dope vectors
  */
 /* NOSTRICT */
	va_start(ap, ndimensions);

 /* NOSTRICT */
//	(void) va_arg(ap, int);
 /* NOSTRICT */
//	(void) va_arg(ap, int);

 /* NOSTRICT */
	dim = va_arg(ap, int);
	bsize = 0;
	size = 1;

	i = ndim - 1;
	do {
		REG_1 addr_t   *dope_p;	/* -> current dope vector	 */
		REG_5 int       j;	/* loop counter			 */
		REG_2 addr_t    next;	/* -> next subarray		 */

 /* NOSTRICT */
		dope_p = (addr_t *) (array + bsize);

		size *= dim;
		bsize += size * sizeof(addr_t);
		next = array + bsize;
 /* NOSTRICT */
		dim = va_arg(ap, int);

		j = size;
		do {
			*dope_p++ = next;
			next += dim * (i == 1 ? elsize : sizeof(addr_t));
		} while (--j > 0);
	} while (--i > 0);

	va_end(ap);
 /*
  * all done
  */
	return (array);
}

#ifndef	lint
static char     rcsid[] = "$Header: /share1/ipw-1.1/share/src/lib/libipw/util/RCS/allocnd.c,v 1.11 1997/02/17 04:27:12 ceretha Exp $";

#endif
