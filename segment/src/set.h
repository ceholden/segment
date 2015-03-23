/* set.h 1.2  2/24/89 */

#ifndef TYPEDEF_H
#define TYPEDEF_H
typedef char   *addr_t;		/* generic address               */
typedef int     bool_t;		/* TRUE or FALSE                 */
#endif  TYPEDEF_H

addr_t          create_set();
void            init_set();
bool_t          add_to_set();
void            begin_set();
bool_t          get_from_set();
void            destroy_set();
