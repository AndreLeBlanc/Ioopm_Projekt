#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <setjmp.h>
#include "linked_list.h"
#include "heap.h"


#ifndef __stack_traverser__
#define __stack_traverser__

/// Checks if the heap grows upwards.
///
/// \returns true if the heap grows upwards
bool heap_grows_upwards();

/// Creates a stack top and returns it.
///
/// \returns the new stack top
void *get_stack_top();

/// Traverses the stack and creates a list of the pointers that points to the heap h.
///
/// \param h is the heap to be checked
/// \returns ll_node, the first node of the list containing all the pointers that point to the heap h
ll_head get_alive_stack_pointers(heap_t *h, void *top, void *bottom);

#endif
