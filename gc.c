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

void **environ; //bottom of the stack
  
void print_stack() {

  Dump_registers();

  puts("\n-------------------------------\n");
  void *top;
  int el_top = 123; 
  top = & el_top;

  printf("Top of stack: %p\nBottom of stack: %p\n", top, *environ);

  int *t = (int *)top;
  int *b = (int *)environ;

  /*
    Så för att traversera stacken är tanken att man tar sista variablens adress och använder
    den som topp i stacken. Eftersom allting som pushas till stacken läggs högst upp så
    måste den här pekaren uppdateras för varje stack-variabel som läggs till.
  */
  printf("Content of top: %d\nContent of bottom: %d\n", *t, *b);
  /*
    int stacksize;
    if(b > t) {
    stacksize = b-t;
    } else {
    stacksize = t-b;
    }

    printf("Stack size: %d", stacksize);
  */
  //stack dump 
  puts("\nStack dump: \n");
  if(t < b){
    while(t < b){ 
      int *hola = (int *)top;
      printf("\n%p: %d\n", top, *hola);
      top += sizeof(void *);
    }
  } else if(b < t) {
    while(b < t) {
      int *hola2 = (int *)top;
      printf("\n%p: %d\n", top, *hola2);
      top -= sizeof(void *);
    }
  }
  puts("\n-------------------------------\n");
}
