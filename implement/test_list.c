#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include "util.h"
#include "list.h"
#include "tree.h"

#define ALPHA "ABCDEFGHIJKLMNOPQRSTUVWXYZ������"
#define ALPHA_EX "ABCDEFGHIJKLMNOPQRSTUVWXYZ ,.;:-?!������"

typedef struct ware{
  char* name;
  int price;
  char letter;
} ware_t;

void free_data(void* data){
  // free(((ware_t*)data)->name);
  // free((ware_t*)data);
}

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

void print_list(list_t* list){
  printf("\n==================\n");
  for(int c=1; c<=list_length(list); c++){
    printf("%d. ", c);
    ware_print(list_get(list, c));
  }
}

int main(void){

  list_t* list = list_new(sizeof(ware_t), free_data);
  for(int c=1; c<=6; c++){
    ware_t* ware = ware_new();
    list_append(list, ware);
    free(ware);
    printf("\n%d. ", c);
    ware_print(list_tail(list));
  }

  print_list(list);

  list_remove(list, ask_question_int("Enter a number to remove > ",
				     1, list_length(list)));

  print_list(list);

  list_destroy(list);

  return 0;
}
