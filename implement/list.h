#ifndef __list_h_
#define __list_h_
#include <stdbool.h>
#include "main.h"

/// \file list.h
///
/// \author Christoffer Gustafsson
/// \version 1.0
/// \date 2015-10-12
/// \bug None know

typedef struct listNode listNode_t;
typedef struct list list_t;
typedef void (*free_data)(void*);
typedef void (*print_data)(void*);
typedef bool (*cmpdata)(void*, void*);

/// Initialize list on heap.
///
/// \param elementSize size of data to put in list.
/// \returns empty list
list_t* list_new(int elementSize, free_data _free_data);

/// Free list and all of its content.
///
/// \param list list to be freed
void    list_destroy(list_t* list);

/// Copy data to lists heap.
///
/// \param list list to append to
/// \data data to put in list
/// \returns true if succesfull, else false
bool    list_append(list_t* list, void* data);

/// Remove single node from list and free it.
///
/// \param list list to remove from
/// \param index which element to remove, head is 1
void    list_remove(list_t* list, int index);

/// Get latest added data.
///
/// \param list list to operate on
/// \returns pointer latest added data
void*   list_tail(list_t* list);

/// Get first added data.
///
/// \param list list to operate on
/// \returns pointer to first data
void*   list_head(list_t* list);

/// Get given data, first element is 1.
///
/// \param list list to operate on
/// \param index which element to get, head is 1
/// \returns pointer to data
void*   list_get(list_t* list, int index);

/// Get total elements in list.
/// 
/// \param list list to operate on
/// \returns length of list
int     list_length(list_t* list);

/// Get data size in bytes of data.
///
/// \param list list to operate on
/// \returns size of the data elements
int     list_size(list_t* list);

/// Compare all nodes in list with data.
///
/// \param list list to operate on
/// \param data data to compare elements to
/// \param _cmpdata function which compares data
/// \returns true if any element match data, else false
bool    list_for_all(list_t* list, void* data, cmpdata _cmpdata);

#endif
