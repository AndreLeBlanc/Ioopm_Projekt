#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#ifndef __heap__
#define __heap__

#define PAGE_SIZE 2048

// Heap struct
// An opaque struct which holds all the necessary information we need. 

typedef struct heap{
  void* meta_p;       // pointer to heap's metadata (maybe unnecessary)
  void* user_start_p; // pointer to start of user's allocated space
  void* bump_p;       // pointer to next free spot in user's space
  void* end_p;       // pointer to end of allocated space
  size_t total_size;  // total size of the heap (with metadata)
  size_t user_size;   // size of user's allocated space (total_size minus metadata)
  bool unsafe_stack;  // wether or not unsafe stack
  float gc_threshold; // garbage collector threshold (1.0 = full memory)
} heap_t;


// mallocates space for heap, places metadata in the front. 

heap_t *h_init(size_t bytes, bool unsafe_stack, float gc_threshold);

void h_delete(heap_t* h);

#endif
