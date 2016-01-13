#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <limits.h>
#include "util.h"
#include "tree.h"
#include "../gc.h"

#define ALPHA "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define ALPHA_EX "ABCDEFGHIJKLMNOPQRSTUVWXYZ ,.;:-?!"

heap_t *our_heap;

typedef struct ware{
  char* name;
  char* desc;
  int price;
  int amount;
  int shelf_number;
  char shelf_letter;
} ware_t;

typedef struct shopitem{
  char* name;
  int amount;
  int max;
} shopitem_t;

void add_ware(tree_t** tree);
void remove_ware(tree_t** tree);
void edit_ware(tree_t** tree);
void undo(tree_t** tree);
void list(tree_t** tree);
void shop(tree_t** tree);
bool quit();
void free_ware(void* ware);
void free_name(void* name);

/// ###################################################

int main(int argc, char *argv[]){
  our_heap = h_init(10000, true, 0.0);

  printf("\n===========================================\n\n"
	 "    Vlkommen till Lagerhanteraren 2.0\n");

  tree_t* tree = NULL;
  bool running = true;

  while(running){
    switch(ask_question_char
	   ("\n===========================================\n"
	    "[L]ägg till en vara\n"
	    "[T]a bort en vara\n"
	    "[R]edigera en vara\n"
	    "Lista [h]ela varukatalogen\n"
	    "[P]acka en pall\n"
	    "[A]vsluta\n\n"
	    "Vad vill du göra? > ",
	    "LTRHPA")){
    case 'L': add_ware(&tree); break;
    case 'T': remove_ware(&tree); break;
    case 'R': edit_ware(&tree); break;
    case 'H': list(&tree); break;
    case 'P': shop(&tree); break;
    case 'A': running = quit(); break;
    default : printf("Fel inmatning\n"); break;
    }
  }
  tree_destroy(&tree);
  int collected = h_gc(our_heap);
  printf("Used data: %d\n", h_used(our_heap));
  printf("Collected data: %d\n", collected);
  h_delete(our_heap);

  return 0;
}

/// ###################################################

void free_ware(void* ware){
  return;
}

void free_name(void* name){
  return;
}

void* shelf_collision(void* dest, void* src){
  ((ware_t*)dest)->amount += ((ware_t*)src)->amount;
  return dest;
}

void print_ware(void* ware){
    printf("\n%s%s \n%s%c%d \n%s%s \n%s%d \n%s%d\n",
	 "Namn : ",((ware_t*)ware)->name,
	 "Hylla : ",((ware_t*)ware)->shelf_letter,((ware_t*)ware)->shelf_number,
	 "Beskrivning : ", ((ware_t*)ware)->desc,
	 "Pris : ",((ware_t*)ware)->price,
	 "Antal : ", ((ware_t*)ware)->amount);
    printf("----------------------------\n");
}

void print_all_wares(tree_t *tree,void* str1, int *tree_index,int page,int ware_choice)
{
  *tree_index = *tree_index + 1;
  printf("%d%s",(*tree_index), " : ");
  printf("%s\n", (char*)str1);
}

void print_string_tree(void* str1, int depth, char root){
  if(strlen((char*)str1) < 8) printf("%s\t\t|", (char*)str1);
  else printf("%s\t|", (char*)str1);
  for(int c=0; c<depth; c++) printf(" ");
  printf("%c%d\n", root, depth);
}

void print_string_index(tree_t *tree,void* str1, int *tree_index,int page,int ware_choice)
{
  *tree_index = *tree_index + 1;
  //vre och undre grns fr vad som printas
  if ((((page+1)*20) >=  *tree_index)&&((page*20)<*tree_index))
    {
      printf("%d%s",(*tree_index) - ((page)*20), " : ");
      printf("%s\n", (char*)str1);
    }
}

void print_list_for_index(tree_t *tree, void* str1, int *tree_index,int page,int ware_choice){
  *tree_index = *tree_index + 1;
  if (*tree_index == (ware_choice+(20*page))){
    list_t *list = tree_getList(tree);
    for(int c=1; c <= list_length(list); c++){
      void *data = list_get(list,c);
      print_ware(data);
    }
  }
}

