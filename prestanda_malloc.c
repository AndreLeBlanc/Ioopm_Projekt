#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "heap.h"
#include "traverser.h"
#include "stack_traverser.h"
#include "gc.h"

int main() {
  int *to_add = 0;
  
  for (int i = 0; i < 1000; i++) {
    to_add = malloc(sizeof(int *));
    *to_add++;
  }
  
}
