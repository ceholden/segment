/* set_p.h 1.2  4/9/89 */

#include  "set.h"

#ifndef LINT_CAST
#ifdef  lint
#define LINT_CAST(e)	( (e) ? 0 : 0 )
#else
#define LINT_CAST(e)	( e )
#endif  lint
#endif  LINT_CAST

#define REG_1	register
#define REG_2	register
#define REG_3	register
#define REG_4	register
#define REG_5	register
#define REG_6	register

typedef enum {
    SWRITE,
    SREAD
}               SSTATE;

typedef union {
    char            citem;
    short           sitem;
    long            litem;
}               SITEM;

typedef struct {
    short           size;	/* 1, 2, or 4: size of elemnt in bytes */
    long            n;		/* maximum length in elements */
    SITEM          *limitp;	/* ptr to the last position + 1 in set
				 * storage */
    SITEM          *addp;	/* ptr to next element to add */
    SITEM          *getp;	/* ptr to next element to get */
    SSTATE          state;	/* SWRITE or SREAD */
    SITEM          *items;	/* ptr to beginning of set storage */
}               SET_T;

#ifndef TRUE
#define TRUE  1
#define FALSE 0
#endif



