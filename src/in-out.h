#ifndef IN_OUT
#define IN_OUT

#include "access_operations.h"

// import functions
int import_edge(vertex *a, vertex *b, edge **edges, int e_size, int *edge_nb,
                face *f);

void import(FILE *input, double *original_vertices, vertex **vertices, int vertex_nb, edge **edges,
            int *edge_nb, int e_size, face **faces, int face_nb);

// copy functions
void copy_object(vertex **vertices, int *vertex_nb, int *edge_nb, face **faces,
                 int *face_nb, vertex **v_dest, int *vertex_nb_dest,
                 edge **e_dest, int e_size_copy, int *edge_nb_dest,
                 face **f_dest, int *face_nb_dest);
 
// export functions
void export(FILE *output, vertex **vertices, int vertex_nb, face **faces,
            int face_nb);

void free_object(vertex **vertices, int vertex_nb, edge **edges, int edge_nb,
                 face **faces, int face_nb);
#endif