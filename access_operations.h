#ifndef ACCESS_OPERATIONS
#define ACCESS_OPERATIONS

#include "data_structures.h"
#include "linked_lists.h"

// local iteration
void iterate_f_neighbours_e(edge *e, void (*f)(face *));
void iterate_e_neighbours_f(face *f, void (*fn)(edge *));
void iterate_e_neighbours_v(vertex *v, void (*f)(edge *));

// get adjacent vertices
vertex *adjacent_v_e(edge *e, vertex *v);
vertex_chain *adjacents_v(vertex *v);
vertex_chain *adjacent_i_j(vertex *i, vertex *j);

// get edge of two vertices
edge *adjacent_e_v(vertex *v1, vertex *v2, int i);

#endif