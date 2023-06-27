#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <cblas.h>
#include <lapacke.h>

#include "distance_operations.h"

//closest point on a face to a point
double *closest_point_face(double *p, double *a,
                                  double *b, double *c) {
  double ab[3] = {0., 0., 0.};
  cblas_daxpy(3, 1., b, 1, ab, 1);
  cblas_daxpy(3, -1., a, 1, ab, 1);

  double ac[3] = {0., 0., 0.};
  cblas_daxpy(3, 1., c, 1, ac, 1);
  cblas_daxpy(3, -1., a, 1, ac, 1);

  double ap[3] = {0., 0., 0.};
  cblas_daxpy(3, 1., p, 1, ap, 1);
  cblas_daxpy(3, -1., a, 1, ap, 1);

  double d1 = cblas_ddot(3, ab, 1, ap, 1);
  double d2 = cblas_ddot(3, ac, 1, ap, 1);
  if (d1 <= 0.f && d2 <= 0.f) {
    return a;
  } // #1

  double bp[3] = {0., 0., 0.};
  cblas_daxpy(3, 1., p, 1, bp, 1);
  cblas_daxpy(3, -1., b, 1, bp, 1);
  double d3 = cblas_ddot(3, ab, 1, bp, 1);
  double d4 = cblas_ddot(3, ac, 1, bp, 1);
  if (d3 >= 0.f && d4 <= d3) {
    return b;
  } // #2

  double cp[3] = {0., 0., 0.};
  cblas_daxpy(3, 1., p, 1, cp, 1);
  cblas_daxpy(3, -1., c, 1, cp, 1);
  double d5 = cblas_ddot(3, ab, 1, cp, 1);
  double d6 = cblas_ddot(3, ac, 1, cp, 1);

  if (d6 >= 0.f && d5 <= d6) {
    return c;
  } // #3

  double vc = d1 * d4 - d3 * d2;
  double *res = malloc(3*sizeof(double));
  for (int i=0; i<3; i++){
    res[i] = 0.;
  }

  if (vc <= 0.f && d1 >= 0.f && d3 <= 0.f) {
    double v = d1 / (d1 - d3);
    cblas_daxpy(3, v, ab, 1, res, 1);
    cblas_daxpy(3, 1., a, 1, res, 1);
    return res; // #4
  }

  double vb = d5 * d2 - d1 * d6;
  if (vb <= 0.f && d2 >= 0.f && d6 <= 0.f) {
    double v = d2 / (d2 - d6);
    cblas_daxpy(3, v, ac, 1, res, 1);
    cblas_daxpy(3, 1., a, 1, res, 1);
    return res; // #5
  }

  double va = d3 * d6 - d5 * d4;
  if (va <= 0.f && (d4 - d3) >= 0.f && (d5 - d6) >= 0.f) {
    double v = (d4 - d3) / ((d4 - d3) + (d5 - d6));
    cblas_daxpy(3, v, c, 1, res, 1);
    cblas_daxpy(3, 1. -v, b, 1, res, 1);
    return res; // #6
  }

  double denom = 1.f / (va + vb + vc);
  double v = vb * denom;
  double w = vc * denom;
  cblas_daxpy(3, v, ab, 1, res, 1);
  cblas_daxpy(3, w, ac, 1, res, 1);
  cblas_daxpy(3, 1., a, 1, res, 1);
  return res; // #0
}

//barycentric coordinates of a point in a triangle
void barycentric_calc(double *p, double *a, double *b, double *c, double *k_a, double *k_b, double *k_c){
  //p = k_a*a + k_b*b + k_c*c
  //the function modifies k_a, k_b and k_c
  
  double mat_abc[12] = {a[0], a[1], a[2], 1.,
                       b[0], b[1], b[2], 1.,
                       c[0], c[1], c[2], 1.};

  double p_and_1[4] = {p[0], p[1], p[2], 1.};

  double *s = malloc(sizeof(double) * 3);
  lapack_int rank;

  lapack_int info = LAPACKE_dgelss(LAPACK_COL_MAJOR, 4, 3, 1, mat_abc, 4, p_and_1, 4, s, -1., &rank);
  free(s);

  *k_a = p_and_1[0];
  *k_b = p_and_1[1];
  *k_c = p_and_1[2];
  
  if (info != 0){
  printf("p : %lf %lf %lf\na : %lf %lf %lf\nb : %lf %lf %lf\nc : %lf %lf %lf\n",
  p[0], p[1], p[2],
  a[0], a[1], a[2],
  b[0], b[1], b[2],
  c[0], c[1], c[2]);
  printf("Barycenters : %lf %lf %lf\n\n", *k_a, *k_b, *k_c);}
}

//square distances calculations
double square_dist_v(double *v1, double *v2){
  double v1v2[3] = {0., 0., 0.};
  cblas_daxpy(3, 1., v2, 1, v1v2, 1);
  cblas_daxpy(3, -1., v1, 1, v1v2, 1);

  double dist = cblas_ddot(3, v1v2, 1, v1v2, 1);
  return dist;
}

double square_dist_v_f(double *v, face *f, double *p_copy){
  //computes dist and copies closest point to p_copy
  double a[3] = {(f->a)->x, (f->a)->y, (f->a)->z};
  double b[3] = {(f->b)->x, (f->b)->y, (f->b)->z};
  double c[3] = {(f->c)->x, (f->c)->y, (f->c)->z};

  double *p = closest_point_face(v, a, b, c);
  if (p_copy != NULL){
    cblas_dcopy(3, p, 1, p_copy, 1);
  }

  double dist = square_dist_v(v, p);

  if (a != p && b != p && c != p) {
    free(p);
  }

  return dist;
}


