#include "linked_list.h"
#include <stdlib.h> // For malloc

// NODE STRUCT

typedef struct node{
  struct node* next;
  struct node* previous;
  void* nodeContent;
}ll_node;

// CREATE FUNCTIONS

ll_node** LL_initRoot() {
  ll_node** root = malloc(sizeof(ll_node*));
  *root = NULL;
  return root;
}

ll_node* LL_createNode(void* content) {
  ll_node* newNode = malloc(sizeof(ll_node));
  newNode->next = NULL;
  newNode->previous = NULL;
  newNode->nodeContent = content;
  return newNode;
}

// GET FUNCTIONS

ll_node* LL_getNext(ll_node* toGetFrom) {
  if(toGetFrom != NULL) {
    return toGetFrom->next;
  } else {
    return NULL;
  }
}

ll_node* LL_getPrevious(ll_node** root, ll_node* toGetFrom) {
  if(toGetFrom != NULL) {
    return toGetFrom->previous;
  } else {
    return NULL;
  }
}

void* LL_getContent(ll_node* toGetFrom) {
  if(toGetFrom != NULL) {
    return toGetFrom->nodeContent;
  } else {
    return NULL;
  }
}

ll_node* LL_getNodeAtIndex(ll_node** root, int index) {
  ll_node* currentNode = *root;
  int i = 0;
  while(currentNode != NULL && i < index) {
    currentNode = currentNode->next;
    i++;
  }
  return currentNode;
}

int LL_getNodeIndex(ll_node** root, ll_node* toGetFrom) {return -1;};

// INSERT FUNCTIONS

void LL_insertSequentially(ll_node** root, ll_node* toInsert){
  // if currentNode is empty, the list is empty and root will point at the new node
  if(*root == NULL) {
    *root = toInsert;
  } else {
    ll_node* cursor = *root;
    while(cursor->next != NULL) {
      cursor = cursor->next;
    }
    toInsert->previous = cursor;
    cursor->next = toInsert;
  }
}


ll_node* LL_createAndInsertSequentially(ll_node** root, void* content){
  ll_node* newNode = LL_createNode(content);
  LL_insertSequentially(root, newNode);
  return newNode;
}

/* insertComparatively
   Inserts a node into the linked list and uses a pointer to a function which accepts two void pointers and compares the values at these pointers. This function will insert the node whenever the function returns 0 or more.
 */

void LL_insertComparatively(ll_node** root, ll_node* toInsert,
			 int(*compareFunction)(void*, void*)) {
  if(toInsert != NULL) {
    if(*root == NULL) {
      // if list is empty
      *root = toInsert;
    } else if (compareFunction(toInsert->nodeContent, (*root)->nodeContent) < 0) {
      // if the new node should be all the way to the left
      toInsert->next = *root;
      *root = toInsert;
      if(toInsert->next != NULL) {
	toInsert->next->previous = toInsert;
      }
    } else {
      ll_node* cursor = *root;
      while(cursor->next != NULL &&
	    compareFunction(toInsert->nodeContent, cursor->next->nodeContent) >= 0) {
	cursor = cursor->next;
      }
      toInsert->next = cursor->next;
      toInsert->previous = cursor;
      cursor->next = toInsert;
      if(toInsert->next != NULL) {
	toInsert->next->previous = toInsert;
      }
    }
  }
}

ll_node* LL_createAndInsertComparatively(ll_node** root, void* content,
					  int(*compareFunction)(void*, void*)) {
  ll_node* newNode = LL_createNode(content);
  LL_insertComparatively(root, newNode, compareFunction);
  return newNode;
}

// REMOVE FUNCTIONS

ll_node* LL_removePointer(ll_node** root, ll_node* toRemove){
  if(toRemove == NULL || LL_isEmpty(root)) {
    return NULL;
  } else if(*root == toRemove) {
    // if toRemove is first node, set root as next node
    *root = toRemove->next;
    if(*root != NULL) {
      (*root)->previous = NULL;
    }
    return toRemove;
  } else {
    // if toRemove is in other part of the list, search for it and
    // make previous node point to the one after toRemove.
    ll_node* cursor = *root;

    while(cursor->next != NULL && cursor->next != toRemove){
      cursor = cursor->next;
    }
    // if the node was found, remove and return it, otherwise return NULL
    if(cursor->next != NULL) {
      cursor->next = cursor->next->next;
      if(cursor-> next != NULL) {
	cursor->next->previous = cursor;
      }
      return toRemove;
    } else {
      return NULL;
    }
  }
}

