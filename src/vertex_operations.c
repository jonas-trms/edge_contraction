#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <cblas.h>
#include <lapacke.h>

#include "vertex_operations.h"

//projection subproblem
void project_points(double *original_vertices, int original_size, barycenter_node *barycenters,
face **faces, int face_nb){
  for (int i = 0; i < original_size; i++) {
    //finds closest point on mesh by projecting on all faces
    double v_closest[3];
    double v_tmp[3];
    int face_closest = 0;
    double dist_curr = square_dist_v_f(&original_vertices[i*3], faces[0], v_closest);

    for (int j = 1; j < face_nb; j++) {
      double dist_tmp = square_dist_v_f(&original_vertices[i], faces[j], v_tmp);

      if (dist_tmp < dist_curr) {
        dist_curr = dist_tmp;
        face_closest = j;
        cblas_dcopy(3, v_tmp, 1, v_closest, 1);
      }
    }

    //computes and sets barycenters
    face *f = faces[face_closest];
    barycenters[i].a = (f->a)->ind;
    barycenters[i].b = (f->b)->ind;
    barycenters[i].c = (f->c)->ind;

    double a[3] = {(f->a)->x, (f->a)->y, (f->a)->z};
    double b[3] = {(f->b)->x, (f->b)->y, (f->b)->z};
    double c[3] = {(f->c)->x, (f->c)->y, (f->c)->z};

    barycentric_calc(v_closest, a, b, c, &barycenters[i].k_a, &barycenters[i].k_b, &barycenters[i].k_c);
  }
}

//linear least squares subproblem
void improve_vertex_positions(double *original_vertices, int original_size, barycenter_node *barycenters,
vertex **vertices, int vertex_nb, edge **edges, int edge_nb, int k){
  //inits d_1
  double *d_1 = malloc(sizeof(double) * ((original_size + edge_nb) * 3));
  for (int i=0; i<original_size; i++){
    d_1[i] = original_vertices[i*3];
  }
  for (int i=0; i<edge_nb; i++){
    d_1[original_size + i] = 0.;
  }

  for (int i=0; i<original_size; i++){
    d_1[(original_size + edge_nb) + i] = original_vertices[i*3 + 1];
  }
  for (int i=0; i<edge_nb; i++){
    d_1[(original_size + edge_nb) + original_size + i] = 0.;
  }

  for (int i=0; i<original_size; i++){
    d_1[2*(original_size + edge_nb) + i] = original_vertices[i*3 + 2];
  }
  for (int i=0; i<edge_nb; i++){
    d_1[2*(original_size + edge_nb) + original_size + i] = 0.;
  }

  //inits mat_A
  double *mat_A = malloc(sizeof(double) * (vertex_nb * (original_size + edge_nb)));

  //barycenters
  for (int i=0; i<original_size; i++){
    for (int j=0; j<vertex_nb; j++){
      //index of b.i.j in mat_A (barycentric coordinate of i on j)
      if (barycenters[i].a != vertices[barycenters[i].a]->ind){
        printf("Error : construction\n");
      }

      if (barycenters[i].b != vertices[barycenters[i].b]->ind){
        printf("Error : construction\n");
      }

      if (barycenters[i].c != vertices[barycenters[i].c]->ind){
        printf("Error : construction\n");
      }

      int mat_ind = (j * (original_size + edge_nb)) + i;

      if (barycenters[i].a == vertices[j]->ind){
        mat_A[mat_ind] = barycenters[i].k_a;
      }

      else if (barycenters[i].b == vertices[j]->ind){
        mat_A[mat_ind] = barycenters[i].k_b;
      }

      else if (barycenters[i].c == vertices[j]->ind){
        mat_A[mat_ind] = barycenters[i].k_c;
      }

      else{
        mat_A[mat_ind] = 0.0;
      }
    }
  }

  //edges
  int k_root = sqrt(k);

  for (int i=0; i<edge_nb; i++){
    for (int j=0; j<vertex_nb; j++){
      //index of e.i.j in mat_A (coordinate of edges[i] on j)
      int mat_ind = (j * (original_size + edge_nb)) + i + original_size;

      if ((edges[i]->a)->ind == vertices[j]->ind){
        mat_A[mat_ind] = k_root;
      }

      else if ((edges[i]->b)->ind == vertices[j]->ind){
        mat_A[mat_ind] = -k_root;
      }

      else{
        mat_A[mat_ind] = 0.0;
      }
    }
  }
  
  FILE *output = fopen("d_1.txt", "w");

  for (int i=0; i<(original_size + edge_nb); i++){
      fprintf(output, "%+12.8f ", d_1[i]);
      fprintf(output, "%+12.8f ", d_1[(original_size + edge_nb) + i]);
      fprintf(output, "%+12.8f\n", d_1[2*(original_size + edge_nb) + i]);
    }

  fclose(output);

  output = fopen("mat_A.txt", "w");

  for (int i=0; i<(original_size + edge_nb); i++){
    for (int j=0; j<vertex_nb; j++){
      int mat_ind = (j * (original_size + edge_nb)) + i;
      fprintf(output, "%+12.8f ", mat_A[mat_ind]);
    }
    fprintf(output, "\n");
  }

  fclose(output); 

  //solves the linear least squares problem
  double *s = malloc(sizeof(double) * min(vertex_nb, (original_size + edge_nb)));
  lapack_int rank;

  lapack_int info = LAPACKE_dgelss(LAPACK_COL_MAJOR, (original_size + edge_nb), vertex_nb, 3, mat_A, (original_size + edge_nb), d_1, (original_size + edge_nb), s, -1., &rank);
  free(s);
  
  if (info != 0){
    printf("Error linear_least_squares_x : %d\n", info);
  }

  if ((original_size + edge_nb) < vertex_nb){
    printf("Error : m > n + e\n");
  }

  for (int i=0; i<vertex_nb; i++){
    vertices[i]->x = d_1[i];
  }

  for (int i=0; i<vertex_nb; i++){
    vertices[i]->y = d_1[(original_size + edge_nb) + i];
  }

  for (int i=0; i<vertex_nb; i++){
    vertices[i]->z = d_1[2*(original_size + edge_nb) + i];
  }

  free(d_1);
  free(mat_A);
}