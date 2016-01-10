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
  new_heap->user_start_p      = (void*) new_heap + sizeof(heap_t);
  new_heap->active_page_list  = NULL;
  new_heap->passive_page_list = (void*) new_heap + sizeof(heap_t);
  new_heap->compact_page_list = NULL;
  new_heap->end_p             = (void*) new_heap + bytes - 1;
  new_heap->total_size        = bytes;
  new_heap->user_size         = bytes - sizeof(heap_t);
  new_heap->used_space        = 0;
  new_heap->avail_space       = bytes - sizeof(heap_t);
  new_heap->unsafe_stack      = unsafe_stack;
  new_heap->gc_threshold      = gc_threshold;

  // set up pages
  size_t bytes_left = bytes;
  page_t* cursor = new_heap->passive_page_list;

  while(cursor) {
    // set defaults for page
    cursor->user_start_p = (void*) cursor + sizeof(page_t);
    cursor->bump_p       = (void*) cursor + sizeof(page_t);
    cursor->end_p        = (void*) cursor + (bytes_left > PAGE_SIZE ? PAGE_SIZE : bytes_left);
    cursor->active       = false;
    cursor->unsure       = false;
    cursor->next_page    = (bytes_left > PAGE_SIZE ? (void*) cursor->end_p + 1 : NULL);
    // go to next page and update bytes_left as well as user_size and avail_space in heap
    cursor = cursor->next_page;
    bytes_left -= PAGE_SIZE;
    new_heap->total_size -= sizeof(page_t);
    new_heap->avail_space -= sizeof(page_t);
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

size_t fs_calculate_size(char* format_string) {
  int fs_length = strlen(format_string);
  int multiplier = 0;

  size_t size = 0;

  for(int i = 0; i < fs_length; i++) {
    if(format_string[i] >= 'A' && format_string[i] <= 'Z') {
      format_string[i] = tolower(format_string[i]);
    }
    switch(format_string[i]) {
      // if any of the following characters: multiply size with multiplier and add to size.
      // Pointers
    case '*':
      size += sizeof(void*) * (multiplier ? multiplier : 1);
      multiplier = 0;
      break;
      // Integers
    case 'i':
      size += sizeof(int) * (multiplier ? multiplier : 1);
      multiplier = 0;
      break;
      // Floats
    case 'f':
      size += sizeof(float) * (multiplier ? multiplier : 1);
      multiplier = 0;
      break;
      // Characters
    case 'c':
      size += sizeof(char) * (multiplier ? multiplier : 1);
      multiplier = 0;
      break;
      // Longs
    case 'l':
      size += sizeof(long) * (multiplier ? multiplier : 1);
      multiplier = 0;
      break;
      // Doubles
    case 'd':
      size += sizeof(double) * (multiplier ? multiplier : 1);
      multiplier = 0;
      break;
      // if none of these characters, then check if it is a multiplier
    default:
      if(format_string[i] >= '0' && format_string[i] <= '9') {
	// if the char is an integer, convert and save to multiplier.
	int digit = format_string[i] -  '0';
        multiplier = multiplier * 10 + digit;
      } else {
	// if an invalid character is in the string, return 0
	return 0;
      }
	break;
    }
  }
  return size;
}

void post_compact_page_reset(heap_t *h) {
  page_t *cursor = h->active_page_list;

  while(cursor) {
    // reset bump_p
    cursor->bump_p = (void*) cursor;
    // set page to apssive
    cursor->active = false;    
    // next page
    cursor = cursor->next_page;
  }

  cursor = h->passive_page_list;
  if(cursor) {
    // if list is not empty
    while(cursor->next_page) {
      cursor=cursor->next_page;
    }
    cursor->next_page = h->active_page_list;
  } else {
    // if list is empty
    h->passive_page_list = h->active_page_list;
  }

  // set compact_page_list becomes the active list
  h->active_page_list = h->compact_page_list;
  // reset compact_page_list
  h->compact_page_list = NULL;
}

page_t *get_allocation_page(heap_t* h, size_t bytes,  bool compact) {
  page_t *active_page_cursor;
  if(compact) {
    active_page_cursor = h->compact_page_list;
  } else {
    active_page_cursor = h->active_page_list;
  }
  page_t *previous_active_page_cursor = NULL;

  while(active_page_cursor) {

    // go through all active pages
    if(active_page_cursor->bump_p + bytes < active_page_cursor->end_p) {
      // if object fits in page return that page
      return active_page_cursor;
    }
    // go to next active page
    previous_active_page_cursor = active_page_cursor;
    active_page_cursor = active_page_cursor->next_page;
  }

  // if this point is reached, then no active pages are suitable
  page_t *passive_page_cursor = h->passive_page_list;
  page_t *previous_passive_page_cursor = NULL;
  while(passive_page_cursor) {
    // go through all passive pages

    if(passive_page_cursor->bump_p + bytes < passive_page_cursor->end_p) {
      // if object fits in page make active and return that page
      passive_page_cursor->active = true;

      // TODO: make this a comparative insertion so pages are always in order.
      // add passive page to active page list
      if(previous_active_page_cursor == NULL) {
        // if active page list is empty
	if(compact) {
	  h->compact_page_list = passive_page_cursor;
	} else {
	  h->active_page_list = passive_page_cursor;
	}
      } else {
        // if active page list is not empty
        previous_active_page_cursor->next_page = passive_page_cursor;
      }

      // remove passive page from passive page list
      if(previous_passive_page_cursor == NULL) {
        // if it is the first page in list, change heaps list pointer
        h->passive_page_list = passive_page_cursor->next_page;
      } else {
        // if it is not the first page in list, change previous page's next pointer
        previous_passive_page_cursor->next_page = passive_page_cursor->next_page;
      }
      passive_page_cursor->next_page = NULL;
      return passive_page_cursor;
    }

    // go to next passive page
    previous_passive_page_cursor = passive_page_cursor;
    passive_page_cursor = passive_page_cursor->next_page;
  }
  // if no page was found, return NULL
  return NULL;
}

void *h_alloc(heap_t* h, size_t bytes, char* format_string, bool compact) {
  size_t total_bytes = bytes + sizeof(metadata_t);
  page_t *p = get_allocation_page(h, total_bytes, compact);
  
  if(p && p->bump_p + total_bytes <= p->end_p) {// if there is a page and it has space, allocate
    // save bump pointer for returning. This pointer skips the metadata
    void* new_pointer = p->bump_p + sizeof(metadata_t);
    // update bump pointer and avail space
    p->bump_p += total_bytes;
    h->used_space += total_bytes;
    h->avail_space -= total_bytes;

    // update metadata
    md_set_format_string(new_pointer, format_string);
    md_set_bit_vector(new_pointer, '\0');
    md_set_forwarding_address(new_pointer, NULL);
    md_set_copied_flag(new_pointer, false);

    // return pointer
    return new_pointer;
  } else { // if there is no page or space, return null
    return NULL;
  }
}

void *h_alloc_data(heap_t* h, size_t bytes) {
  return h_alloc(h, bytes, "none", false); 
}

void *h_alloc_struct(heap_t* h, char* format_string) {
  size_t object_bytes = fs_calculate_size(format_string);
  if(object_bytes) {
    // if calculation succeeded, allocate
    return h_alloc(h, object_bytes, format_string, false);
  } else {
    // if calculation failed, return NULL
    return NULL;
  }
}

void *h_alloc_compact(heap_t* h, void* object) {
  char* format_string = md_get_format_string(object);
  size_t object_bytes = fs_calculate_size(format_string);
  if(object_bytes) {
    // if calculation succeeded, allocate
    return h_alloc(h, object_bytes, format_string, true);
  } else {
    // if calculation failed, return NULL
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
// TODO: I think this is wrong. Don't this this should be here. 
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

void update_objects_pointers(void* object) {
  char* format_string = md_get_format_string(object);

  int multiplier = 1;

  void *pointer = object;
  
  for(int i = 0; i < strlen(format_string); i++) { 
    switch(format_string[i]) {
      // if a pointer, add to list and increment pointer appropriately
      // if any of the other characters, increment pointer appropriately
      // Pointers
    case '*':
      for(int i = 0; i < multiplier; i++) {
	// follow pointer, get metadata and update.
	if(md_get_forwarding_address(*((void**) pointer))) {
	  *((void**) pointer) = md_get_forwarding_address(*((void**) pointer));
	}	
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
      }
    }
  }
}
