#ifndef DISTANCE_OPERATIONS
#define DISTANCE_OPERATIONS

#include "access_operations.h"

double *closest_point_face(double *p, double *a, double *b, double *c);

void barycentric_calc(double *p, double *a, double *b, double *c, double *k_a, double *k_b, double *k_c);

double square_dist_v(double *v1, double *v2);

double square_dist_v_f(double *v, face *f, double *p_copy);

#endif