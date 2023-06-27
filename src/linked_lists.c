#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "linked_lists.h"

int min(int a, int b) {
  if (a <= b) {
    return a;
  } else {
    return b;
  }
}

// linked lists
// creation
vertex_chain *new_vchain(void) {
  vertex_chain *new = malloc(sizeof(vertex_chain));
  new->start = NULL;

  return new;
}

edge_chain *new_echain(void) {
  edge_chain *new = malloc(sizeof(edge_chain));
  new->start = NULL;

  return new;
}

face_chain *new_fchain(void) {
  face_chain *new = malloc(sizeof(face_chain));
  new->start = NULL;

  return new;
}

// freeing lists
void free_v_chain(vertex_chain *vchain) {
  vertex_node *curr = vchain->start;

  while (curr != NULL && curr->next != NULL) {
    vertex_node *tmp = curr;
    curr = curr->next;
    free(tmp);
  }

  if (curr != NULL) {
    free(curr);
  }

  free(vchain);
}

void free_e_chain(edge_chain *echain) {
  edge_node *curr = echain->start;

  while (curr != NULL && curr->next != NULL) {
    edge_node *tmp = curr;
    curr = curr->next;
    free(tmp);
  }

  if (curr != NULL) {
    free(curr);
  }

  free(echain);
}

void free_f_chain(face_chain *fchain) {
  face_node *curr = fchain->start;

  while (curr != NULL && curr->next != NULL) {
    face_node *tmp = curr;
    curr = curr->next;
    free(tmp);
  }

  if (curr != NULL) {
    free(curr);
  }

  free(fchain);
}

// member test
vertex_node *member_v(vertex *v, vertex_chain *vchain) {
  vertex_node *curr = vchain->start;

  if (curr == NULL) {
    return NULL;
  }

  int i = 0;

  if (v->id == (curr->v)->id) {
    i = 1;
  }

  vertex_node *curr_prev = curr;

  while (curr->next != NULL) {
    if (v->id == ((curr->next)->v)->id) {
      return curr;
    }

    else {
      curr_prev = curr;
      curr = curr->next;
    }
  }

  if (v->id == (curr->v)->id) {
    return curr_prev;
  }

  else if (i == 1) {
    return curr;
  }

  else {
    return NULL;
  }
}

edge_node *member_e(edge *e, edge_chain *echain) {
  edge_node *curr = echain->start;

  if (curr == NULL) {
    return NULL;
  }

  int i = 0;

  if (e->id == (curr->e)->id) {
    i = 1;
  }

  edge_node *curr_prev = curr;

  while (curr->next != NULL) {
    if (e->id == ((curr->next)->e)->id) {
      return curr;
    }

    else {
      curr_prev = curr;
      curr = curr->next;
    }
  }

  if (e->id == (curr->e)->id) {
    return curr_prev;
  }

  else if (i == 1) {
    return curr;
  }

  else {
    return NULL;
  }
}

face_node *member_f(face *f, face_chain *fchain) {
  face_node *curr = fchain->start;

  if (curr == NULL) {
    return NULL;
  }

  int i = 0;

  if (f->id == (curr->f)->id) {
    i = 1;
  }

  face_node *curr_prev = curr;

  while (curr->next != NULL) {
    if (f->id == ((curr->next)->f)->id) {
      return curr;
    }

    else {
      curr_prev = curr;
      curr = curr->next;
    }
  }

  if (f->id == (curr->f)->id) {
    return curr_prev;
  }

  else if (i == 1) {
    return curr;
  }

  else {
    return NULL;
  }
}

// insertion
int insert_v(vertex *v, vertex_chain *vchain) {
  vertex_node *curr = member_v(v, vchain);

  if (curr == NULL) {
    vertex_node *new = malloc(sizeof(vertex_node));
    new->v = v;
    new->next = vchain->start;
    vchain->start = new;

    return 1;
  }

  else {
    return 0;
  }
}

