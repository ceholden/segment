#ifndef lint
static char     SCCS_ID[] = "set.c 1.3  4/9/89";

#endif

/*
 *	This file contains a package of routines to manipulate sets,
 *	that is unordered collections of elements without duplicate members.
 *	The user can create a set, empty a set, add a member to a set,
 *	prepare to read a set, get a member from a set, and destroy
 *	a set.  Set elements can be chars, shorts, or longs.  The user
 *	specifies the element type and maximum length when he creates the
 *	set.  The routines maintain a minimal sense of state -- a set
 *	is either set for reading (SREAD) or writing (SWRITE).  One cannot
 *	read from a set in the SWRITE state and vice versa.
 *
 *	The package uses a very simple member entry algorithm,
 *	straight linear lookup, because the initial application
 *	that it was written for normally only uses short sets.
 *	A more sophisticated algorithm should be used for long
 *	sets.
 *
 *
 *	addr_t
 *	create_set( nelem, size )
 *	unisgned int nelem;
 *	unsigned int size;
 *
 *	Create a set that will be able to contain nelem elements of
 *	appropriate size.  Size must be 1, 2, or 4, which specify
 *	char, short, and int respectively.  The set is initialized to
 *	state SWRITE.  If the necessary storage is not available, the
 *	routine returns NULL, otherwise a pointer which should be used
 *	in subsequent calls which reference the created set.
 *
 *
 *	void
 *	init_set( Sp )
 *	addr_t Sp;
 *
 *	Empty the set of any previous contents and put it into state
 *	SWRITE.
 *
 *
 *	bool_t
 *	add_to_set( Sp, pitem )
 *	addr_t  Sp;
 *	addr_t  pitem;
 *
 *	Add item pointed to by pitem to the set.  The type of item must
 *	be the same as that the set was created with.  The routine returns
 *	TRUE if there was room for the item, and FALSE otherwise.  If the
 *	set is not in state SWRITE, this routine will exit to the system
 *	with an error message.
 *
 *
 *	void
 *	begin_set( Sp )
 *	addr_t Sp;
 *
 *	Put the set pointed to by sp into state SREAD and prepare to
 *	return the first item upon a succeeding get_from_set().
 *
 *
 *	bool_t
 *	get_from_set( Sp, pitem )
 *	addr_t Sp;
 *	addr_t  pitem;
 *
 *	Get the next item from the set pointed to by Sp and return it
 *	in the location pointed to by pitem.  The location pointed to
 *	by pitem must be capable of holding the type of item that the
 *	set was created to hold.  The routine returns FALSE if all the
 *	items have already been read from the set, and TRUE otherwise.
 *	If the set is not in state SREAD, this routine will exit to the
 *	system with an error message.
 *
 *
 *	void
 *	destroy_set( Sp )
 *	addr_t Sp;
 *
 *	Destroy the set pointed to by Sp and free the associated storage.
 *
 */

#include  <assert.h>
#include  <stdio.h>

#include  "set_p.h"
#include  "set.h"

char           *malloc();
char           *calloc();
void            free();
void            exit();

addr_t
create_set(nelem, size)
unsigned int    nelem;
unsigned int    size;
{
    REG_1 SET_T    *S;

    if ((S = (SET_T *) LINT_CAST(malloc(sizeof(SET_T)))) == NULL)
	return (NULL);

    if (size == 1 || size == 2 || size == 4) {
	S->size = size;
    } else {
	fprintf(stderr, "create_set:  illegal element size %d\n", size);
	exit(1);
    }

    if ((S->items = (SITEM *) LINT_CAST(calloc(nelem, sizeof(SITEM))))
    == NULL) {
	free((char *) S);
	return (NULL);
    }
    S->n = nelem;
    S->limitp = &S->items[nelem];
    S->getp = S->addp = S->items;
    S->state = SWRITE;

    return ((addr_t) S);
}


void
init_set(Sp)
addr_t          Sp;
{
    REG_1 SET_T    *S;

    S = (SET_T *) LINT_CAST(Sp);
    S->state = SWRITE;
    S->getp = S->addp = S->items;
}


bool_t
add_to_set(Sp, pitem)
addr_t          Sp;
REG_2 addr_t    pitem;
{
    REG_1 SET_T    *S;
    REG_3 SITEM    *ip;

    S = (SET_T *) LINT_CAST(Sp);

    if (S->state != SWRITE) {
	fprintf(stderr, "add_to_set: set is not in state SWRITE\n");
	exit(1);
    }

 /*
  * Check if item is already in the set.  Check list of set items starting
  * with the most recently added item to optimize search.
  */
    ip = S->addp;

    switch (S->size) {

	case 1:
	    while (ip != S->items) {
		if ((--ip)->citem == *((char *) pitem))
		    return (TRUE);	/* Found it.  Don't have to add it */
	    }
	    break;

	case 2:
	    while (ip != S->items) {
		if ((--ip)->sitem == *((short *) LINT_CAST(pitem)))
		    return (TRUE);	/* Found it.  Don't have to add it */
	    }
	    break;

	case 4:
	    while (ip != S->items) {
		if ((--ip)->litem == *((long *) LINT_CAST(pitem)))
		    return (TRUE);	/* Found it.  Don't have to add it */
	    }
	    break;

	default:
	    assert(FALSE);
    }

 /*
  * Do we have enough room for another item?
  */
    if (S->addp != S->limitp) {

	switch (S->size) {	/* Yes.  Add it. */

	    case 1:
		(S->addp++)->citem = *((char *) pitem);
		break;

	    case 2:
		(S->addp++)->sitem = *((short *) LINT_CAST(pitem));
		break;

	    case 4:
		(S->addp++)->litem = *((long *) LINT_CAST(pitem));
		break;

	    default:
		assert(FALSE);

	}
	return (TRUE);

    } else {

    /*
     * Not enough room for another item.
     */
	return (FALSE);
    }
}


void
begin_set(Sp)
addr_t          Sp;
{
    REG_1 SET_T    *S;

    S = (SET_T *) LINT_CAST(Sp);
    S->state = SREAD;
    S->getp = S->items;
}


bool_t
get_from_set(Sp, pitem)
addr_t          Sp;
addr_t          pitem;
{
    REG_1 SET_T    *S;

    S = (SET_T *) LINT_CAST(Sp);

    if (S->state != SREAD) {
	fprintf(stderr, "get_from_set: set is not in state SREAD\n");
	exit(1);
    }

 /*
  * Are we already at the end of the element list?
  */
    if (S->getp == S->addp) {
	return (FALSE);		/* Yes */
    } else {

	switch (S->size) {	/* No */

	    case 1:
		*((char *) pitem) = (S->getp++)->citem;
		break;

	    case 2:
		*((short *) LINT_CAST(pitem)) = (S->getp++)->sitem;
		break;

	    case 4:
		*((long *) LINT_CAST(pitem)) = (S->getp++)->litem;
		break;

	    default:
		assert(FALSE);
	}

	return (TRUE);
    }
}


void
destroy_set(Sp)
addr_t          Sp;
{
    SET_T          *S;

    S = (SET_T *) LINT_CAST(Sp);
    free((char *) S->items);
    free((char *) S);
}

