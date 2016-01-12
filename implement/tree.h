#ifndef __tree_h_
#define __tree_h_
#include "list.h"

/// \file tree.h
///
/// \author Christoffer Gustafsson
/// \version 1.0
/// \date 2015-10-12
/// \bug None know

typedef struct tree tree_t;

typedef int (*cmpkey)(void*, void*);
typedef void (*print_keyt)(void*, int, char);
typedef void (*print_key)(void*);
typedef void (*free_key)(void*);
typedef void* (*col)(void*, void*);
typedef void (*print)(tree_t *tree, void*, int *tree_index,
		      int page,int ware_choice);

enum result{
  LEFT,
  RIGHT,
  EMPTY,
  MATCH
};

/// Create a new empty tree.
///
/// \param data_size size of data to put in list
/// \param _free_data function who frees data from heap
/// \param _free_key function who frees key from heap
/// \returns empty tree with a initialized list
tree_t* tree_new(int data_size, free_data _free_data, 
		 free_key _free_key);

/// Remove tree and free up the memory used.
///
/// \param tree tree to free
void    tree_destroy(tree_t** tree);

/// Insert key and data in a node.
///
/// \param key Must be on heap.
/// \param data Can be on stack.
/// \returns False if collision occured, else true.
bool    tree_insert(tree_t** tree, void* key, cmpkey _cmpkey, 
		    void* data, cmpdata _cmpdata, int data_size, 
		    free_data _free_data, free_key _free_key, 
		    col collision);

/// Remove first node in given tree.
///
/// \param key remove tree with matching key
/// \param _cmpkey function which compare keys
/// \returns given tree but updated
tree_t*  tree_remove(tree_t* tree, void* key, cmpkey _cmpkey);

/// Find node with given key.
///
/// \param key remove tree with matching key
/// \param _cmpkey function which compare keys
/// \returns Tree containing give key or null.
tree_t* tree_find(tree_t* tree, void* key, cmpkey _cmpkey);

/// Total nodes on largest branch.
/// 
/// \returns total dept of given tree, including self
int     tree_depth(tree_t* tree);

/// Get total amount of nodes.
///
/// \returns total  amount of nodes, 0 if null tree
int     tree_nodes(tree_t* tree);

/// Return key.
void* tree_getKey(tree_t* tree);

/// Return list.
list_t* tree_getList(tree_t* tree);

/// Print the keys of the tree, left to right.
void    tree_print(tree_t* tree, print_keyt _print_key);

/// Compare all list nodes in whole tree. 
/// function terminates and returns true if match found
bool    tree_for_all(tree_t* tree, void* key, cmpkey _cmpkey, 
		     void* data, cmpdata _cmpdata);

/// Print out tree in alphabetical order.
void for_all_nodes(tree_t *tree, print _print,int *tree_index, int page,int ware_choice);

#endif
