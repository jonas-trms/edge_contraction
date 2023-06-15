#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "in-out_energy.h"
#include "projection.h"

// energy function
double e_dist(vertex_static **original_vertices, int original_size,
              face **faces, int face_nb) {
  double res = 0;

  for (int i = 0; i < original_size; i++) {
    double dist_curr = square_dist_v_f(original_vertices[i], faces[0]);

    for (int j = 1; j < face_nb; j++) {
      double tmp = square_dist_v_f(original_vertices[i], faces[j]);

      if (tmp < dist_curr) {
        dist_curr = tmp;
      }
    }

    res += dist_curr;
  }

  return res;
}

double e_spring(edge **edges, int edge_nb, double k) {
  double res = 0;

  if (k != 0) {
    for (int i = 0; i < edge_nb; i++) {
      vertex_static *a = v_to_static(edges[i]->a);
      vertex_static *b = v_to_static(edges[i]->b);
      res += k * square_dist_v(a, b);

      free(a);
      free(b);
    }
  }

  return res;
}

double energy(double crep, double k, vertex_static **original_vertices,
              int original_size, int vertex_nb, edge **edges, int edge_nb,
              face **faces, int face_nb) {
  return e_dist(original_vertices, original_size, faces, face_nb) +
         crep * (double)(vertex_nb) + e_spring(edges, edge_nb, k);
}

int main(int argc, char *argv[]) {
  // syntax for use : first arg is input1, second arg is input2, third is k,
  // fourth is crep
  double crep = atof(argv[3]);
  double k = atof(argv[4]);

  // time
  clock_t start, end;
  start = clock();

  // imports input1, reads vertex number
  int original_size;

  FILE *input1 = fopen(argv[1], "r");
  fscanf(input1, "#%d vertices; %*d faces\n\n", &original_size);

  vertex_static **original_vertices =
      malloc(original_size * sizeof(vertex_static));

  import_original(input1, original_vertices, original_size);

  fclose(input1);

  // imports input2
  int vertex_nb;
  int edge_nb;
  int face_nb;

  FILE *input2 = fopen(argv[2], "r");
  fscanf(input2, "#%d vertices; %d faces\n\n", &vertex_nb, &face_nb);

  // creates tabs
  int v_size = 2 * vertex_nb, e_size = 6 * face_nb, f_size = 2 * face_nb;
  vertex **vertices = malloc(v_size * sizeof(vertex *));
  edge **edges = malloc(e_size * sizeof(edge *));
  face **faces = malloc(f_size * sizeof(face *));

  // imports objects
  import_obj(input2, vertices, vertex_nb, edges, &edge_nb, e_size, faces,
             face_nb);
  fclose(input2);

  end = clock();
  printf("\nObjects imported\nTime elapsed : "
         "%lf s\n\n",
         (double)(end - start) / (double)CLOCKS_PER_SEC);

  start = clock();

  double e_calc = energy(crep, k, original_vertices, original_size, vertex_nb,
                         edges, edge_nb, faces, face_nb);

  // free
  free_object(vertices, vertex_nb, edges, edge_nb, faces, face_nb);
  free(vertices);
  free(edges);
  free(faces);

  for (int i = 0; i < original_size; i++) {
    free(original_vertices[i]);
  }
  free(original_vertices);

  end = clock();

  printf("\nEnergy : "
         "%lf\nTime elapsed : %lf s\n\n",
         e_calc, (double)(end - start) / (double)CLOCKS_PER_SEC);
}