#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <setjmp.h>
#include "heap.h"

#define Dump_registers()			\
  jmp_buf env;					\
  if (setjmp(env)) {abort();}			\

extern char **environ; //bottom of the stack

size_t h_gc(heap_t *h) {
    if (h.gc_threshold >0.95) {
      void* bottom = (void*) environ;
      ll_head stack_p =  get_alive_stack_pointers(h, get_stack_top(), bottom);
      ll_head heap_p = traverse_pointers_from_LL(stack_p);
      LL_map(heap_p, h_alloc_compact(h, (void*))); //TODO this isn't correct
      void post_compact_page_reset(h);
    }   
 }
