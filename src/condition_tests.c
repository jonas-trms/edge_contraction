#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "condition_tests.h"

// condition tests
// boundary
int is_boundary_e(edge *e) {
  if ((e->faces)->start != NULL && ((e->faces)->start)->next == NULL) {
    return 1;
  }

  else {
    return 0;
  }
}

int is_boundary_v(vertex *v) {
  edge_node *curr = (v->edges)->start;

  int i = 0;

  while (i == 0 && curr != NULL) {
    i = is_boundary_e(curr->e);
    curr = curr->next;
  }

  return i;
}

// existence
int exists_f_e(edge *e, vertex *i, vertex *j, vertex *k) {
  // sorts vertices in lex order
  vertex_chain *new = new_vchain();
  insert_v_id(i, new);
  insert_v_id(j, new);
  insert_v_id(k, new);
  vertex *a = (new->start)->v;
  vertex *b = ((new->start)->next)->v;
  vertex *c = (((new->start)->next)->next)->v;
  free_v_chain(new);

  // exists ?
  face_node *curr = (e->faces)->start;
  int res = 0;

  while (res == 0 && curr != NULL) {
    face *f = malloc(sizeof(face));
    f->a = a;
    f->b = b;
    f->c = c;

    if (compare_f_ids(f, curr->f) == 0) {
      res = 1;
    }

    else {
      curr = curr->next;
    }

    free(f);
  }
  return res;
}

int cond_edge_collapse_boundary(edge *e, int vertex_nb) {
  // cond on boundary
  int cond2 = 0;
  int b_a = is_boundary_v(e->a);
  int b_b = is_boundary_v(e->b);
  if ((b_a == 1) && (b_b == 1)) {
    cond2 = is_boundary_e(e);
  }

  else if ((b_a == 0) && (b_b == 0)) {
    if (vertex_nb >= 4) {
      cond2 = 1;
    }
  }

  else if (vertex_nb >= 3) {
    cond2 = 1;
  }

  return cond2;
}