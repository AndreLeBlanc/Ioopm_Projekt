#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <setjmp.h>
#include "heap.h"
#include "stack_traverser.h"
#include "traverser.h"
#include "linked_list.h"

size_t h_gc(heap_t *h) {
    size_t num_bytes_collected = 0;
    ll_head stack_pts = get_alive_stack_pointers(h);
    ll_head heap_pts = traverse_pointers_from_LL(stack_pts);

    if(heap_pts != NULL) {
      void *heap_cursor = *heap_pts;
      void *stack_cursor = *stack_pts;
      while(heap_cursor != NULL) {
        printf("\n\n%p --> %p\n", heap_cursor, LL_getContent(heap_cursor));
	void* curr_obj = LL_getContent(heap_cursor);
	num_bytes_collected += fs_get_object_size(curr_obj);
	h_alloc_compact(h, curr_obj);
        heap_cursor = LL_getNext(heap_cursor);
      }
    } else {
      return 0;
    }
    // LL_map(heap_p, h_alloc_compact_map); //TODO this isn't correct

    post_compact_page_reset(h);
    return num_bytes_collected;
 }
