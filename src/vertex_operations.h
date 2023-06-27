#ifndef VERTEX_OPERATIONS
#define VERTEX_OPERATIONS

#include "distance_operations.h"

void project_points(double *original_vertices, int original_size, barycenter_node *barycenters,
face **faces, int face_nb);

void improve_vertex_positions(double *original_vertices, int original_size, barycenter_node *barycenters,
vertex **vertices, int vertex_nb, edge **edges, int edge_nb, int k);

#endif