/// \file

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "linked_list.h"

#ifndef __heap__
#define __heap__

#define NUMBER_OF_PAGES 1
#define PAGE_SIZE 2048

// TODO: THIS IS TEMPORARY! If you are accessing the heap through this struct then you need to use a get-function (which might need creating)
struct heap{
  void* meta_p;       // pointer to heap's metadata (maybe unnecessary)
  void* user_start_p; // pointer to start of user's allocated space
  void* active_page_list;
  void* passive_page_list;
  void* end_p;        // pointer to end of allocated space
  size_t total_size;  // total size of the heap (with metadata)
  size_t user_size;   // size of user's allocated space (total_size minus metadata)
  size_t used_space;  // amount of bytes allocated.
  size_t avail_space; // amount of allocatable space left.
  bool unsafe_stack;  // whether or not unsafe stack
  float gc_threshold; // garbage collector threshold (1.0 = full memory)
};

// TODO: THIS IS TEMPORARY! This is here only for gui.c, nothing outside of heap.c should use this. 
typedef struct page{
  void* user_start_p; // pointer to start of user's allocated space
  void* bump_p;       // pointer to next free spot in user's space
  void* end_p;        // pointer to end of allocated space
  bool active;        // boolean keeping track if the page is active (true) or passive (false)
  bool unsure;        // boolean keeping track of whether a page is unsure or not 
  struct page* next_page;    // a pointer to the next page
} page_t;

/**
   @brief Heap struct type used to manipulate the heap. 
*/
typedef struct heap heap_t;

/**
   h_init
   @brief Initializes the heap. 
   @param bytes Size of the heap, preferrably a multiple of PAGE_SIZE. 
   @param unsafe_stack A bool indicating whether or not there is an unsafe stack
   @param gc_threshold A value between 0 and 1 indicating the threshold for when garbage collection is triggered. Should be close to, yet lower than 0.5.
   @return A pointer to the heap.

   The heap will use some of the allocated space to store metadata.
 */
heap_t *h_init(size_t bytes, bool unsafe_stack, float gc_threshold);

/**
   h_delete
   @brief Deletes the heap and frees the memory. 
   @param h The pointer to the heap.
 */
void h_delete(heap_t* h);

/**
   h_alloc_data
   @brief Allocates space in the heap. This allocation can NOT be used to store pointers as it will not follow any pointers in this allocation when marking garbage.
   @param h The pointer to the heap.
   @param bytes The amount of bytes to allocate, cannot be larger than PAGE_SIZE.

   This will allocate data with an empty format string. It will be ignored when marking garbage. 
 */
void *h_alloc_data(heap_t *h, size_t bytes);

/**
   h_alloc_struct
   @brief Allocates space in the heap. Allocates the appropriate amount of space based on the format string. 
   @param h The pointer to the heap.
   @param format_string A string which describes the scruct which will be stored in the allocation. 
   @return The pointer to the allocated space. 
 */
void* h_alloc_struct(heap_t *h, char *format_string);

/**
   h_gc
   @brief Manually trigger garbage collection.
   @param h The pointer to the heap.
   @return The number of bytes collected.
 */
size_t h_gc(heap_t* h);

/**
   h_avail
   @brief Returns the available free memory.
   @param h The pointer to the heap.
   @return The available free memory.
*/
size_t h_avail(heap_t *h);

/**
   h_used
   @brief Returns the amount of used memory. Does not count metadata.
   @param h The pointer to the heap.
   @return The amount of used memory.
*/
size_t h_used(heap_t *h);

// TODO: Rename to md_get_heap_start
/**
   get_heap_start
   @brief Returns the pointer to the start of the user-allocation area of the heap.
   @param h The pointer to the heap.
   @return The pointer to the start of the allocation space of the heap.
 */
void* get_heap_start(heap_t *h);

// TODO: Rename to md_get_heap_end
/**
   get_heap_end
   @brief Returns the pointer to the end of the heap.
   @param h The pointer to the heap.
   @return The pointer to the end of the heap.
 */
void* get_heap_end(heap_t *h);

/************************************/
/*                                  */
/*  Metadata wrapper                */
/*                                  */
/************************************/

/**
   @brief Checks if an object is a valid allocated object.  
   @param object A pointer to the allocated object.
   @return Whether or not the pointer points to a valid object.
   
   Currently does not work. 
*/
bool validate_object(void* object);

/**
   @brief Returns a pointer to the format string of the object.
   @param object A pointer to the allocated object.
   @return A pointer to the object's format string.
*/
char* md_get_format_string(void* object);
void md_set_format_string(void* object, char* format_string);

/**
   @brief Returns the bit vector of the object.
   @param object A pointer to the allocated object.
   @return The object's bit vector.
*/
char md_get_bit_vector(void* object);
void md_set_bit_vector(void* object, char bit_vector);

/**
   @brief Returns the forwarding address of the object.
   @param object A pointer to the allocated object.
   @return The object's forwarding address.
*/
void* md_get_forwarding_address(void* object);
void md_set_forwarding_address(void* object, void* forward_address);

/**
   @brief Returns the copied flag.
   @param object A pointer to the allocated object
   @return The object's copied flag
*/
bool md_get_copied_flag(void* objec);
void md_set_copied_flag(void* object, bool copied_flag);

/************************************/
/*                                  */
/*  Format string                   */
/*                                  */
/************************************/

/**
   @brief Returns a list of all the pointers in an object.
   @param object A pointer to the allocated object.
   @return A list with all pointers within an object. 

   Uses the format string to find all pointers in an object and puts
   them in a list.
*/
ll_head fs_get_pointers_within_object(void* object);

/**
   @brief Gets the size of the object.
   @param object A pointer to the object.
   @return The size of the object. 
 */
size_t fs_get_object_size(void* object);

#endif
