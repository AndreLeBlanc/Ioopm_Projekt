#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "traverser.c"

void retrieveAllPointers() { // calls on the traverser to retrieve all ponters in our heap
  return;
}

void checkUnsafePointers() { // checks for and handles unsafe pointers
  return;
}

void bartlettCompact() { // performs the bartlett operations
  return;
}

int main(int argc, const char** argv, const char** envp) { // just testing
  register void* stack asm("esp");
  // put stack somewhere
  return do_main(argc, argv, envp);
}
