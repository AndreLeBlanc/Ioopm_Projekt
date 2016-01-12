#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <setjmp.h>
#include "heap.h"
#include "stack_traverser.h"
#include "traverser.h"

extern char **environ; //bottom of the stack

size_t h_gc(heap_t *h) {
  if (h->gc_threshold > 0.95) {
    void* bottom = (void *) environ;
    ll_head stack_pts =  get_alive_stack_pointers(h, get_stack_top(), bottom);
    ll_head heap_pts = traverse_pointers_from_LL(stack_pts);
    // LL_map(heap_p, h_alloc_compact_map); //TODO this isn't correct
    // post_compact_page_reset(h);
    puts("[STACK_PTS]");
    LL_map(stack_pts, printAddress);
    puts("[HEAP_PTS]");
    LL_map(heap_pts, printAddress);
  }
 }
