#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <setjmp.h>
#include "linked_list.h"
#include "heap.h"


#ifndef __stack_traverser__
#define __stack_traverser__

void print_stack_list(ll_node **root);

/**
   @brief Checks if the void pointer ptr is pointing at the specified part of the heap h.
   @param ptr the pointer to be checked.
   @param  the part of the heap to be checked.
   @returns  true if the pointer ptr is pointing somewhere in h.
*/
bool is_pointing_at_heap(void *ptr, heap_t *h);

/**
   @brief Creates A stack top and returns it. 
   @return The new top of the stack. 
*/
void *get_stack_top();

/**
   @brief Traverses the stack and creates a list of the pointers that points to the heap h.
   @param  h is the heap to be checked.
   @return A list containing all the pointers that point to the heap h.
*/ 
ll_node **get_alive_stack_pointers(heap_t *h);

#endif
