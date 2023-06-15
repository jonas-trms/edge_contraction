#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "in-out.h"

// in-out
// import functions
int import_edge(vertex *a, vertex *b, edge **edges, int e_size, int *edge_nb,
                face *f) {
  vertex *vmin;
  vertex *vmax;

  // sorts vertices in lex order
  vertex_chain *new = new_vchain();
  insert_v_id(a, new);
  insert_v_id(b, new);

  // adds vertices to edge
  vmin = (new->start)->v;
  vmax = ((new->start)->next)->v;
  free_v_chain(new);

  int i = 0;
  int j = -1;
  while (j == -1 && i < *edge_nb) {
    if (((edges[i]->a)->id == vmin->id) && ((edges[i]->b)->id == vmax->id)) {
      j = 0;
    }

    else {
      i++;
    }
  }

  if (j == -1) {
    if (i < e_size) {
      edges[i] = malloc(sizeof(edge));
      edges[i]->a = a;
      edges[i]->b = b;
      edges[i]->faces = new_fchain();
      edges[i]->id = i;
      edges[i]->ind = i;
      (*edge_nb)++;
      j = 1;
    }

    else {
      return -1;
    }
  }

  insert_e(edges[i], f->edges);
  insert_e(edges[i], a->edges);
  insert_e(edges[i], b->edges);
  insert_f(f, edges[i]->faces);

  return j;
}

void import(FILE *input, vertex **vertices, int vertex_nb, edge **edges,
            int *edge_nb, int e_size, face **faces, int face_nb) {
  *edge_nb = 0;

  // imports vertices
  for (int i = 0; i < vertex_nb; i++) {
    // imports current vertex
    vertices[i] = malloc(sizeof(vertex));
    vertices[i]->id = i;
    vertices[i]->ind = i;
    vertices[i]->edges = new_echain();
    fscanf(input, "v %lf %lf %lf\n", &(vertices[i]->x), &(vertices[i]->y),
           &(vertices[i]->z));
  }

  // imports faces
  fscanf(input, "\n");
  for (int i = 0; i < face_nb; i++) {
    // reads current face
    faces[i] = malloc(sizeof(face));
    faces[i]->id = i;
    faces[i]->ind = i;
    int v1, v2, v3;
    fscanf(input, "f %d %d %d\n", &v1, &v2, &v3);

    // sorts vertices in lex order
    vertex_chain *new = new_vchain();
    insert_v_id(vertices[v1 - 1], new);
    insert_v_id(vertices[v2 - 1], new);
    insert_v_id(vertices[v3 - 1], new);

    // adds vertices to current face
    faces[i]->a = (new->start)->v;
    faces[i]->b = ((new->start)->next)->v;
    faces[i]->c = (((new->start)->next)->next)->v;
    free_v_chain(new);

    // creates chain for face
    faces[i]->edges = new_echain();

    // imports edges
    import_edge(faces[i]->a, faces[i]->b, edges, e_size, edge_nb, faces[i]);
    import_edge(faces[i]->a, faces[i]->c, edges, e_size, edge_nb, faces[i]);
    import_edge(faces[i]->b, faces[i]->c, edges, e_size, edge_nb, faces[i]);
  }
}

// export functions
void export(FILE *output, vertex **vertices, int vertex_nb, face **faces,
            int face_nb) {
  fprintf(output, "#%d vertices; %d faces\n\n", vertex_nb, face_nb);

  // writes vertices
  for (int i = 0; i < vertex_nb; i++) {
    fprintf(output, "v %lf %lf %lf\n", vertices[i]->x, vertices[i]->y,
            vertices[i]->z);
  }

  fprintf(output, "\n");

  // writes faces
  for (int i = 0; i < face_nb; i++) {
    fprintf(output, "f %d %d %d\n", ((faces[i]->a)->ind) + 1,
            ((faces[i]->b)->ind) + 1, ((faces[i]->c)->ind) + 1);
  }
}

void free_object(vertex **vertices, int vertex_nb, edge **edges, int edge_nb,
                 face **faces, int face_nb) {
  for (int i = 0; i < vertex_nb; i++) {
    free_e_chain(vertices[i]->edges);
    free(vertices[i]);
  }

  for (int i = 0; i < edge_nb; i++) {
    free_f_chain(edges[i]->faces);
    free(edges[i]);
  }

  for (int i = 0; i < face_nb; i++) {
    free_e_chain(faces[i]->edges);
    free(faces[i]);
  }
}