//todo handle upper and lower case
int cmpname(void* name1, void* name2){
  if(name1 == NULL)   return EMPTY;

  int result = strcmp(name1, name2);

  if(result > 0)      return LEFT;
  else if(result < 0) return RIGHT;
  else                return MATCH;
}

//todo test upper and lower case behaviour
bool cmpshelf(void* shelf1, void* shelf2){
  if(((ware_t*)shelf1)->shelf_letter == ((ware_t*)shelf2)->shelf_letter &&
     ((ware_t*)shelf1)->shelf_number == ((ware_t*)shelf2)->shelf_number){
    return true;
  }
  else return false;
}

void print_name(void* name){
  printf("%s\n", (char*)name);
}

/// ###################################################

// \brief Finds out if given ware is ok to insert into tree
bool ware_allowed(tree_t* tree, char* name, ware_t* ware){
  return !tree_for_all(tree, name, cmpname, ware, cmpshelf);
}

void add_ware(tree_t** tree){
  ware_t *ware = h_alloc_struct(our_heap, "**iiic");
  ware->name = ask_question_string("Ange namn > ", ALPHA);
  *ware->name = toupper(*ware->name);
  ware->desc = ask_question_string("Ange beskrivning > ", ALPHA_EX);
  ware->price = ask_question_int("Ange pris > ", 0, INT_MAX);
  ware->amount = ask_question_int("Ange antal > ", 0, INT_MAX);
  ware->shelf_letter = ask_question_char("Ange hyllbokstav > ", ALPHA);
  ware->shelf_number = ask_question_int("Ange hyllnummer > ",0,INT_MAX);
  if(ware_allowed(*tree, ware->name, ware)){
    char* keyName;
    if(tree_find(*tree, ware->name, cmpname))
      keyName = ware->name;
    else{
      keyName = h_alloc_data(our_heap, sizeof(char)*(strlen(ware->name)+1));
      strcpy(keyName, ware->name);
    }
    if(!tree_insert(tree, keyName, cmpname, ware, cmpshelf,
		    sizeof(ware_t), free_ware, free_name,
		    shelf_collision)){
      //free(ware->name);
      //free(ware->desc);
    }
  }
  else{
    printf("Lagerplatsen är upptagen.\n");
    //  free(ware->name);
    //free(ware->desc);
  }
  //free(ware);
}

/// ###################################################

void remove_ware(tree_t** tree){
  char* str = ask_question_string("Vad vill du ta bort? > ", ALPHA_EX);
  tree_t* found = tree_find(*tree, str, cmpname);

  if(found){
    if(list_length(tree_getList(found)) > 1){
      for(int c=1; c<= list_length(tree_getList(found)); c++){
	printf("%d. ", c);
	print_ware(list_get(tree_getList(found), c));
      }
      printf("0. Ta bort alla.\n");
      printf("-1. Avbryt.\n");
      int ans = ask_question_int("Vad vill du ta bort? > ",-1,INT_MAX);
      if(ans == -1){
	printf("Borttagning avbruten.\n");
      }
      else if(ans == 0){
	*tree = tree_remove(*tree, str, cmpname);
	printf("Tog bort alla varor med namnet %s.\n", str);
      }
      else{
	list_remove(tree_getList(found), ans);
	printf("Tog bort en vara med namnet %s.\n", str);
      }
    }
    else{
      *tree = tree_remove(*tree, str, cmpname);
      printf("Tog bort varan %s.\n", str);
    }
  }
  else printf("Kunde inte ta bort varan %s.\n", str);

  //free(str);
}

/// ###################################################

