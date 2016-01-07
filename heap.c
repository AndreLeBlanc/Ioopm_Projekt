#include "heap.h"
#include "linked_list.h"
#include <string.h>
#include <stdio.h>
#include <ctype.h>

// mallocates space for heap, places metadata in the front. 

heap_t *h_init(size_t bytes, bool unsafe_stack, float gc_threshold) {
  if(bytes < sizeof(heap_t) + sizeof(page_t)) {
    // if space allocated is not even enough for heap and page metadata, don't allocate
    return NULL;
  }

  heap_t *new_heap = malloc(bytes);

  // create metadata struct and place it in the front of the heap
  // the user sees this as a heap_t struct and is therefore not
  // aware that it is the pointer to the whole heap. 
  new_heap->meta_p            = new_heap;
  new_heap->user_start_p      = new_heap + sizeof(heap_t);
  new_heap->active_page_list  = NULL;
  new_heap->passive_page_list = new_heap + sizeof(heap_t);
  new_heap->bump_p            = new_heap + sizeof(heap_t);
  new_heap->end_p             = new_heap + bytes;
  new_heap->total_size        = bytes;
  new_heap->user_size         = bytes - sizeof(heap_t);
  new_heap->used_space        = 0;
  new_heap->avail_space       = bytes - sizeof(heap_t);
  new_heap->unsafe_stack      = unsafe_stack;
  new_heap->gc_threshold      = gc_threshold;

  // set up pages
  size_t bytes_left = bytes;
  page_t* cursor = new_heap->passive_page_list;
  
  while(bytes_left > sizeof(page_t)) {
    
    // set defaults for page
    cursor->user_start_p = cursor + sizeof(page_t);
    cursor->bump_p       = cursor + sizeof(page_t);
    cursor->end_p        = cursor + (bytes_left - PAGE_SIZE > 0 ? PAGE_SIZE : bytes_left) - 1;
    cursor->active       = false;
    cursor->unsure       = false;
    cursor->next_page    = (bytes_left - PAGE_SIZE > 0 ? cursor->end_p + 1 : NULL);

    // go to next page and update bytes_left as well as user_size and avail_space in heap
    cursor = cursor->next_page;
    bytes_left -= PAGE_SIZE;
    printf("1");
    new_heap->total_size -= sizeof(page_t);
    new_heap->avail_space -= sizeof(page_t);
    printf("2");
  }

  // return pointer to heap
  return new_heap;
}

void h_delete(heap_t *h) {
  if(h != NULL) {
    free(h);
  } 
}

size_t h_avail(heap_t *h) {
  return h->avail_space;
}

size_t h_used(heap_t *h) {
  return h->used_space;
}

void* get_heap_start(heap_t* h) {
  return h->user_start_p;
}

void* get_heap_end(heap_t* h) {
  return h->end_p;
}

/************************************/
/*                                  */
/*  Allocation                      */
/*                                  */
/************************************/

typedef struct metadata {
  char format_string[10];
  char bit_vector;
  void* forwarding_address;
  bool copied_flag;
} metadata_t;

void *h_alloc_data(heap_t* h, size_t bytes) {
  size_t total_bytes = bytes + sizeof(metadata_t);

  if(h->bump_p + total_bytes <= h->end_p) {// if there is space, allocate
    // save bump pointer for returning. This pointer skips the metadata
    void* new_pointer = h->bump_p + sizeof(metadata_t);
    // update bump pointer and avail space
    h->bump_p += total_bytes;
    h->used_space += total_bytes;
    h->avail_space -= total_bytes;
    
    // update metadata
    md_set_format_string(new_pointer, "none");
    md_set_bit_vector(new_pointer, '\0');
    md_set_forwarding_address(new_pointer, NULL);
    md_set_copied_flag(new_pointer, false);
    
    // return pointer
    return new_pointer;    
  } else { // if there is no space, return null
    return NULL;
  }
}

size_t fs_calculate_size(char* format_string) {
  int fs_length = strlen(format_string);
  int multiplier = 1;

  size_t size = 0;
  
  for(int i = 0; i < fs_length; i++) {
    if(format_string[i] >= 'A' && format_string[i] <= 'Z') {
      format_string[i] = tolower(format_string[i]);
    }
    switch(format_string[i]) {
      // if any of the following characters: multiply size with multiplier and add to size. 
      // Pointers
    case '*':
      size += sizeof(void*) * multiplier;
      multiplier = 1;
      break;
      // Integers
    case 'i':
      size += sizeof(int) * multiplier;
      multiplier = 1;
      break;
      // Floats
    case 'f':
      size += sizeof(float) * multiplier;
      multiplier = 1;
      break;
      // Characters
    case 'c':
      size += sizeof(char) * multiplier;
      multiplier = 1;
      break;
      // Longs
    case 'l':
      size += sizeof(long) * multiplier;
      multiplier = 1;
      break;
      // Doubles
    case 'd':
      size += sizeof(double) * multiplier;
      multiplier = 1;
      break;
      // if none of these characters, then check if it is a multiplier
    default:  
      if(format_string[i] >= '0' && format_string[i] <= '9') { 
	// if the char is an integer, convert and save to multiplier.
	int digit = format_string[i] -  '0'; 
	if(multiplier == 1) {
	  multiplier = digit;
	} else {
	  multiplier = multiplier * 10 + digit;
	} 
      } else {
	// if an invalid character is in the string, return 0
	return 0;
      }	    
	break;
    }
  }
  return size;
}


