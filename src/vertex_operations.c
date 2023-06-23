#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "vertex_operations.h"
#include "matrix.h"

// basic operations on static vertices (vectors)
vertex_static *v_to_static(vertex *v) {
  vertex_static *res = malloc(sizeof(vertex_static));
  res->x = v->x;
  res->y = v->y;
  res->z = v->z;

  return res;
}

vertex_static *sum_v(vertex_static *v1, vertex_static *v2) {
  // v1 + v2
  vertex_static *res = malloc(sizeof(vertex_static));
  res->x = v1->x + v2->x;
  res->y = v1->y + v2->y;
  res->z = v1->z + v2->z;

  return res;
}

vertex_static *substraction_v(vertex_static *v1, vertex_static *v2) {
  // v1 - v2
  vertex_static *res = malloc(sizeof(vertex_static));
  res->x = v1->x - v2->x;
  res->y = v1->y - v2->y;
  res->z = v1->z - v2->z;

  return res;
}

vertex_static *scalar_v(double k, vertex_static *v1) {
  // k * v
  vertex_static *res = malloc(sizeof(vertex_static));
  res->x = k * v1->x;
  res->y = k * v1->y;
  res->z = k * v1->z;

  return res;
}

double dot(vertex_static *v1, vertex_static *v2) {
  return (v1->x) * (v2->x) + (v1->y) * (v2->y) + (v1->z) * (v2->z);
}

//closest point on a face to a point
vertex_static *closest_point_face(vertex_static *p, vertex_static *a,
                                  vertex_static *b, vertex_static *c) {
  vertex_static *ab = substraction_v(b, a);
  vertex_static *ac = substraction_v(c, a);
  vertex_static *ap = substraction_v(p, a);

  double d1 = dot(ab, ap);
  double d2 = dot(ac, ap);
  if (d1 <= 0.f && d2 <= 0.f) {
    free(ab);
    free(ac);
    free(ap);
    return a;
  } // #1

  vertex_static *bp = substraction_v(p, b);
  double d3 = dot(ab, bp);
  double d4 = dot(ac, bp);
  if (d3 >= 0.f && d4 <= d3) {
    free(ab);
    free(ac);
    free(ap);
    free(bp);
    return b;
  } // #2

  vertex_static *cp = substraction_v(p, c);
  double d5 = dot(ab, cp);
  double d6 = dot(ac, cp);

  if (d6 >= 0.f && d5 <= d6) {
    free(ab);
    free(ac);
    free(ap);
    free(bp);
    free(cp);
    return c;
  } // #3

  double vc = d1 * d4 - d3 * d2;
  if (vc <= 0.f && d1 >= 0.f && d3 <= 0.f) {
    double v = d1 / (d1 - d3);
    vertex_static *tmp = scalar_v(v, ab);
    vertex_static *res = sum_v(a, tmp);
    free(tmp);
    free(ab);
    free(ac);
    free(ap);
    free(bp);
    free(cp);
    return res; // #4
  }

  double vb = d5 * d2 - d1 * d6;
  if (vb <= 0.f && d2 >= 0.f && d6 <= 0.f) {
    const float v = d2 / (d2 - d6);
    vertex_static *tmp = scalar_v(v, ac);
    vertex_static *res = sum_v(a, tmp);
    free(tmp);
    free(ab);
    free(ac);
    free(ap);
    free(bp);
    free(cp);
    return res; // #5
  }

  double va = d3 * d6 - d5 * d4;
  if (va <= 0.f && (d4 - d3) >= 0.f && (d5 - d6) >= 0.f) {
    const float v = (d4 - d3) / ((d4 - d3) + (d5 - d6));
    vertex_static *cb = substraction_v(c, b);
    vertex_static *tmp = scalar_v(v, cb);
    vertex_static *res = sum_v(b, tmp);
    free(cb);
    free(tmp);
    free(ab);
    free(ac);
    free(ap);
    free(bp);
    free(cp);
    return res; // #6
  }

  double denom = 1.f / (va + vb + vc);
  double v = vb * denom;
  double w = vc * denom;
  vertex_static *tmp1 = scalar_v(v, ab);
  vertex_static *tmp2 = scalar_v(w, ac);
  vertex_static *tmp3 = sum_v(tmp1, tmp2);
  vertex_static *res = sum_v(a, tmp3);
  free(ab);
  free(ac);
  free(ap);
  free(bp);
  free(cp);
  free(tmp1);
  free(tmp2);
  free(tmp3);
  return res; // #0
}


//barycentric coordinates of a point in a triangle
void barycentric_calc(vertex_static *p, vertex_static *a, vertex_static *b, vertex_static *c, int *k_a, int *k_b, int *k_c){
  //p = k_a*a + k_b*b + k_c*c
  //the function modifies k_a, k_b and k_c
  
  MATRIX_TYPE tab_abc[3][3] = {a->x, a->y, a->z, 
                                  b->x, b->y, b->z,
                                  c->x, c->y, c->z};
  Matrix *mat_abc = Matrix_gen(3, 3, tab_abc);
  free(tab_abc);

  Matrix *mat_abc_inverse = M_inverse(mat_abc);
  M_free(mat_abc);

  MATRIX_TYPE tab_p[3] = {p->x, p->y, p->z};
  Matrix *mat_p = Matrix_gen(3, 1, tab_res);
  free(tab_res);

  Matrix *res = M_mul(mat_abc_inverse, mat_p);
  M_free(mat_abc_inverse);
  M_free(mat_p);

  *k_a = (res->data)[0];
  *k_b = (res->data)[1];
  *k_c = (res->data)[2];

  M_free(res);
}

double square_dist_v(vertex_static *v1, vertex_static *v2) {
  vertex_static *v1v2 = substraction_v(v2, v1);

  double dist = dot(v1v2, v1v2);

  free(v1v2);
  return dist;
}

double square_dist_v_f(vertex_static *v, face *f) {
  vertex_static *a = v_to_static(f->a);
  vertex_static *b = v_to_static(f->b);
  vertex_static *c = v_to_static(f->c);

  vertex_static *p = closest_point_face(v, a, b, c);

  double dist = square_dist_v(v, p);

  if (a != p && b != p && c != p) {
    free(p);
  }
  free(a);
  free(b);
  free(c);

  return dist;
}