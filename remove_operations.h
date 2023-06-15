#ifndef REMOVE_OPERATIONS
#define REMOVE_OPERATIONS

#include "access_operations.h"

// face removal
face *f_global_rem;
edge *e_global_rem;

void remove_f_succ(edge *e);
void remove_f(face *f, face **faces, int *face_nb);

// edge removal
face **faces_global;
int *face_nb_global;

void remove_f_one_arg(face *f);
void remove_e(edge *e, edge **edges, int *edge_nb, face **faces, int *face_nb);

// merge vertices
vertex *global_a;
vertex *global_b;
int *edge_nb_global;
edge **edges_global;

void merge_v_face(face *f, vertex *a, vertex *b);
void merge_v_face_one_arg(face *f);
void merge_v_edge(edge *e, vertex *a, vertex *b);
void merge_v_edge_one_arg(edge *e);
void insert_e_one_arg(edge *e);
void merge_v(vertex *a, vertex *b, vertex **vertices, int *vertex_nb);

// merge edges
edge *global_edge_replaced;
edge *global_edge_replacer;

void replace_e_chain_one_arg(face *f);
void insert_f_one_arg(face *f);
void merge_e(edge *e1, edge *e2, edge **edges, int *edge_nb, face **faces,
             int *face_nb);

#endif