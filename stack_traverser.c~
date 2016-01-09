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
#include "linked_list.h"
#include "heap.h"

extern void *environ; // bottom of the stack

void print_stack() {

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

  int *t = (int *)top;
  int *b = (int *)environ;

  int n = 0;
  n = 1337;

  printf("Top: %d\nBottom: %d\n", *t, *b);
  
  int stacksize;
  int bottomBigger = 0;
  
  if (b > t) {
    stacksize = b-t;
    bottomBigger = 1;
  }
  else {
    stacksize = t-b;
  }

  printf("Stacksize: %d", stacksize);

  // stack dump
  puts("\nStack dump: \n");

  while (t < b) {
    if (*t % n == 0 && *t != 0) {
      printf("\n%p: %d -------TEST VALUE\n", t, *t);
    }
    else {
      printf("\n%p: %d\n", t, *t);
    }
    t++;
  }
  puts("\n----------------------------------------\n");  
}

int main() {
  print_stack();
  return 0;
}









