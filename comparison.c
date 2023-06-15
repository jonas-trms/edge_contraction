#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "comparison.h"

// comparison based on vertices ids
int compare_e_ids(edge *e1, edge *e2) {
  if (((e1->a)->id == (e2->a)->id) && ((e1->b)->id == (e2->b)->id)) {
    return 0;
  }

  else {
    return 1;
  }
}

int compare_f_ids(face *f1, face *f2) {
  if (((f1->a)->id == (f2->a)->id) && ((f1->b)->id == (f2->b)->id) &&
      ((f1->c)->id == (f2->c)->id)) {
    return 0;
  }

  else {
    return 1;
  }
}