#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include "heap.h"

extern void **environ; //bottom of the stack
  
void print_stack() {

  const int N = 10;
  int arr[N];
  for(int i = 0; i < N; i++) {
    arr[i] = i * 100 + i * 10 + i;
  }

  //  int *top = &arr[0];


  puts("\n-------------------------------\n");
   void *top = __builtin_frame_address(1); //top of stack
  // *t = __builtin_frame_address(1); //top of stack (does not work in mac os x)

  printf("Top of stack: %p\nBottom of stack: %p\n", top, environ);

  int *t = (int *)top;
  int *b = (int *)environ;

  int n = 0;
  n = 1337;

  // t = &n;
  /*
  Så för att traversera stacken är tanken att man tar sista variablens adress och använder
    den som topp i stacken. Eftersom allting som pushas till stacken läggs högst upp så
    måste den här pekaren uppdateras för varje stack-variabel som läggs till.
  */
  printf("Top: %d\nBottom: %d\n", *t, *b);

  int stacksize;
  int bottomBigger = 0;
  if(b > t) {
    stacksize = b-t;
    bottomBigger = 1;
  } else {
    stacksize = t-b;
  }

  printf("Stack size: %d", stacksize);

  //stack dump
  puts("\nStack dump: \n");
  while(t < b){
    //trying to find our test var
    if(*t % 1337 == 0 && *t != 0) {
      printf("\n%p: %d ------TEST VALUE\n", t, *t);
    } else {
      printf("\n%p: %d\n", t, *t);
    }
    t++;
  }

  puts("\n-------------------------------\n");

}
