#include "linked_list.h"

#ifndef __heap_traverser__
#define __heap_traverser__

/**
@breif Traverses the heap from all pointers in the list and updates the list with all the new pointers found.
@param pointers a list with stack pointers
@return A list with stack pointers and pointers to all alive objects in our heap
@warning The list of pointers has to be allocated inside a heap of our own design.
*/
ll_head traverse_pointers_from_LL(ll_head pointers);

/**
@breif Prints the adress of an object.
@param object
 */
void printAddress(void *object);

#endif
