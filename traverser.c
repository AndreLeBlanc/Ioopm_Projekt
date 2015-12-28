#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "traverser.h"

// int main() {
//
//   heap_t *heap = h_init(2048, 1, 1.0);
//   h_alloc_struct(heap, "i");
//   // h_alloc_data(heap, 16);
//   print_heap(heap);
//
// }

void print_traversed_heap(heap_t *heap) {
  int count = 1;
  void *user_start_p = heap->user_start_p;
  while(user_start_p < heap->bump_p) {
      printf("%d:: %p\n", count, user_start_p);
      user_start_p += sizeof(void *);
      count++;
  }
}
