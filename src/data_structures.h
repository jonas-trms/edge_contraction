#ifndef DATA_STRUCTURES
#define DATA_STRUCTURES

/* // original mesh
struct vertex_static {
  double x;
  double y;
  double z;
}; 

typedef struct vertex_static vertex_static;*/

//barycenters
struct barycenter_node {
  int a;
  int b;
  int c;
  double k_a;
  double k_b;
  double k_c;
};

typedef struct barycenter_node barycenter_node;

// intern mesh
struct vertex;
struct edge;
struct face;

struct vertex_node {
  struct vertex *v;
  struct vertex_node *next;
};

struct edge_node {
  struct edge *e;
  struct edge_node *next;
};

struct face_node {
  struct face *f;
  struct face_node *next;
};

typedef struct vertex_node vertex_node;
typedef struct edge_node edge_node;
typedef struct face_node face_node;

struct vertex_chain {
  vertex_node *start;
};

struct edge_chain {
  edge_node *start;
};

struct face_chain {
  face_node *start;
};

typedef struct vertex_chain vertex_chain;
typedef struct edge_chain edge_chain;
typedef struct face_chain face_chain;

struct vertex {
  // ind represents index in global tab
  int id;
  int ind;
  double x;
  double y;
  double z;
  edge_chain *edges;
};

struct edge {
  // ind represents index in global tab
  // invariant : a<=b for compare_v
  int id;
  int ind;
  struct vertex *a;
  struct vertex *b;
  face_chain *faces;
};

struct face {
  // ind represents index in global tab
  // invariant : a<=b<=c for compare_v
  int id;
  int ind;
  struct vertex *a;
  struct vertex *b;
  struct vertex *c;
  edge_chain *edges;
};

typedef struct vertex vertex;
typedef struct edge edge;
typedef struct face face;

#endif