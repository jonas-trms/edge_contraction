#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "remove_operations.h"

// remove operations
void remove_f_succ(edge *e) {
  // e_global_rem : if face deletion is called by edge deletion
  if (e != e_global_rem) {
    remove_f_chain(f_global_rem, e->faces);
  }
}

void remove_f(face *f, face **faces, int *face_nb) {
  // tab
  int ind = f->ind;
  faces[ind] = faces[(*face_nb) - 1];
  faces[ind]->ind = ind;
  (*face_nb)--;

  // edges
  f_global_rem = f;
  iterate_e_neighbours_f(f, &remove_f_succ);

  free_e_chain(f->edges);
  free(f);
}

// edge removal
void remove_f_one_arg(face *f) { remove_f(f, faces_global, face_nb_global); }

void remove_e(edge *e, edge **edges, int *edge_nb, face **faces, int *face_nb) {
  // tab
  int ind = e->ind;
  edges[ind] = edges[(*edge_nb) - 1];
  edges[ind]->ind = ind;
  (*edge_nb)--;

  // faces
  e_global_rem = e;
  faces_global = faces;
  face_nb_global = face_nb;

  iterate_f_neighbours_e(e, &remove_f_one_arg);

  // vertices
  remove_e_chain(e, (e->a)->edges);
  remove_e_chain(e, (e->b)->edges);

  // free e
  free_f_chain(e->faces);
  free(e);
}

// merge vertices
void merge_v_face(face *f, vertex *a, vertex *b) {
  if ((f->a)->id == a->id) {
    f->a = b;
  }

  else if ((f->b)->id == a->id) {
    f->b = b;
  }

  else if ((f->c)->id == a->id) {
    f->c = b;
  }

  // sorts vertices in lex order
  vertex_chain *new = new_vchain();
  insert_v_id(f->a, new);
  insert_v_id(f->b, new);
  insert_v_id(f->c, new);

  // adds vertices to current face
  f->a = (new->start)->v;
  f->b = ((new->start)->next)->v;
  f->c = (((new->start)->next)->next)->v;
  free_v_chain(new);
}

void merge_v_face_one_arg(face *f) { merge_v_face(f, global_a, global_b); }

void merge_v_edge(edge *e, vertex *a, vertex *b) {
  if ((e->a)->id == a->id) {
    e->a = b;
  }

  else if ((e->b)->id == a->id) {
    e->b = b;
  }

  // sorts vertices in lex order
  vertex_chain *new = new_vchain();
  insert_v_id(e->a, new);
  insert_v_id(e->b, new);

  // adds vertices to current face
  e->a = (new->start)->v;
  e->b = ((new->start)->next)->v;
  free_v_chain(new);

  iterate_f_neighbours_e(e, &merge_v_face_one_arg);
}

void merge_v_edge_one_arg(edge *e) { merge_v_edge(e, global_a, global_b); }

void insert_e_one_arg(edge *e) { insert_e(e, global_b->edges); }

void merge_v(vertex *a, vertex *b, vertex **vertices, int *vertex_nb) {
  // replaces a with b (doesn't deleted a)

  // merge for neighbours
  global_a = a;
  global_b = b;
  iterate_e_neighbours_v(a, &merge_v_edge_one_arg);
  iterate_e_neighbours_v(a, insert_e_one_arg);

  // mean position for b
  b->x = (a->x + b->x) / 2;
  b->y = (a->y + b->y) / 2;
  b->z = (a->z + b->z) / 2;

  // deletes a
  int ind = a->ind;
  vertices[ind] = vertices[(*vertex_nb) - 1];
  vertices[ind]->ind = ind;
  (*vertex_nb)--;

  free_e_chain(a->edges);
  free(a);
}

// merge edges
void replace_e_chain_one_arg(face *f) {
  remove_e_chain(global_edge_replaced, f->edges);
  insert_e(global_edge_replacer, f->edges);
}

void insert_f_one_arg(face *f) { insert_f(f, global_edge_replacer->faces); }

void merge_e(edge *e1, edge *e2, edge **edges, int *edge_nb, face **faces,
             int *face_nb) {
  // replaces e1 with e2 if compare_e_ids(e1, e2) == 0
  if (compare_e_ids(e1, e2) != 0) {
    printf("Erreur e1 != e2\n");
    printf("e1 : %d %d, e2 : %d %d\n", (e1->a)->id, (e1->b)->id, (e2->a)->id,
           (e2->b)->id);
  }

  if (e1 == NULL || e2 == NULL) {
    printf("Erreur Null\n");
  }
  for (int i = 0; i < *face_nb; i++) {
    edge_node *test = member_e(e1, faces[i]->edges);
    if (test != NULL) {
      face_node *autre = member_f(faces[i], e1->faces);
      if (autre == NULL) {
        printf("Erreur autre\n");
      }
    }
  }

  // merge lists
  global_edge_replaced = e1;
  global_edge_replacer = e2;
  iterate_f_neighbours_e(e1, &replace_e_chain_one_arg);
  iterate_f_neighbours_e(e1, &insert_f_one_arg);
  remove_e_chain(e1, (e1->a)->edges);
  remove_e_chain(e1, (e1->b)->edges);

  // frees e1
  int ind = e1->ind;
  free_f_chain(e1->faces);
  free(e1);
  if (ind != (*edge_nb) - 1) {
    edges[ind] = edges[(*edge_nb) - 1];
    edges[ind]->ind = ind;
  }
  (*edge_nb)--;
}