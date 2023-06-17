#ifndef LINKED_LISTS
#define LINKED_LISTS

#include "data_structures.h"
#include "comparison.h"

// creation
vertex_chain *new_vchain(void);
edge_chain *new_echain(void);
face_chain *new_fchain(void);

// freeing lists
void free_v_chain(vertex_chain *vchain);
void free_e_chain(edge_chain *echain);
void free_f_chain(face_chain *fchain);

// member test
vertex_node *member_v(vertex *v, vertex_chain *vchain);
edge_node *member_e(edge *e, edge_chain *echain);
face_node *member_f(face *f, face_chain *fchain);

// insertion
int insert_v(vertex *v, vertex_chain *vchain);
int insert_e(edge *e, edge_chain *echain);
int insert_f(face *f, face_chain *fchain);

// remove operations
int remove_v_chain(vertex *v, vertex_chain *vchain);
int remove_e_chain(edge *e, edge_chain *echain);
int remove_f_chain(face *f, face_chain *fchain);

// sort by id
int insert_v_id(vertex *v, vertex_chain *vchain);

// intersection (chains already sorted by id)
vertex_chain *intersect_v_chains(vertex_chain *a, vertex_chain *b);

#endif