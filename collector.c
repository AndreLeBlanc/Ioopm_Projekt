#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "traverser.h"
#include "linked_list.h"
#include "heap.h"

void retrieveAllPointers() { // calls on the traverser to retrieve all ponters in our heap
  return;
}

void checkUnsafePointers() { // checks for and handles unsafe pointers
  return;
}

void bartlettCompact(heap_t *h, ll_head marked_list) { // performs the bartlett operations
  // go through list of objects and allocate a new space for them
  ll_node *cursor = *marked_list;
  while(cursor) {
    // get new pointer
    void* new_pointer = h_alloc_compact(h, LL_getContent(cursor));
    // set forwarding address
    md_set_forwarding_address(LL_getContent(cursor), new_pointer);

    // move along cursor
    cursor = LL_getNext(cursor);
  }

  // go through list of objects again, this time updating pointers and then copying data
  cursor = *marked_list;
  while(cursor) {
    void* pointer = LL_getContent(cursor);
    // update pointers
    update_objects_pointers(pointer);

    //TODO, update stack pointers

    // copy all data
    memcpy(md_get_forwarding_address(pointer),
	   pointer,
	   fs_get_object_size(pointer));

    // set forwarding address of copied data to NULL
    md_set_forwarding_address(md_get_forwarding_address(pointer), NULL);

    // set copied flag
    md_set_copied_flag(LL_getContent(cursor), true);

    // move along cursor
    cursor = LL_getNext(cursor);
  }

  // set pages
  post_compact_page_reset(h);
}

/*
  Removed because of confusion. /V
*/
// int main(int argc, const char** argv, const char** envp) { // just testing
//   register void* stack asm("esp");
//   // put stack somewhere
//   return do_main(argc, argv, envp);
// }
