#ifndef EDGE_OPERATIONS
#define EDGE_OPERATIONS

#include "access_operations.h"
#include "remove_operations.h"
#include "condition_tests.h"

int edge_collapse(edge *e, vertex **vertices, int *vertex_nb, edge **edges,
                  int *edge_nb, face **faces, int *face_nb);

#endif