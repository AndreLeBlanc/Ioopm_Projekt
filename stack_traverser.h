#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <setjmp.h>
#include "heap.h"

#ifndef __stack_traverser__
#define __stack_traverser__

void fuckOff();
void endiannessTestAux();

void endiannessTest();
bool heap_grows_upwards();
bool is_pointing_at_heap(void *ptr, heap_t *h);
void *get_stack_top();
bool stack_grows_from_top();
void create_stack_list(int flag, void *top, heap_t *h, ll_node **root);
ll_node **traverse_stack_list(heap_t *h);
void print_stack_list(ll_node **root);

#endif
