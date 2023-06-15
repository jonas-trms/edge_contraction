#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "access_operations.h"
#include "condition_tests.h"
#include "edge_operations.h"
#include "in-out.h"
#include "remove_operations.h"

int main(int argc, char *argv[]) {
  // syntax for use : first arg is input, second arg is output, third is desired
  // ratio
  double ratio = atof(argv[3]);
  int vertex_nb_init;

  // opens input, reads vertex and face numbers
  int vertex_nb;
  int edge_nb;
  int face_nb;

  FILE *input = fopen(argv[1], "r");
  fscanf(input, "#%d vertices; %d faces\n\n", &vertex_nb, &face_nb);

  vertex_nb_init = vertex_nb;

  // creates tabs
  int v_size = 2 * vertex_nb, e_size = 6 * face_nb, f_size = 2 * face_nb;
  vertex **vertices = malloc(v_size * sizeof(vertex *));
  edge **edges = malloc(e_size * sizeof(edge *));
  face **faces = malloc(f_size * sizeof(face *));

  // imports objects
  clock_t start, end;
  start = clock();

  import(input, vertices, vertex_nb, edges, &edge_nb, e_size, faces, face_nb);
  fclose(input);

  end = clock();

  printf("\nObject imported\n%d Vertices, %d Edges, %d Faces\nTime elapsed : "
         "%lf s\n\n",
         vertex_nb, edge_nb, face_nb,
         (double)(end - start) / (double)CLOCKS_PER_SEC);

  srand(1);
  long operation_nb = 0;
  long i = 0;

  start = clock();

  while ((double)vertex_nb_init / (double)vertex_nb < ratio) {
    int r = rand() % edge_nb;

    int tmp = edge_collapse(edges[r], vertices, &vertex_nb, edges, &edge_nb,
                            faces, &face_nb);

    operation_nb += tmp;
    i++;
  }

  printf("Total collapses : %ld\n", operation_nb);

  FILE *output = fopen(argv[2], "w");
  export(output, vertices, vertex_nb, faces, face_nb);
  fclose(output);

  end = clock();

  free_object(vertices, vertex_nb, edges, edge_nb, faces, face_nb);
  free(vertices);
  free(edges);
  free(faces);

  printf("\nObject reduced %lf times\n%d Vertices, %d Edges, %d Faces\n"
         "Time elapsed : %lf s\n\n",
         (double)vertex_nb_init / (double)vertex_nb, vertex_nb, edge_nb,
         face_nb, (double)(end - start) / (double)CLOCKS_PER_SEC);
}