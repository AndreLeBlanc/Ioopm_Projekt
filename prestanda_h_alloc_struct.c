#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "heap.h"
#include "traverser.h"
#include "stack_traverser.h"
#include "gc.h"

typedef struct to_add {
  int number1;
  int number2;
}to_add_;

void set_struct_number_1(to_add_ *h, int new) {
  h->number1 = new;
}

void set_struct_number_2(to_add_ *h, int new) {
  h->number2 = new;
}

int main() {
  heap_t *h = h_init(5000, true, 200.0);

  
  
  for (int i = 0; i < 200; i++) {
    to_add_ *test_struct = h_alloc_struct(h, "*ii");
    set_struct_number_1(test_struct, i);
    set_struct_number_2(test_struct, i+1);
    i = i + 2;
  }

  return 0; 
}
