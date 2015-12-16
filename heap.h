#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "linked_list.h"

#ifndef __heap__
#define __heap__

#define PAGE_SIZE 2048

// Heap struct
// An opaque struct which holds all the necessary information we need. 

typedef struct heap{
  void* meta_p;       // pointer to heap's metadata (maybe unnecessary)
  void* user_start_p; // pointer to start of user's allocated space
  void* bump_p;       // pointer to next free spot in user's space
  void* end_p;        // pointer to end of allocated space
  size_t total_size;  // total size of the heap (with metadata)
  size_t user_size;   // size of user's allocated space (total_size minus metadata)
  size_t avail_space; // amount of allocatable space left.
  bool unsafe_stack;  // wether or not unsafe stack
  float gc_threshold; // garbage collector threshold (1.0 = full memory)
} heap_t;


// mallocates space for heap, places metadata in the front. 
heap_t *h_init(size_t bytes, bool unsafe_stack, float gc_threshold);

void h_delete(heap_t* h);

// Allocates a certain amount of space on the heap. With metadata, though format string is blank.
// This allocated space may NOT be used for pointers
void *h_alloc_data(heap_t *h, size_t bytes);

// Allocates space on the heap based on the format string included. This is allocated with metadata.
void* h_alloc_struct(heap_t *h, char *format_string);

/************************************/
/*                                  */
/*  Metadata wrapper                */
/*                                  */
/************************************/

// @param object A pointer to the allocated object
// @return Whether or not the pointer points to a valid object.
bool validate_object(void* object);

// @param object A pointer to the allocated object
// @return A pointer to the format string
char* md_get_format_string(void* object);
void md_set_format_string(void* object, char* format_string);

// @param object A pointer to the allocated object
// @return Bitvector
char md_get_bit_vector(void* object);
void md_set_bit_vector(void* object, char bit_vector);

// @param object A pointer to the allocated object
// @return The object's forwarding address
void* md_get_forwarding_address(void* object);
void md_set_forwarding_address(void* object, void* forward_address);

// @param object A pointer to the allocated object
// @return The object's copied flag
bool md_get_copied_flag(void* objec);
void md_set_copied_flag(void* object, bool copied_flag);

/************************************/
/*                                  */
/*  Format string                   */
/*                                  */
/************************************/

// Uses the format string to find all pointers in an object and puts
// them in a list.
// @param object A pointer to the allocated object
// @return A list with all pointers within an object. 
ll_head fs_get_pointers_within_object(void* object);

size_t fs_get_object_size(void* object);

#endif
