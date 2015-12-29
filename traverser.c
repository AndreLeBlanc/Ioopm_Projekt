#include <stdio.h>
#include <stdlib.h>
#include "heap.h"
#include "traverser.h"
#include "linked_list.h"

int main() {

  heap_t *heap = h_init(2048, 1, 1.0);
  h_alloc_struct(heap, "i");
  void *pointer = h_alloc_data(heap, 16);
  void *pointer2 = h_alloc_struct(heap, "***iii");
  print_traversed_heap(heap);

}

ll_head traverse_pointers_from_LL(ll_head pointers) {

    if(!LL_isEmpty(pointers)) {

        //loop through all the pointers
        ll_node* cursor = *pointers;
        while(cursor != NULL) {
            //grab pointers within object.
            ll_head child_pointers = fs_get_pointers_within_object(cursor);
            if(child_pointers != NULL) {
                LL_insertSequentially(pointers, traverse_pointers_from_LL(child_pointers)); //add to list and recursive call
            } else {
                continue;
            }
            cursor = LL_getNext(cursor);
        }

        return pointers;

    } else {
        return NULL;
    }

}

void print_traversed_heap(heap_t *heap) {
  int count = 1;
  void *user_start_p = heap->user_start_p;
  while(user_start_p < heap->bump_p) {
      printf("%d:: %p\n", count, user_start_p);
      user_start_p += sizeof(void *);
      count++;
  }
}
