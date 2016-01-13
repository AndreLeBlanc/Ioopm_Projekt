#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "list.h"
#include "../gc.h"

extern heap_t* our_heap;

struct listNode{
  void* data;
  struct listNode *next;
};

struct list{
  int length;
  int elementSize;
  listNode_t *tail;
  listNode_t *head;
};

list_t* list_new(int elementSize, free_data _free_data){
  assert(elementSize > 0);

  list_t* list = h_alloc_struct(our_heap, "ii**");
  list->length = 0;
  list->elementSize = elementSize;

  return list;
}

listNode_t* list_node_free(list_t* list, listNode_t* node){
  /*listNode_t* next = node->next;
  list->_free_data(node->data);
  free(node);*/
  return NULL;
}

void list_destroy(list_t* list){
  /*if(list == NULL) return;
  while(list->head != NULL) list->head = list_node_free(list, list->head);
  free(list);*/
}

bool list_append(list_t* list, void* data){
  if(list == NULL) return false;
  listNode_t* node = h_alloc_struct(our_heap, "**");
  node->data = h_alloc_data(our_heap, sizeof(void *));
  memcpy(node->data, data, list->elementSize);

  if(list->length == 0) list->tail = list->head = node;
  else list->tail = list->tail->next = node;
  list->length++;
  return true;
}

void list_remove(list_t* list, int index){
  /*  if(!list || index < 1 || index > list->length) return;

  if(index==1){
    list->head = list_node_free(list, list->head);
  }
  else if(index == list->length){
    listNode_t* temp = list->head;
    while(temp->next != list->tail) temp = temp->next;
    temp->next = list_node_free(list, list->tail);
    list->tail = temp;
  }
  else{
    int c=0;
    listNode_t* temp = list->head;
    while(++c < index-1) temp = temp->next;
    listNode_t* temp2 = temp->next->next;
    list->_free_data(temp->next->data);
    free(temp->next);
    temp->next = temp2;
  }
  list->length--;*/
}

void* list_tail(list_t* list){
  return list->tail->data;
}

void* list_head(list_t* list){
  return list->head->data;
}

void* list_get(list_t* list, int index){
  if(!list || index < 1 || index > list->length) return NULL;

  int c=0;
  listNode_t* temp = list->head;
  while(++c < index) temp = temp->next;
  return temp->data;
}

int list_length(list_t* list){
  if(list) return list->length;
  else     return 0;
}

int list_size(list_t* list){
  if(list) return list->elementSize;
  else     return 0;
}

bool for_all_aux(listNode_t* node, void* data, cmpdata _cmpdata){
  if(node){
    if(_cmpdata(data, node->data)) return true;
    else return for_all_aux(node->next, data, _cmpdata);
  }
  return false;
}

bool list_for_all(list_t* list, void* data, cmpdata _cmpdata){
  if(list){
    return for_all_aux(list->head, data, _cmpdata);
  }
  return false;
}
