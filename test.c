#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include "util.h"
#include "list.h"
#include "tree.h"

// https://github.com/IOOPM-UU/ioopm15/blob/master/forelasningar/L8/f8.pdf

#define ALPHA "ABCDEFGHIJKLMNOPQRSTUVWXYZÅÄÖåäö"
#define ALPHA_EX "ABCDEFGHIJKLMNOPQRSTUVWXYZ ,.;:-?!ÅÄÖåäö"

//#define TEST_CHAR
//#define TEST_INT
//#define TEST_STRING
//#define TEST_LIST
#define TEST_TREE

typedef struct ware{
  char* name;
  int price;
  char letter;
} ware_t;

#if defined(TEST_LIST) || defined(TEST_TREE)

void free_ware(void* ware){
  free(((ware_t*)ware)->name);
  free((ware_t*)ware);
}

void free_name(void* name){
  free((char*)name);
}

/*
void free_data_f(void* data){
  free((int*)data);
}

void free_key_f(void* data){
  free((int*)data);
}
*/

ware_t* ware_new(){
  ware_t* ware = malloc(sizeof(ware_t));
  ware->name =   ask_question_string("Enter name > ", ALPHA);
  ware->price =  ask_question_int("Enter price > ", INT_MIN, INT_MAX);
  ware->letter = ask_question_char("Enter letter > ", ALPHA);
  return ware;
}

void ware_print(void* data){
  printf("%s\t%d\t%c\n", ((ware_t*)data)->name,
	 ((ware_t*)data)->price, ((ware_t*)data)->letter);
}

int compare_ints(void* num1, void* num2){
  if(num1 == NULL) return EMPTY;
  if(*(int*)num2 < *(int*)num1)  return LEFT;
  if(*(int*)num2 > *(int*)num1)  return RIGHT;
  else return MATCH;
}

bool compare_data_f(void* data1, void* data2){
  return false;
}

void print_int_tree(void* int1, int depth, char root){
  printf("%d\t|", *(int*)int1);
  for(int c=0; c<depth; c++) printf(" ");
  printf("%c%d\n", root, depth);
}

void print_int(void* int1){
  printf("%d\n", *(int*)int1);
}

void print_int_2(void* int1, int *tree_index,int page)
{ 
  *tree_index = *tree_index + 1;
  //övre och undre gräns för vad som printas 
  if ((((page+1)*20) >=  *tree_index)&&((page*20)<*tree_index))
    { 
      printf("%d%s",(*tree_index) - ((page)*20), " : ");
      printf("%d\n", *(int*)int1);
    }
} 

void print_string_tree(void* str1, int depth, char root){
  if(strlen((char*)str1) < 8) printf("%s\t\t|", (char*)str1);
  else printf("%s\t|", (char*)str1);
  for(int c=0; c<depth; c++) printf(" ");
  printf("%c%d\n", root, depth);
}

//todo handle upper and lower case
int cmpname(void* name1, void* name2){
  if(name1 == NULL)   return EMPTY;

  int result = strcmp(name1, name2);

  if(result > 0)      return LEFT;
  else if(result < 0) return RIGHT;
  else                return MATCH;
}

bool cmpshelf(void* shelf1, void* shelf2){
  return false;
}

void print_name(void* name){
  printf("%s\n", (char*)name);
}

#endif

int main(void){

  #ifdef TEST_CHAR
  while(true) printf("%c\n", ask_question_char("Enter something: ", ALPHA));
  #endif

  #ifdef TEST_INT
  while(true) printf("%d\n", ask_question_int("Ange något mellan -1337 och 1337: ", -1337, 1337));
  #endif

  #ifdef TEST_STRING
  while(true) printf("msg:%s\n", ask_question_string("Enter your name: ", ALPHA));
  #endif

  #ifdef TEST_LIST
  list_t* list = list_new(sizeof(ware_t), free_data);
  for(int c=1; c<=6; c++){
    ware_t* ware = ware_new();
    list_append(list, ware);
    free(ware);
    printf("\n%d. ", c);
    ware_print(list_tail(list));
  }
  
  printf("\nBefore\n");
  for(int c=1; c<=list_length(list); c++){
    printf("%d. ", c);
    ware_print(list_get(list, c));
  }

  list_remove(list, ask_question_int("Enter a number to remove > ", 1, list_length(list)));
  
  printf("\nAfter\n");
  for(int c=1; c<=list_length(list); c++){
    printf("%d. ", c);
    ware_print(list_get(list, c));
  }
  
  list_destroy(list);
  #endif


  #ifdef TEST_TREE
  tree_t* tree = NULL;

  for(int c=0; c<6; c++){
    ware_t* ware = ware_new();
    //list_append(list, ware);
    char* key = malloc(sizeof(char)*(strlen(ware->name)+1));
    strcpy(key, ware->name);
    tree_insert(&tree, key, cmpname, ware, cmpshelf, 
		sizeof(ware_t), free_ware, free_name);
    printf("\n.%s .%c .%d\n", ware->name, ware->letter, ware->price);
    free(ware);
  }

  printf("\n-------------------\ndepth:%d",tree_depth(tree));
  printf("  \tnodes:%d\n", tree_nodes(tree));
  tree_print(tree, print_string_tree);

  
  char* keyToRemove = ask_question_string("Enter key to be removed > ", ALPHA);
  tree_remove(tree, keyToRemove, cmpname);
  free(keyToRemove);
  printf("\n-------------------\nTree after remove\n");
  printf("\n-------------------\ndepth:%d",tree_depth(tree));
  printf("  \tnodes:%d\n", tree_nodes(tree));
  tree_print(tree, print_string_tree);  

  tree_destroy(&tree);
  #endif

  return 0;
}
