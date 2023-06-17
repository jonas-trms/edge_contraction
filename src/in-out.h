#ifndef IN_OUT
#define IN_OUT

#include "access_operations.h"

// import functions
int import_edge(vertex *a, vertex *b, edge **edges, int e_size, int *edge_nb,
                face *f);

void import(FILE *input, vertex **vertices, int vertex_nb, edge **edges,
            int *edge_nb, int e_size, face **faces, int face_nb);
 
// export functions
void export(FILE *output, vertex **vertices, int vertex_nb, face **faces,
            int face_nb);

void free_object(vertex **vertices, int vertex_nb, edge **edges, int edge_nb,
                 face **faces, int face_nb);
#endif