#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "edge_operations.h"

// edge collapse
int min(int a, int b) {
  if (a <= b) {
    return a;
  } else {
    return b;
  }
}

int edge_collapse(edge *e, vertex **vertices, int *vertex_nb, edge **edges,
                  int *edge_nb, face **faces, int *face_nb) {
  int cond2 = cond_edge_collapse_boundary(e, *vertex_nb);

  // cond_adj_i_j
  vertex *i = e->a;
  vertex *j = e->b;

  vertex_chain *adj_i_j = adjacent_i_j(i, j);

  vertex_node *curr = adj_i_j->start;

  int cond1 = 1;
  while (cond1 == 1 && curr != NULL) {
    vertex *k = curr->v;

    if (exists_f_e(e, i, j, k) == 0) {
      cond1 = 0;
    }

    curr = curr->next;
  }

  int cond = min(cond1, cond2);

  if (cond == 1) {
    // removes considered edges
    remove_e(e, edges, edge_nb, faces, face_nb);

    // merges vertices
    edge_nb_global = edge_nb;
    edges_global = edges;
    merge_v(i, j, vertices, vertex_nb);

    // deletes double edges
    curr = adj_i_j->start;
    while (curr != NULL) {
      vertex *k = curr->v;

      edge *e1 = adjacent_e_v(j, k, 1);
      edge *e2 = adjacent_e_v(j, k, 2);

      if (e1 == NULL || e2 == NULL) {
        printf("Erreur edge adjacent i k ou j k\n");
      }

      merge_e(e1, e2, edges, edge_nb, faces, face_nb);

      curr = curr->next;
    }
  }

  free_v_chain(adj_i_j);

  return cond;
}