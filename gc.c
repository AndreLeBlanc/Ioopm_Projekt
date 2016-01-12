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
    ll_head stack_addr = get_alive_stack_address_pointers(h); //same as above but stack address
    ll_head heap_pts = traverse_pointers_from_LL(stack_pts);

    if(heap_pts != NULL) {
      // puts("addr");
      // LL_map(stack_addr, printAddress);
      // puts("stack_pts");
      // LL_map(stack_pts, printAddress);
      // puts("heap");
      // LL_map(heap_pts, printAddress);

      void *heap_cursor = *heap_pts;
      void *stack_cursor = *stack_addr;

      while(heap_cursor != NULL) {
        void ***stack_content = (void ***)LL_getContent(stack_cursor);
        // printf("\n\nheap: %p --> %p\n", heap_cursor, LL_getContent(heap_cursor));
        // printf("\nstack: %p --> %p\n", stack_cursor, LL_getContent(stack_cursor));
      	void* curr_obj = LL_getContent(heap_cursor);
      	num_bytes_collected += fs_get_object_size(curr_obj);
        //change stack_pointer
        // printf("before: %p\n", *stack_content);
      	*stack_content = h_alloc_compact(h, curr_obj);
        // printf("after: %p\n", *stack_content);
        heap_cursor = LL_getNext(heap_cursor);
        stack_cursor = LL_getNext(stack_cursor);
      }

    } else {
      return 0;
    }

    // ll_head stack_pts_after = get_alive_stack_pointers(h);
    // ll_head stack_addr_after = get_alive_stack_address_pointers(h); //same as above but stack address
    // ll_head heap_pts_after = traverse_pointers_from_LL(stack_pts_after);
    // puts("addr_after");
    // LL_map(stack_addr_after, printAddress);
    // puts("stack_pts_after");
    // LL_map(stack_pts_after, printAddress);
    // puts("heap_after");
    // LL_map(heap_pts_after, printAddress);

    post_compact_page_reset(h);
    return num_bytes_collected;
 }
