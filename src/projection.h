#ifndef PROJECTION
#define PROJECTION

#include "access_operations.h"

vertex_static *sum_v(vertex_static *v1, vertex_static *v2);
vertex_static *substraction_v(vertex_static *v1, vertex_static *v2);
vertex_static *scalar_v(double k, vertex_static *v1);
double dot(vertex_static *v1, vertex_static *v2);
vertex_static *closest_point_face(vertex_static *p, vertex_static *a,
                                  vertex_static *b, vertex_static *c);
vertex_static *v_to_static(vertex *v);
double square_dist_v(vertex_static *v1, vertex_static *v2);
double square_dist_v_f(vertex_static *v, face *f);

#endif