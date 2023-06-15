#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "in-out_energy.h"

// in-out
// import functions
int import_edge(vertex *a, vertex *b, edge **edges, int e_size, int *edge_nb) {
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
      edges[i]->id = i;
      edges[i]->ind = i;
      (*edge_nb)++;
      j = 1;
    }

    else {
      return -1;
    }
  }

  return j;
}

void import_original(FILE *input, vertex_static **original_vertices,
                     int vertex_nb) {
  for (int i = 0; i < vertex_nb; i++) {
    original_vertices[i] = malloc(sizeof(vertex_static));
    fscanf(input, "v %lf %lf %lf\n", &(original_vertices[i]->x),
           &(original_vertices[i]->y), &(original_vertices[i]->z));
  }
}

void import_obj(FILE *input, vertex **vertices, int vertex_nb, edge **edges,
                int *edge_nb, int e_size, face **faces, int face_nb) {
  *edge_nb = 0;

  // imports vertices
  for (int i = 0; i < vertex_nb; i++) {
    // imports current vertex
    vertices[i] = malloc(sizeof(vertex));
    vertices[i]->id = i;
    vertices[i]->ind = i;
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

    // imports edges
    import_edge(faces[i]->a, faces[i]->b, edges, e_size, edge_nb);
    import_edge(faces[i]->a, faces[i]->c, edges, e_size, edge_nb);
    import_edge(faces[i]->b, faces[i]->c, edges, e_size, edge_nb);
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
    free(vertices[i]);
  }

  for (int i = 0; i < edge_nb; i++) {
    free(edges[i]);
  }

  for (int i = 0; i < face_nb; i++) {
    free(faces[i]);
  }
}