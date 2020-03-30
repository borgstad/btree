#include <stdio.h>
#include "btree.h"


int main() {
  
  Btree *a = btreeCreate(10);
  printf("%i\n", a);
  return(0);
}
