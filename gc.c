#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

extern void **environ; //bottom of the stack

// Heap struct
// An opaque struct which holds all the necessary information we need. 

typedef struct heap{
  void* meta_p;       // pointer to heap's metadata (maybe unnecessary)
  void* user_start_p; // pointer to start of user's allocated space
  void* bump_p;       // pointer to next free spot in user's space
  size_t total_size;  // total size of the heap (with metadata)
  size_t user_size;   // size of user's allocated space (total_size minus metadata)
  float gc_threshold; // garbage collector threshold (1.0 = full memory)
} heap_t;


// mallocates space for heap, places metadata in the front. 

heap_t *h_init(size_t bytes, bool unsafe_stack, float gc_threshold) {
  void *new_heap = malloc(bytes);

  // create metadata struct and place it in the front of the heap
  // the user sees this as a heap_t struct and is therefore not
  // aware that it is the pointer to the whole heap. 
  ((heap_t*) new_heap)->meta_p = new_heap;
  ((heap_t*) new_heap)->user_start_p = new_heap + sizeof(heap_t);
  ((heap_t*) new_heap)->bump_p = new_heap + sizeof(heap_t);
  ((heap_t*) new_heap)->total_size = bytes;
  ((heap_t*) new_heap)->user_size = bytes - sizeof(heap_t);
  ((heap_t*) new_heap)->gc_threshold = gc_threshold;;
  
  return new_heap;
}

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
  for (NULL; t < b; t++){
    //trying to find our test var
    if(*t % 1337 == 0 && *t != 0) {
      printf("\n%p: %d ------TEST VALUE\n", t, *t);
    } else {
      printf("\n%p: %d\n", t, *t);
    }
  }

  puts("\n-------------------------------\n");

}
