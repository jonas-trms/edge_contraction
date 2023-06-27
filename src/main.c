#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "access_operations.h"
#include "condition_tests.h"
#include "edge_operations.h"
#include "in-out.h"
#include "remove_operations.h"
#include "vertex_operations.h"
#include "energy.h"

int main(int argc, char *argv[]){
  // syntax for use : first arg is input, second arg is output, third is k,
  // fourth is crep, fifth is iter_nb, sixth is subproblem_iter
  double k = atof(argv[3]);
  double crep = atof(argv[4]);
  long iter_nb = strtol(argv[5], NULL, 10);
  long subproblem_iter = strtol(argv[6], NULL, 10);

  // opens input, reads vertex and face numbers
  int vertex_nb;
  int edge_nb;
  int face_nb;

  FILE *input = fopen(argv[1], "r");
  fscanf(input, "#%d vertices; %d faces\n\n", &vertex_nb, &face_nb);

  // creates tabs
  int v_size = 2 * vertex_nb, e_size = 6 * face_nb, f_size = 2 * face_nb;
  vertex **vertices = malloc(v_size * sizeof(vertex *));
  edge **edges = malloc(e_size * sizeof(edge *));
  face **faces = malloc(f_size * sizeof(face *));

  int vertex_nb_copy = vertex_nb;
  int edge_nb_copy = edge_nb;
  int face_nb_copy = face_nb;
  vertex **vertices_copy = malloc(v_size * sizeof(vertex *));
  edge **edges_copy = malloc(e_size * sizeof(edge *));
  face **faces_copy = malloc(f_size * sizeof(face *));

  int original_size = vertex_nb;
  double *original_vertices = malloc(3 * original_size * sizeof(double));

  barycenter_node *barycenters = malloc(original_size * sizeof(barycenter_node));

  // imports objects
  clock_t start, end;
  start = clock();

  import(input, original_vertices, vertices, vertex_nb, edges, &edge_nb, e_size,
         faces, face_nb);
  fclose(input);

  copy_object(vertices, &vertex_nb, &edge_nb, faces, &face_nb, vertices_copy,
              &vertex_nb_copy, edges_copy, e_size, &edge_nb_copy, faces_copy,
              &face_nb_copy);

  double e_curr =
      energy(crep, k, original_vertices, original_size, vertex_nb_copy,
             edges_copy, edge_nb_copy, faces_copy, face_nb_copy);
  double e_init = e_curr;

  end = clock();

  printf("\nObject imported\n%d Vertices, %d Edges, %d Faces\nInitial energy : "
         "%lf\nTime elapsed : %lf s\n\n",
         vertex_nb, edge_nb, face_nb, e_curr,
         (double)(end - start) / (double)CLOCKS_PER_SEC);

  srand(1);
  long operation_nb = 0;

  start = clock();

  for (long i = 1; i <= iter_nb; i++) {
    int r = rand() % edge_nb;

    int tmp =
        edge_collapse(edges_copy[r], vertices_copy, &vertex_nb_copy, edges_copy,
                      &edge_nb_copy, faces_copy, &face_nb_copy);

    if (tmp == 1) {
      double e_bef =
          energy(crep, k, original_vertices, original_size, vertex_nb_copy,
                 edges_copy, edge_nb_copy, faces_copy, face_nb_copy);

      for (long i=0; i<subproblem_iter; i++){
        //printf("Ok1\n");

        project_points(original_vertices, original_size, barycenters, 
        faces_copy, face_nb_copy);

        //printf("Ok2\n");

        improve_vertex_positions(original_vertices, original_size, barycenters,
        vertices_copy, vertex_nb_copy, edges_copy, edge_nb_copy, k);

        //printf("Ok3\n");
      }

      double e_new =
          energy(crep, k, original_vertices, original_size, vertex_nb_copy,
                 edges_copy, edge_nb_copy, faces_copy, face_nb_copy);

      printf("e_curr : %lf, e_bef : %lf, e_new : %lf", e_curr, e_bef, e_new);

      if (1 == 1) {//e_new < e_curr
        e_curr = e_new;

        free_object(vertices, vertex_nb, edges, edge_nb, faces, face_nb);
        copy_object(vertices_copy, &vertex_nb_copy, &edge_nb_copy, faces_copy,
                    &face_nb_copy, vertices, &vertex_nb, edges, e_size,
                    &edge_nb, faces, &face_nb);
        operation_nb++;

      }

      else {
        free_object(vertices_copy, vertex_nb_copy, edges_copy, edge_nb_copy,
                    faces_copy, face_nb_copy);
        copy_object(vertices, &vertex_nb, &edge_nb, faces, &face_nb,
                    vertices_copy, &vertex_nb_copy, edges_copy, e_size,
                    &edge_nb_copy, faces_copy, &face_nb_copy);
      }
    }

    printf("\nOperations considered : %ld\nOperations done : %ld\nCurrent energy : %lf\n", i, operation_nb, e_curr);
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

  free_object(vertices_copy, vertex_nb_copy, edges_copy, edge_nb_copy,
              faces_copy, face_nb_copy);
  free(vertices_copy);
  free(edges_copy);
  free(faces_copy);

  free(original_vertices);
  free(barycenters);

  printf("\nObject reduced\n%d Vertices, %d Edges, %d Faces\nFinal energy : "
         "%lf\nTime elapsed : %lf s\n\n",
         vertex_nb, edge_nb, face_nb, e_curr,
         (double)(end - start) / (double)CLOCKS_PER_SEC);
}