ll_node* LL_removeIndex(ll_node** root, int index){
  if(index < 0 || LL_isEmpty(root)) {
    // if index is less than 0 or if list is empty return NULL
    return NULL;
  } else if(index == 0) {
    // if index is 0, set root as next node
    ll_node* returnNode = *root;
    *root = (*root)->next;
    if(*root != NULL) {
      (*root)->previous = NULL;
    }
    return returnNode;
  } else {
    // if index is higher than 0, search for node and make previous
    // node point to the one after the one at index.

    ll_node* cursor = *root;

    int i = 0;
    while(cursor->next != NULL && i < index-1){
      cursor = cursor->next;
      i++;
    }
    // if the node was found, remove and return it, otherwise return NULL
    if(cursor->next != NULL) {
      ll_node* returnNode = cursor->next;
      cursor->next = cursor->next->next;
      if(cursor->next != NULL) {
	cursor->next->previous = cursor;
      }
      return returnNode;
    } else {
      return NULL;
    }
  }
}

void* LL_deletePointer(ll_node** root, ll_node* toRemove){
  ll_node* node = LL_removePointer(root, toRemove);
  void* content = LL_getContent(node);
  free(node);
  return content;
}

void* LL_deleteIndex(ll_node** root, int index){
  ll_node* node = LL_removeIndex(root, index);
  void* content = LL_getContent(node);
  free(node);
  return content;
}

void LL_purgePointer(ll_node** root, ll_node* toRemove){
  free(LL_deletePointer(root, toRemove));
}

void LL_purgeIndex(ll_node** root, int index){
  free(LL_deleteIndex(root, index));
}

void LL_deleteList(ll_node** root){
  for(int i = LL_length(root); i > 0; i--) {
    LL_deletePointer(root, *root);
  }
  free(root);
}

void LL_purgeList(ll_node** root){
  for(int i = LL_length(root); i > 0; i--) {
    LL_purgePointer(root, *root);
  }
  free(root);
}

void LL_purgeNode(ll_node* node){
  free(LL_getContent(node));
  free(node);
}

/************************************/
/*                                  */
/*  MISC FUNCTIONS                  */
/*                                  */
/************************************/

int LL_length(ll_node** root) {
  int i = 0;
  ll_node* cursor = *root;

  while(cursor != NULL) {
    cursor = cursor->next;
    i++;
  }
  return i;
}

int LL_isEmpty(ll_node** root) {
  return(*root == NULL);
}

int LL_isValidAux(ll_node* cursor, int(validationFunction)(void*, void*), void* validationData);
int LL_isValid(ll_node** root, int(validationFunction)(void*, void*), void* validationData) {

  if(validationData == NULL){
    return 0;
  }

  if(root == NULL) {
    // if there is no list return 1. This is because the assumption is
    // that a list is valid and saying it is invalid implies that
    // nodes were checked.
    return 1;
  } else {
    return LL_isValidAux(*root, validationFunction, validationData);
  }
}

int LL_isValidAux(ll_node* cursor, int(validationFunction)(void*, void*), void* validationData) {
  if(cursor == NULL) {
    // if end of list is reached, return valid
    return 1;
  } else if(!validationFunction(LL_getContent(cursor), validationData)) {
    // if validation function returns 0, return invalid
    return 0;
  } else {
    // recursively continue
    return LL_isValidAux(LL_getNext(cursor), validationFunction, validationData);
  }
}

void LL_map(ll_node** root, void(*mapFunction)(void*)) {
  ll_node* cursor = *root;

  while(cursor != NULL) {
    mapFunction(LL_getContent(cursor));
    cursor = LL_getNext(cursor);
  }
}

ll_node* LL_search(ll_node** root, void* content, int(*compareFunction)(void*, void*)) {
  ll_node* cursor;
  if(root != NULL) {
    cursor = *root;
  } else {
    cursor = NULL;
  }

  while(cursor != NULL) {
    if(compareFunction(LL_getContent(cursor), content) == 0) {
      // if content matches the content of the cursor, return cursor
      return cursor;
    }
    cursor = LL_getNext(cursor);
  }
  // if nothing was found, return NULL
  return NULL;
}
