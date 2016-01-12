#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "heap.h"
#include "traverser.h"
#include "stack_traverser.h"
#include "gc.h"


int string_length(char *string){
  int length = 0;
  for (int i = 0; string[i] != '\0'; i++) {
    length++;
  }
  return length;
}


int **addNumbers() {
  static int *array[100];

  for (int i = 0; i < 100; i++) {
    *array[i] = i;
  }

  return array;
}

int main() {
  heap_t *h = h_init(5000, true, 200.0);
  int *to_add = 0;
  
  for (int i = 0; i < 1000; i++) {
    to_add = h_alloc_data(h, sizeof(int));
    *to_add++;
  }
  
}





