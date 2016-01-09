#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <setjmp.h>
#include "linked_list.h"
#include "heap.h"


#ifndef __stack_traverser__
#define __stack_traverser__

void fuckOff();
void endiannessTestAux();
void endiannessTest();
void print_stack_list(ll_node **root);

/// Checks if the heap grows upwards.
///
/// \returns true if the heap grows upwards
bool heap_grows_upwards();

/// Checks if the void pointer ptr is pointing at the specified part of the heap h.
///
/// \param ptr the pointer to be checked
/// \param the part of the heap to be checked
/// \returns true if the pointer ptr is pointing somewhere in h
bool is_pointing_at_heap(void *ptr, heap_t *h);

/// Creates a stack top and returns it.
///
/// \returns the new stack top
void *get_stack_top();

/// Checks if the stack grows upwards or downwards.
///
/// \returns true if the stack grows upwards
bool stack_grows_from_top();

/// Creates a list of the pointers that points to the heap h.
///
/// \param stack_grows_upwards is a boolean and tells if the stack grows upwards
/// \param top is the top of the stack
/// \param h is the heap to be checked
/// \param root is the root of the list
void create_stack_list(void *top, heap_t *h, ll_node **root);

/// Calls other functions to traverse the stack and create the stack list.
///
/// \param h is the heap to be checked
/// \returns ll_node, the first node of the list containing all the pointers that point to the heap h
ll_node **get_alive_stack_pointers(heap_t *h);

#endif
