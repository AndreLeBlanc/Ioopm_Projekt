#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "traverser.h"
#include "linked_list.h"

void retrieveAllPointers() { // calls on the traverser to retrieve all ponters in our heap
  return;
}

void checkUnsafePointers() { // checks for and handles unsafe pointers
  return;
}

void bartlettCompact(ll_head marked_list) { // performs the bartlett operations
  // Make a list of pages, grab only from passive lists.
  // Add a page, and start filling it, when it's full, get a new one from the passives.
  // When the compacting is done, make all pages in the heap's active page list passive
  // And this list becomes the active page list. 

  // Go through list
  //    Copy object to a new
  //    Set copied flag
  //    Set forwarding address

  // Go through list and update pointers
  return;
}

/*
int main(int argc, const char** argv, const char** envp) { // just testing
  register void* stack asm("esp");
  // put stack somewhere
  return do_main(argc, argv, envp);
}
*/
