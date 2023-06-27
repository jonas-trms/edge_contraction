#ifndef ENERGY
#define ENERGY

#include "vertex_operations.h"

double energy(double crep, double k, double *original_vertices,
              int original_size, int vertex_nb, edge **edges, int edge_nb,
              face **faces, int face_nb);

#endif