ware_t* make_new_ware(tree_t* tree, tree_t* itemtree, int index){
  ware_t* existing_ware = list_get(tree_getList(itemtree), index);
  ware_t* new_ware = h_alloc_struct(our_heap, "**iiic");
  memcpy(new_ware, existing_ware, sizeof(ware_t));

  bool run = true;
  while(run){

    printf("\n");
    print_ware(existing_ware);
    printf("Vad vill du ändra?\n");
    printf("[B]eskrivning\n");
    printf("[P]ris\n");
    printf("[A]ntal\n");
    printf("[H]yllplats\n");
    printf("A[v]sluta\n");
    char choice = ask_question_char("> ", "BPAHV");

    switch(choice){
    case 'B': //desc
      if(!strcmp(new_ware->desc, existing_ware->desc))
	new_ware->desc = ask_question_string("Ange ny beskrivning > ", ALPHA_EX);
      else printf("Fel, redan redigerat beskrivningen.\n");

      break;
    case 'P':
      if(new_ware->price == existing_ware->price)
	new_ware->price = ask_question_int("Ange nytt pris > ", 0, INT_MAX);
      else printf("Fel, redan redigerat priset.\n");
      break;
    case 'A':
      if(new_ware->amount == existing_ware->amount)
	new_ware->amount = ask_question_int("Ange nytt antal > ", 0, INT_MAX);
      else printf("Fel, redan redigerat antalet.\n");
      break;
    case 'H':
      if(new_ware->shelf_letter == existing_ware->shelf_letter &&
	 new_ware->shelf_number == existing_ware->shelf_number){
	new_ware->shelf_letter = ask_question_char("Ange ny hyllbokstav > ", ALPHA);
	new_ware->shelf_number = ask_question_int("Ange nytt hyllnummer > ", 0, INT_MAX);
      }
      else printf("Fel, redan redigerat hyllplatsen.\n");
      break;
    case 'V':
      run = false;
      break;
    }
  }

  if(ware_allowed(tree, new_ware->name, new_ware)){
    char* new_name = h_alloc_data(our_heap, sizeof(char)*(strlen(existing_ware->name)+1));
    strcpy(new_name, existing_ware->name);
    new_ware->name = new_name;
    return new_ware;
  }
  else{
    return NULL;
  }
}

void edit_ware(tree_t** tree){
  char* ans = ask_question_string("Vilken vara vill du ändra? > ", ALPHA);

  tree_t* itemtree = tree_find(*tree, ans, cmpname);
  if(itemtree){
    for(int c=1; c<= list_length(tree_getList(itemtree)); c++){
	printf("%d. ", c);
	print_ware(list_get(tree_getList(itemtree), c));
      }
    printf("0. Avbryt\n");
    int val = ask_question_int("Vilket nr vill du ändra? > ", 0,
			       list_length(tree_getList(itemtree)));
    if(val){
      ware_t* new_ware = make_new_ware(*tree, itemtree, val);
      if(new_ware){
	list_remove(tree_getList(itemtree), val);
	list_append(tree_getList(itemtree), new_ware);
	printf("Redigerad vara:\n");
	print_ware(list_tail(tree_getList(itemtree)));
      }
      else{
	printf("ändring misslyckades\n");
      }
    }
    else printf("Avbryter.\n\n");
  }
  else printf("Varan finns inte.\n\n");

  //free(ans);
}

/// ###################################################

void undo(tree_t** tree){
  printf("To be implemented\n");
}

/// ###################################################

void list(tree_t** tree)
{
  printf("\n");
  int page = 0;
  char choice = 1;
  int amountofwares = tree_nodes(*tree);
  int h = 0;
  int *tree_index = &h;
  int ware_choice;

  while (choice != 'G' && choice != 'g')
    {
      h = 0;
     for_all_nodes(*tree, print_string_index, tree_index, page, ware_choice);

     if (amountofwares >((1+page)*20))
       {
	 choice = ask_question_char("\n[N]sta sida, [g] tillbaka eller [v]lj vara > ", "NnGgVv");
	 printf("\n");
       }
     else
       {
	 choice = ask_question_char("\n[G] tillbaka  eller [v]lj vara > ", "GgVv");
       }
     if (choice == 'N' || choice == 'n')
       {
       page++;
       }
     if (choice == 'V' || choice == 'v')
       {
	 ware_choice = ask_question_int ("\nVlj vara genom att mata in index > ", 1, 20);
	 h = 0;
	 for_all_nodes(*tree, print_list_for_index, tree_index,page,ware_choice);
	 choice = ask_question_char( "\nVill du [f]ortstta eller [g] tillbaka?", "FfGg");
       }
    }

}

/// ###################################################

void shopitem_free(void* shopitem){
  return;
}

void shop_print_list(list_t* shoplist){
  if(list_length(shoplist)){
    printf("Din pall innehller:\n");
    for(int c=1; c<=list_length(shoplist); c++){
      shopitem_t* item = list_get(shoplist, c);
      printf("  - %s (%d)\n", item->name, item->amount);
    }
    printf("\n");
  }
  else{
    printf("Din pall r tom.\n\n");
  }
}

