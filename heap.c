#include "heap.h"
#include <string.h>

// mallocates space for heap, places metadata in the front. 

heap_t *h_init(size_t bytes, bool unsafe_stack, float gc_threshold) {
  if(bytes < sizeof(heap_t)) {
    // if space allocated is not even enough for metadata, don't allocate
    return NULL;
  }

  void *new_heap = malloc(bytes);

  // create metadata struct and place it in the front of the heap
  // the user sees this as a heap_t struct and is therefore not
  // aware that it is the pointer to the whole heap. 
  ((heap_t*) new_heap)->meta_p = new_heap;
  ((heap_t*) new_heap)->user_start_p = new_heap + (sizeof(heap_t) * 8);
  ((heap_t*) new_heap)->bump_p = new_heap + (sizeof(heap_t) * 8);
  ((heap_t*) new_heap)->end_p = new_heap + (bytes * 8);
  ((heap_t*) new_heap)->total_size = bytes;
  ((heap_t*) new_heap)->user_size = bytes - sizeof(heap_t);
  ((heap_t*) new_heap)->avail_space = bytes - sizeof(heap_t);
  ((heap_t*) new_heap)->unsafe_stack = unsafe_stack;
  ((heap_t*) new_heap)->gc_threshold = gc_threshold;
  return new_heap;
}

void h_delete(heap_t *h) {
  if(h != NULL) {
    free(h);
  } 
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

  if(h->bump_p + (total_bytes * 8) <= h->end_p) {
    // if there is space
    
    // save bump pointer for returning. This pointer skips the metadata
    void* new_pointer = h->bump_p + sizeof(metadata_t);
    // update bump pointer and avail space
    h->bump_p += (total_bytes * 8);
    h->avail_space -= total_bytes;

    // update metadata
    md_set_format_string(new_pointer, "none");
    md_set_bit_vector(new_pointer, '\0');
    md_set_forwarding_address(new_pointer, NULL);
    md_set_copied_flag(new_pointer, false);
    
    // return pointer
    return new_pointer;    
  } else {
    // if there is no space
    return NULL;
  }
}

size_t fs_calculate_size(char* format_string) {
  int fs_length = strlen(format_string);
  int multiplier = 0;

  size_t size = 0;
  
  for(int i = 0; i < fs_length; i++) {
    switch(format_string[i]) {
    case '*':
      break;
    case 'i':
      break;
    case 'f':
      break;
    case 'c':
      break;
    case 'l':
      break;
    case 'd':
      break;
    default:  // if none of these characters, then check if it is a multiplier
      if(format_string[i] > '0' && format_string[i] <= '9') { // TODO: Check if this is right
	// if the char is an integer, convert and save to multiplier.
	int digit = format_string[i]; // TODO: Convert to int
	if(multiplier == 0) {
	  multiplier = digit;
	} else {
	  multiplier = multiplier * 10 + digit;
	} 
      }
	break;
    }
  }
  return size;
}


void *h_alloc_struct(heap_t* h, char* format_string) {
  size_t total_bytes = fs_calculate_size(format_string) + sizeof(metadata_t);

  if(h->bump_p + (total_bytes * 8) <= h->end_p) {
    // if there is space
    
    // save bump pointer for returning. This pointer skips the metadata
    void* new_pointer = h->bump_p + sizeof(metadata_t);
    // update bump pointer and avail space
    h->bump_p += (total_bytes * 8);
    h->avail_space -= total_bytes;

    // update metadata
    md_set_format_string(new_pointer, format_string);
    md_set_bit_vector(new_pointer, '\0');
    md_set_forwarding_address(new_pointer, NULL);
    md_set_copied_flag(new_pointer, false);
    
    // return pointer
    return new_pointer;    
  } else {
    // if there is no space
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

#define FORMAT_STRING_PTR(object) ((char*) object - sizeof(metadata_t))

char* md_get_format_string(void* object) {
  return FORMAT_STRING_PTR(object);
}

void md_set_format_string(void* object, char* format_string) {
  strcpy(FORMAT_STRING_PTR(object), format_string);
}

// Bit vector

#define BIT_VECTOR_PTR(object) ((char*) object - sizeof(metadata_t) + sizeof(char*))

char md_get_bit_vector(void* object) {
  return *BIT_VECTOR_PTR(object);
}

void md_set_bit_vector(void* object, char bit_vector) {
  *BIT_VECTOR_PTR(object) = bit_vector;
}

// Forwarding address

#define FORWARDING_ADDRESS_PTR(object) ((void**) object - sizeof(metadata_t) + sizeof(void*) + sizeof(char))

void* md_get_forwarding_address(void* object) {
  return *FORWARDING_ADDRESS_PTR(object);
}

void md_set_forwarding_address(void* object, void* forwarding_address) {
  *FORWARDING_ADDRESS_PTR(object) = forwarding_address;
}

// Copied flag

#define COPIED_FLAG_PTR(object) ((bool*) object - sizeof(metadata_t) + sizeof(void*) + sizeof(char) + sizeof(void*))

bool md_get_copied_flag(void* object) {
  return *COPIED_FLAG_PTR(object);
}

void md_set_copied_flag(void* object, bool copied_flag) {
  *COPIED_FLAG_PTR(object) = copied_flag;
}

size_t fs_get_object_size(void* object) {
  return fs_calculate_size(md_get_format_string(object));
}
