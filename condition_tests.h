#ifndef CONDITION_TESTS
#define CONDITION_TESTS

#include "access_operations.h"

// boundary
int is_boundary_e(edge *e);
int is_boundary_v(vertex *v);

// existence
int exists_f_e(edge *e, vertex *i, vertex *j, vertex *k);

int cond_edge_collapse_boundary(edge *e, int vertex_nb);


#endif