int insert_e(edge *e, edge_chain *echain) {
  edge_node *curr = member_e(e, echain);

  if (curr == NULL) {
    edge_node *new = malloc(sizeof(edge_node));
    new->e = e;
    new->next = echain->start;
    echain->start = new;

    return 1;
  }

  else {
    return 0;
  }
}

int insert_f(face *f, face_chain *fchain) {
  face_node *curr = member_f(f, fchain);

  if (curr == NULL) {
    face_node *new = malloc(sizeof(face_node));
    new->f = f;
    new->next = fchain->start;
    fchain->start = new;

    return 1;
  }

  else {
    return 0;
  }
}

// remove operations
int remove_v_chain(vertex *v, vertex_chain *vchain) {
  vertex_node *curr = member_v(v, vchain);

  if (curr == NULL) {
    return 0;
  }

  else if (curr->next == NULL) {
    vertex_node *tmp = (vchain->start)->next;
    free(vchain->start);
    vchain->start = tmp;

    return 1;
  }

  else {
    vertex_node *tmp = (curr->next);
    curr->next = tmp->next;
    free(tmp);

    return 1;
  }
}

int remove_e_chain(edge *e, edge_chain *echain) {
  edge_node *curr = member_e(e, echain);

  if (curr == NULL) {
    return 0;
  }

  else if (curr->next == NULL) {
    edge_node *tmp = (echain->start)->next;
    free(echain->start);
    echain->start = tmp;

    return 1;
  }

  else {
    edge_node *tmp = (curr->next);
    curr->next = tmp->next;
    free(tmp);

    return 2;
  }
}

int remove_f_chain(face *f, face_chain *fchain) {
  face_node *curr = member_f(f, fchain);

  if (curr == NULL) {
    return 0;
  }

  else if (curr->next == NULL) {
    face_node *tmp = (fchain->start)->next;
    free(fchain->start);
    fchain->start = tmp;

    return 1;
  }

  else {
    face_node *tmp = (curr->next);
    curr->next = tmp->next;
    free(tmp);

    return 1;
  }
}

// sort by id
int insert_v_id(vertex *v, vertex_chain *vchain) {
  vertex_node *new = malloc(sizeof(vertex_node));
  new->v = v;

  vertex_node *curr = vchain->start;

  if (curr == NULL) {
    new->next = NULL;
    vchain->start = new;
    return 1;
  }

  if (v->id == (curr->v)->id) {
    free(new);
    return 0;
  }

  else if ((v->id) < (curr->v)->id) {
    new->next = curr;
    vchain->start = new;
    return 1;
  }

  while (curr->next != NULL) {
    if (v->id == ((curr->next)->v)->id) {
      free(new);
      return 0;
    }

    else if ((v->id) < ((curr->next)->v)->id) {
      new->next = curr->next;
      curr->next = new;
      return 1;
    }

    else {
      curr = curr->next;
    }
  }

  if (v->id == (curr->v)->id) {
    free(new);
    return 0;
  }

  else if ((v->id) > (curr->v)->id) {
    new->next = NULL;
    curr->next = new;
    return 1;
  }

  return -1;
}

// intersection (chains already sorted by id)
vertex_chain *intersect_v_chains(vertex_chain *a, vertex_chain *b) {
  vertex_node *a_curr = a->start;
  vertex_node *b_curr = b->start;

  vertex_chain *res = new_vchain();

  while (a_curr != NULL && b_curr != NULL) {
    if ((a_curr->v)->id == (b_curr->v)->id) {
      insert_v(a_curr->v, res);
      a_curr = a_curr->next;
      b_curr = b_curr->next;
    }

    else if ((a_curr->v)->id > (b_curr->v)->id) {
      b_curr = b_curr->next;
    }

    else if ((a_curr->v)->id < (b_curr->v)->id) {
      a_curr = a_curr->next;
    }
  }

  return res;
}