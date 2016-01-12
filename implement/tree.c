#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include "tree.h"


struct tree{
  void*        key;
  list_t*      list;
  struct tree* left;
  struct tree* right;
  free_key     _free_key;
};

tree_t* tree_new(int data_size, free_data _free_data,
		 free_key _free_key){
  tree_t* tree = h_alloc_struct(our_heap, "*****");
  tree->list = list_new(data_size, _free_data);
  tree->_free_key = _free_key;
  return tree;
}

void tree_free(tree_t** tree){
  /*  if((*tree)->key ) (*tree)->_free_key((*tree)->key);
  if((*tree)->list) list_destroy((*tree)->list);
  if(*tree) free(*tree);*/
}

void tree_destroy(tree_t** tree){
  /*if(*tree == NULL) return;
  tree_destroy(&(*tree)->left );
  tree_destroy(&(*tree)->right);
  tree_free(tree);*/
}

int tree_dir(tree_t* tree, void* key, cmpkey _cmpkey){
  return (tree == NULL ? EMPTY : _cmpkey(tree->key, key));
}

tree_t* tree_find(tree_t* tree, void* key, cmpkey _cmpkey){
  int dir = tree_dir(tree, key, _cmpkey);

  if     (dir == EMPTY) return tree;
  else if(dir == LEFT)  return tree_find(tree->left , key, _cmpkey);
  else if(dir == RIGHT) return tree_find(tree->right, key, _cmpkey);
  else return tree;
}

tree_t** successor(tree_t** tree, int dir){
  if     (dir==LEFT &&(*tree)->left )return successor(&(*tree)->left ,dir);
  else if(dir==RIGHT&&(*tree)->right)return successor(&(*tree)->right,dir);
  return tree;
}

void tree_clean(tree_t** tree){
  if(!*tree) return;
  list_destroy((*tree)->list);
  (*tree)->_free_key((*tree)->key);
  (*tree)->key = (*tree)->list = NULL;
}

void tree_swap(tree_t** dest, tree_t** src){
  void* key = (*dest)->key;
  void* list = (*dest)->list;
  (*dest)->key  = (*src)->key;
  (*dest)->list = (*src)->list;
  (*src)->key   = key;
  (*src)->list  = list;
}

void replace(tree_t** dest, tree_t** src){
  tree_swap(dest, src);
  void* left = (*src)->left;
  void* right = (*src)->right;
  tree_free(src);
  (*dest)->left = left;
  (*dest)->right = right;
}

tree_t* tree_remove(tree_t* tree, void* key, cmpkey _cmpkey){  
  int dir = tree_dir(tree, key, _cmpkey);

  if(dir == LEFT )
    tree->left  = tree_remove(tree->left , key, _cmpkey);
  else if(dir == RIGHT)
    tree->right = tree_remove(tree->right, key, _cmpkey);
  else if(dir == MATCH){
    if(!tree->left && !tree->right){
      tree_free(&tree);
      tree = NULL; //TODO needed?
    }
    else if(tree->left && !tree->right) replace(&tree, &tree->left);
    else if(tree->right && !tree->left) replace(&tree, &tree->right);
    else{
      tree_t** replacement;
      if(tree_depth(tree->right) >= tree_depth(tree->left)){
	replacement = successor(&tree->right, LEFT);
      }
      else replacement = successor(&tree->left, RIGHT);
      tree_swap(&tree, replacement);
      *replacement=tree_remove(*replacement,(*replacement)->key,_cmpkey);
    }
  }
  return tree;
}

bool tree_insert(tree_t** tree, void* key, cmpkey _cmpkey, 
		 void* data, cmpdata _cmpdata, int data_size,
		 free_data _free_data, free_key _free_key, col collision){
  int dir = tree_dir(*tree, key, _cmpkey);
  
  if(dir == EMPTY){
    *tree = tree_new(data_size, _free_data, _free_key);
    (*tree)->key = key;
    list_append((*tree)->list, data);
  }
  else if(dir == LEFT ) 
    return tree_insert(&(*tree)->left , key, _cmpkey, data, _cmpdata,
		data_size, _free_data, _free_key, collision);
  else if(dir == RIGHT) 
    return tree_insert(&(*tree)->right, key, _cmpkey, data, _cmpdata,
		data_size, _free_data, _free_key, collision);
  else{ //dir == MATCH
    bool found;
    for(int c=1; c<=list_length(tree_getList(*tree)); c++){
      found = _cmpdata(list_get((*tree)->list, c), data);
      if(found){
	collision(list_get((*tree)->list, c), data);
	return false;
      }
    }
    list_append((*tree)->list, data);
  }
  return true;
}

int tree_nodes(tree_t* tree){
  if(tree == NULL) return 0;
  else return (1 + tree_nodes(tree->left) + tree_nodes(tree->right));
}

int tree_depth(tree_t* tree){
  if(tree == NULL) return 0;
  int left = tree_depth(tree->left);
  int right = tree_depth(tree->right);

  if(left > right) return 1 + left;
  else             return 1 + right;
}

list_t* tree_getList(tree_t* tree){
  if(tree) return tree->list;
  else     return NULL;
}

void tree_print_aux(tree_t* tree, print_keyt _print_key,
		    int depth, char root){
  depth++;
  _print_key(tree->key, depth, root);
  if(tree->left ) tree_print_aux(tree->left ,_print_key,depth,'L');
  if(tree->right) tree_print_aux(tree->right,_print_key,depth,'R');
}

void tree_print(tree_t* tree, print_keyt _print_key){
  if(tree_depth(tree) == 0) return;

  int depth = 0;
  _print_key(tree->key, depth, 'T');
  if(tree->left ) tree_print_aux(tree->left ,_print_key,depth,'L');
  if(tree->right) tree_print_aux(tree->right,_print_key,depth,'R');
}

void* tree_getKey(tree_t* tree){
  return tree->key;
}

bool tree_for_all(tree_t* tree, void* key, cmpkey _cmpkey,
		  void* data, cmpdata _cmpdata){
  if(tree){
    if(!(_cmpkey(key, tree_getKey(tree)) == MATCH)){
      if(list_for_all(tree_getList(tree), data, _cmpdata)) return true;
    }
    if(tree_for_all(tree->left ,key,_cmpkey,data,_cmpdata)) return true;
    if(tree_for_all(tree->right,key,_cmpkey,data,_cmpdata)) return true;
  }
  return false;
}


void for_all_nodes(tree_t *tree, print _print ,int *tree_index,int page, int ware_choice) 
{
  if(!tree) return;
  if (tree->left != NULL) 
    { 
      for_all_nodes(tree->left, _print, tree_index, page,ware_choice); 
    } 
 
  _print(tree, tree->key, tree_index, page, ware_choice);
        
  if (tree->right != NULL) 
    { 
      for_all_nodes(tree->right, _print,tree_index,page,ware_choice); 
    } 
}