void *h_alloc_struct(heap_t* h, char* format_string) {
  size_t object_bytes = fs_calculate_size(format_string);
  size_t metadata_bytes = sizeof(metadata_t);
  size_t total_bytes = object_bytes + metadata_bytes;

  if(object_bytes && h->bump_p + total_bytes <= h->end_p) {
    // if there is space or if calculation succeeded
    
    // save bump pointer for returning. This pointer skips the metadata
    void* new_pointer = h->bump_p + metadata_bytes;
    // update bump pointer and avail space
    h->bump_p += total_bytes;
    h->used_space += total_bytes;
    h->avail_space -= total_bytes;

    // update metadata
    md_set_format_string(new_pointer, format_string);
    md_set_bit_vector(new_pointer, '\0');
    md_set_forwarding_address(new_pointer, NULL);
    md_set_copied_flag(new_pointer, false);
    
    // return pointer
    return new_pointer;    
  } else {
    // if there is no space or if calculation failed
    return NULL;
  }
}

bool validate_object(void* object) {
  // TODO: Doesn't validate anything yet. 
  return true; 
}


/************************************/
/*                                  */
/*  Metadata wrapper                */
/*                                  */
/************************************/

// Format string

#define FORMAT_STRING_PTR(object) (char*)(object - sizeof(metadata_t))

char* md_get_format_string(void* object) {
  return FORMAT_STRING_PTR(object);
}

void md_set_format_string(void* object, char* format_string) {
  strcpy(FORMAT_STRING_PTR(object), format_string);
}

// Bit vector

#define BIT_VECTOR_PTR(object) (char*)(object - sizeof(metadata_t) + sizeof(char*))

char md_get_bit_vector(void* object) {
  return *BIT_VECTOR_PTR(object);
}

void md_set_bit_vector(void* object, char bit_vector) {
  *BIT_VECTOR_PTR(object) = bit_vector;
}

// Forwarding address

#define FORWARDING_ADDRESS_PTR(object) (void**)(object - sizeof(metadata_t) + sizeof(void*) + sizeof(char))

void* md_get_forwarding_address(void* object) {
  return *FORWARDING_ADDRESS_PTR(object);
}

void md_set_forwarding_address(void* object, void* forwarding_address) {
  *FORWARDING_ADDRESS_PTR(object) = forwarding_address;
}

// Copied flag

#define COPIED_FLAG_PTR(object) (bool*)(object - sizeof(metadata_t) + sizeof(void*) + sizeof(char) + sizeof(void*))

bool md_get_copied_flag(void* object) {
  return *COPIED_FLAG_PTR(object);
}

void md_set_copied_flag(void* object, bool copied_flag) {
  *COPIED_FLAG_PTR(object) = copied_flag;
}

size_t fs_get_object_size(void* object) {
  return fs_calculate_size(md_get_format_string(object));
}

ll_head fs_get_pointers_within_object(void* object) {
  char* format_string = md_get_format_string(object);

  int multiplier = 1;

  void *pointer = object;

  ll_head pointer_list = LL_initRoot();
  
  for(int i = 0; i < strlen(format_string); i++) { 
    switch(format_string[i]) {
      // if a pointer, add to list and increment pointer appropriately
      // if any of the other characters, increment pointer appropriately
      // Pointers
    case '*':
      for(int i = 0; i < multiplier; i++) {
	LL_createAndInsertSequentially(pointer_list, pointer);
	pointer = ((void*) pointer) + 1;
      }
      multiplier = 1;
      break;
      // Integers
    case 'i':
      pointer = ((int*) pointer) + 1;
      multiplier = 1;
      break;
      // Floats
    case 'f':
      pointer = ((float*) pointer) + 1;
      multiplier = 1;
      break;
      // Characters
    case 'c':
      pointer = ((char*) pointer) + 1;
      multiplier = 1;
      break;
      // Longs
    case 'l':
      pointer = ((long*) pointer) + 1;
      multiplier = 1;
      break;
      // Doubles
    case 'd':
      pointer = ((double*) pointer) + 1;
      multiplier = 1;
      break;
      // if none of these characters, then check if it is a multiplier
    default:  
      if(format_string[i] >= '0' && format_string[i] <= '9') { 
	// if the char is an integer, convert and save to multiplier.
	int digit = format_string[i] -  '0'; 
	if(multiplier == 1) {
	  multiplier = digit;
	} else {
	  multiplier = multiplier * 10 + digit;
	} 
      } else {
	// if an invalid character is in the string, return null
	return NULL;
      }
    }
  }
  return pointer_list;
}