void shop_take(shopitem_t** item, ware_t** ware){
  if((*item)->amount < (*ware)->amount){
    (*ware)->amount -= (*item)->amount;
    (*item)->amount = 0;
  }
  else{
    (*item)->amount -= (*ware)->amount;
    (*ware)->amount = 0;
  }
}


void shop_pack(tree_t** tree, list_t* shoplist){
  if(!list_length(shoplist)) return;

  printf("Lagerplatser fr att packa pallen:\n");

  int total_cost = 0;
  while(list_length(shoplist) > 0){
    shopitem_t* iteminfo = list_get(shoplist, 1);
    tree_t* itemtree = tree_find(*tree, iteminfo->name, cmpname);

    while(iteminfo->amount > 0){
      int index = 1, value = 0;
      for(int n=1; n<=list_length(tree_getList(itemtree)); n++){
	if(((ware_t*)list_get(tree_getList(itemtree),n))->amount>value){
	  index = n;
	  value = ((ware_t*)list_get(tree_getList(itemtree),n))->amount;
	}
      }

      ware_t* ware = list_get(tree_getList(itemtree), index);
      shop_take(&iteminfo, &ware);
      value -= ware->amount;
      total_cost += value * ware->price;

      printf("%c%d\t(%d %s)\n", ware->shelf_letter, ware->shelf_number,
	     value, ware->name);

      if(ware->amount == 0 && list_length(tree_getList(itemtree)) > 1){
	list_remove(tree_getList(itemtree), index);
      }
      else if(ware->amount == 0){
	*tree = tree_remove(*tree, tree_getKey(itemtree), cmpname);
      }
    }

    list_remove(shoplist, 1);
  }

  printf("Totalt pris fr pallen: %d kr\n\n", total_cost);
}

// [x] Buid the ui & exit command
//   [x] Display current cart
//   [x] List wares in database
//   [x] Ask what ware to put in cart
//     [x] Display name and amount (total)
//     [x] Ask for amount to put in cart
//     [x] Figure out where to take from (if multiple locations)
//   [x] Ask if user want to add additional wares
// [x] Hide wares allready in cart
// [x] When done, display cart once again. Additionally,
// [ ] Enable use of pages
// [x] print out "location (amount, name)" for each ware.
// [x] Lastly, print out price
// * list decrease amount in given location

void shop(tree_t** tree){
  list_t* shoplist = list_new(sizeof(shopitem_t), shopitem_free);
  bool run = true;
  int tree_index = 0;

  while(run){
    shop_print_list(shoplist);
    tree_index = 0;
    for_all_nodes(*tree, print_all_wares, &tree_index,0,0);

    shopitem_t item;
    item.name = ask_question_string("\nAnge vara > ", ALPHA);
    item.max = 0;

    if(tree_find(*tree, item.name, cmpname)){
      int max_value = 0;

      //added the item before?
      for(int c=1; c<=list_length(shoplist); c++){
	if(!strcmp(((shopitem_t*)list_get(shoplist, c))->name,
		   item.name)){
	  max_value += ((shopitem_t*)list_get(shoplist, c))->amount;
	}
      }

      for(int c=1; c<=list_length(tree_getList(*tree)); c++){
	item.max += ((ware_t*)list_get(tree_getList(tree_find(*tree, item.name, cmpname)), c))->amount;
      }
      item.max -= max_value;
      printf("Ange antal (0 - %d) ", item.max);
      item.amount = ask_question_int("> ", 0, item.max);
      list_append(shoplist, &item);
      printf("Varan packad.\n\n");
    }
    else printf("Varan finns ej.\n");

    switch(ask_question_char("\nVill du packa en till vara? [J]a/[N]ej > ",
			     "JN")){
    case 'J': break;
    case 'N': run = false; break;
    }
    printf("\n");
  }

  shop_pack(tree, shoplist);

  list_destroy(shoplist);
}

bool quit(){
  //  printf("Total memory: %zu\n", global_counter);
  switch(ask_question_char("Sker att du vill avsluta? [J]a/[N]ej > ", "JN")){
  case 'J':
    h_delete(our_heap);
    return false;
  default :
    return true;
  }
}
