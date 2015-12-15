/* 
- kolla igenom alla stackpekare
- kolla metadatan för varje objekt som pekas på
- om metadata finns så ska pekaren skickas vidare till heaptraverseringen
- sålla bort pekare som inte pekar på något
- returnera alla giltliga pekare till heaptraverseringen i form av en länkad lista.
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <setjmp.h>
#include "linked_list.h"
#include "heap.h"

#define Dump_registers()			\
  jmp_buf env;					\
  if (setjmp(env)) abort();			\

extern char **environ; // bottom of the stack
ll_node **LL_initRoot();
ll_node* LL_createNode(void *content);
void LL_insertSequentially(ll_node** root, ll_node* toInsert);

void buildStackList() {
  void *top = __builtin_frame_address(1); // top of the stack
  ll_node **root = LL_initRoot();
  for (top; top < environ; top + sizeof(void *)) {
    ll_node *stackTop = LL_createNode(top);
    LL_insertSequentially(root, stackTop);
  }

  
  
}

ll_node *traverseStackList() {
  return NULL;  
}

void printStackList() {

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

  int n = 12345678;
  n = 1337;

  printf("Top: %p\nBottom: %p\n", top, environ);
  
  int stacksize;
  int bottomBigger = 0;
  
  if (environ > top) {
    bottomBigger = 1;
  }

  printf("Stacksize: %d", stacksize);

  // stack dump
  puts("\nStack dump: \n");
  
  while (top < environ) {
    printf("\n%p\n", top);
    top = top + sizeof(void *);
  }
  puts("\n----------------------------------------\n");  
}

int main() {
  int n = 3;
  void *top = &n;
  Dump_registers();
  buildStackList();
  printStackList();
  return 0;
}
