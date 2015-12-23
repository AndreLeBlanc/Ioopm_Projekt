/* 
- kolla igenom alla stackpekare
- kolla metadatan för varje objekt som pekas på
- om metadata finns så ska pekaren skickas vidare till heaptraverseringen
- sålla bort pekare som inte pekar på något
- returnera alla giltliga pekare till heaptraverseringen i form av en länkad lista.
*/

/*
  Kompilera med:
  gcc -std=c11 -Wall -ggdb -o stack_traverser stack_traverser.c

  tills vidare.
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <setjmp.h>
#include "linked_list.c"
#include "heap.h"

#define Dump_registers()			\
  jmp_buf env;					\
  if (setjmp(env)) abort();			\

extern char **environ; // bottom of the stack

void fuckOff(){}
void endiannessTestAux(){}

void endiannessTest() { // this function investigates whether the stack grows upwards or downwards.
  int stack;
  void *heap = malloc(1);
  void *heap2 = malloc(1);
  Dump_registers();
  puts("");
  printf("printf:             %15p\n", printf);
  printf("puts:               %15p\n", puts);
  printf("fuckOff:            %15p\n", fuckOff);
  printf("endiannessTestAux:  %15p\n", endiannessTestAux);
  printf("endiannessTest:     %15p\n", endiannessTest);
  printf("heap:     %15p\n", heap);
  printf("heap2:    %15p\n", heap2);
  printf("stack:    %15p\n", &stack);
  int stack2;
  printf("stack2:   %15p\n", &stack2);
}

bool is_pointing_at_heap(void *ptr) {
  if (ptr > heap_t->user_start_p && ptr < heap_t->end_p)

}

bool heap_grows_upwards() {
  void *first = malloc(1);
  void *second = malloc(1);
  if (first < second) {
    free(first);
    free(second);
    return true;
  }
  else {
    free(first);
    free(second);
    return false;
  }  
}

void *get_stack_top() {
  int top_of_stack;
  return &top_of_stack;
}

bool stack_grows_upwards() {
  int stack;
  int stack2;

  if (&stack < &stack2)
    return true;
  else
    return false;
}

ll_node **traverse_stack_list() {
  void *top = get_stack_top();
  ll_node **root = LL_initRoot();
  int counter = 0;
  if (stack_grows_upwards()) {
    puts("Stack grows upwards.");
    while (top < environ) {
      printf("top: %04x\n", top);
      if (validate_object(top)) { // checks the pointers metadata to check whether it's valid or not
	ll_node *stackTop = LL_createAndInsertSequentially(root, top);
	//printf("stackTop: %p\n", stackTop);,
	printf("Count %d: New stackpointer %04x has valid metadata and was added to the list.\n", counter, stackTop->nodeContent);
      }
      top += sizeof(void *);
      counter++;
    }
  }
  else {
    while (top > environ) {
      puts("Stack grows downwards.");
      printf("top: %04x\n", top);
      if (validate_object(top)) { // checks the pointers metadata to check whether it's valid or not
	ll_node *stackTop = LL_createAndInsertSequentially(root, top);
	//printf("stackTop: %p\n", stackTop);,
	printf("Count %d: New stackpointer %04x has valid metadata and was added to the list.\n", counter, stackTop->nodeContent);
      }
      top -= sizeof(void *);
      counter++;
    }
  }
  return root;
}

void print_stack_list(ll_node **root) {
  puts("Printing stacklist");
  ll_node *iterator = *root;
  int counter = 0;

  while (iterator) {
    printf("\nCounter: %d\n%04x\n", counter, iterator->nodeContent);
    counter++;
    if (iterator->previous) {
      printf("%04x has previous pointer at %04x\n", iterator->nodeContent, iterator->previous->nodeContent);
      printf("Difference between this %04x and %04x is %04x\n", iterator->nodeContent, iterator->previous->nodeContent, (iterator->nodeContent - iterator->previous->nodeContent));
    }
    if (iterator->next)
      printf("%04x has next pointer at %04x\n", iterator->nodeContent, iterator->next->nodeContent);
    iterator = iterator->next;
    }
}

void print_stack() { // Not used. Was here just for testing and getting started in the beginning. Gonna be kept for testing purposes
  const int N = 10;
  int arr[N];
  int i = 0;
  while (i < N) {
    arr[i] = i * 100 + i * 10 + i;
    printf("arr[%d]: %d\n", i, arr[i]);
    i++;
  }

  puts("\n----------------------------------------\n");
  void *top = __builtin_frame_address(1); // top of the stack

  printf("Top of the stack: %p\nBottom of the stack %p\n", top, environ);
  
  int bottomBigger = 0;
  
  if (environ > top) {
    bottomBigger = 1;
  }
  
  while (top < environ) {
    printf("\n%p\n", top);
    top = top + sizeof(void *);
  }
  puts("\n----------------------------------------\n");  
}

int main() {
  Dump_registers();
  ll_node **root = traverse_stack_list();
  //print_stack_list(root);
  endiannessTest();
  return 0;
}
