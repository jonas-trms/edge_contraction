#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "energy.h"

// energy function
double e_dist(double *original_vertices, int original_size,
              face **faces, int face_nb) {
  double res = 0;

  for (int i = 0; i < original_size; i++) {
    double dist_curr = square_dist_v_f(&original_vertices[3*i], faces[0], NULL);

    for (int j = 1; j < face_nb; j++) {
      double tmp = square_dist_v_f(&original_vertices[3*i], faces[j], NULL);

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
      double a[3] = {(edges[i]->a)->x, (edges[i]->a)->y, (edges[i]->a)->z};
      double b[3] = {(edges[i]->b)->x, (edges[i]->b)->y, (edges[i]->b)->z};
      res += k * square_dist_v(a, b);
    }
  }

  return res;
}

double energy(double crep, double k, double *original_vertices,
              int original_size, int vertex_nb, edge **edges, int edge_nb,
              face **faces, int face_nb) {
  return e_dist(original_vertices, original_size, faces, face_nb) +
         crep * (double)(vertex_nb) + e_spring(edges, edge_nb, k);
}