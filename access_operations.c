#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "access_operations.h"

// local iteration
void iterate_f_neighbours_e(edge *e, void (*f)(face *)) {
  face_node *curr = (e->faces)->start;

  while (curr != NULL) {
    f(curr->f);
    curr = curr->next;
  }
}

void iterate_e_neighbours_f(face *f, void (*fn)(edge *)) {
  edge_node *curr = (f->edges)->start;

  while (curr != NULL) {
    fn(curr->e);
    curr = curr->next;
  }
}

void iterate_e_neighbours_v(vertex *v, void (*f)(edge *)) {
  edge_node *curr = (v->edges)->start;

  while (curr != NULL) {
    f(curr->e);
    curr = curr->next;
  }
}

// get adjacent vertices
vertex *adjacent_v_e(edge *e, vertex *v) {
  if (((e->a)->id == v->id)) {
    return e->b;
  }

  else if (((e->b)->id == v->id)) {
    return e->a;
  }

  else {
    printf("Erreur adjacent_v_e\n");
    return NULL;
  }
}

vertex_chain *adjacents_v(vertex *v) {
  edge_node *curr = (v->edges)->start;

  vertex_chain *res = new_vchain();

  while (curr != NULL) {
    insert_v_id(adjacent_v_e(curr->e, v), res);
    curr = curr->next;
  }

  return res;
}

vertex_chain *adjacent_i_j(vertex *i, vertex *j) {
  vertex_chain *adj_i = adjacents_v(i);
  vertex_chain *adj_j = adjacents_v(j);

  vertex_chain *adj_i_j = intersect_v_chains(adj_i, adj_j);

  free_v_chain(adj_i);
  free_v_chain(adj_j);

  return adj_i_j;
}

// get edge of two vertices
edge *adjacent_e_v(vertex *v1, vertex *v2, int i) {
  // i : ith adjacent edge
  if (v1 == NULL || v2 == NULL) {
    printf("Erreur adj_e_v\n");
  }

  vertex *vmin;
  vertex *vmax;

  // sorts vertices in lex order
  vertex_chain *new = new_vchain();
  insert_v_id(v1, new);
  insert_v_id(v2, new);

  // adds vertices to edge
  vmin = (new->start)->v;
  vmax = ((new->start)->next)->v;
  free_v_chain(new);

  edge *res = NULL;

  edge_node *curr = (vmin->edges)->start;
  while (res == NULL && curr != NULL) {
    if ((((curr->e)->a)->id == vmin->id) && (((curr->e)->b)->id == vmax->id)) {
      if (i == 1) {
        res = curr->e;
      }

      else {
        i = 1;
      }
    }

    curr = curr->next;
  }

  return res;
}