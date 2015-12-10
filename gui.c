#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "gc.h"
#include "heap.h"
#include "utilities.h"

#define ARRAY_SIZE(x) (sizeof(x) / sizeof((x)[0]))

// heap global variable
extern heap_t* heap_p = NULL;

typedef struct menu_item{
  // name: Menu item name.
  char name[50]; 
  // key: Character to select this item with. Will default to numer this
  // item has in the list if key is '\0' (empty char).
  char key;
  // function: A function pointer which points to the function that
  // the menu item will call.
  void(*function)();
} menu_item_t;


void menu_init_heap() {
  printf("-- Initializing heap --\n");
  if(heap_p == NULL) {
    int bytes;
    bool unsafe_stack;
    float gc_threshold;
    inputInt("Size of heap (in bytes): ", &bytes);
    inputBool("Unsafe stack (true or false): ", &unsafe_stack);
    inputFloat("Garbage collection threshold (Between 0.0 and 1.0): ", &gc_threshold);

    heap_p = h_init(bytes, unsafe_stack, gc_threshold);
  } else {
    printf("Heap already initialized, need to delete before initializing\n");
  }
}

void menu_allocate_raw() {
  printf("-- Allocating raw --\n");
  
}

void menu_allocate_format_string() {
  printf("-- Allocating format string --\n");
}

void menu_allocate_union() {
  printf("-- Allocating union --\n");
  printf("Not yet implemented\n");
}

void menu_trigger_gc() {
  printf("-- Triggering garbage collection --\n");
}

void menu_delete_heap() {
  printf("-- Deleting heap --\n");
  h_delete(heap_p);
  heap_p = NULL;
}

void menu_exit() {
  printf("-- Goodbye --\n");
}
// Displays menu.
// \param item_array An array of menu items which will be listed and chosen by
// \param item_array_size The amount of elements in item_array
// \return The menu item chosen. 
int menu(menu_item_t item_array[], int item_array_size) {
  int first_item_number = 0;

  for(int i = 0; i < item_array_size; i++) {
    (item_array[i].key == '\0') ?
      printf("[%d]", i + first_item_number) : printf("[%c]", item_array[i].key);
    printf(": %s\n",  item_array[i].name);
  }
  printf("\n");
  
  char input;
  inputChar("Choose menu item", &input);

  printf("\n");
  
  for(int i = 0; i < item_array_size + first_item_number; i++) {
    if(input == item_array[i].key) {
      (*item_array[i].function)();
      return i;
    } else if(item_array[i].key == '\0' && input == i + first_item_number) {
      (*item_array[i].function)();
      return i;
    } 
  }
  return -1;
}
 
void print_heap() {
  int print_width = 75;

  if(heap_p != NULL) {
    printf("-- Heap --\n");

    int cursor_loc = 0;
    
    printf("[");
    // print meta space
    while(cursor_loc < ((intptr_t) heap_p->user_start_p - (intptr_t) heap_p)
	  * print_width / ((intptr_t) heap_p->end_p - (intptr_t) heap_p)  &&
	  cursor_loc < print_width) {
      printf("#");
      cursor_loc++;
    }
    // print allocated space
    while(cursor_loc < ((intptr_t) heap_p->bump_p - (intptr_t) heap_p)
	  * print_width / ((intptr_t) heap_p->end_p - (intptr_t) heap_p)  &&
	  cursor_loc < print_width) {
      printf("#");
      cursor_loc++;
    }
    // print free space
    while(true && cursor_loc < print_width) {
      printf("-");
      cursor_loc++;
    }
    printf("]\n");
      
    printf("Meta pointer:\t\t\t%p\n", heap_p->meta_p);
    printf("User start pointer:\t\t%p\n", heap_p->user_start_p);
    printf("Bump pointer:\t\t\t%p\n", heap_p->bump_p);
    printf("End pointer:\t\t\t%p\n", heap_p->end_p); 
    printf("Total size:\t\t\t%zu\n", heap_p->total_size);
    printf("User size:\t\t\t%zu\n", heap_p->user_size);
    printf("Available space:\t\t%zu\n", heap_p->avail_space);
    printf("Unsafe stack:\t\t\t%s\n", heap_p->unsafe_stack ? "trprintf boprintf boue" : "false");
    printf("Garbage collection threshold\t%f\n", heap_p->gc_threshold);
  } else {
    printf("-- Heap not initialized --\n");
  }
  printf("\n");
}

/************************************/
/*                                  */
/*  MAIN                            */
/*                                  */
/************************************/

int main(int argc, char *argv[])
{  
  heap_t my_heap;
  
  menu_item_t item_array[] =
    {{"Initialize heap", 'I', *menu_init_heap},
     {"Allocate (format string)", 'F', *menu_allocate_format_string},
     {"Allocate (raw)", 'R', *menu_allocate_raw},
     {"Allocate (union)", 'U', *menu_allocate_union},
     {"Trigger garbage collection", 'T', *menu_trigger_gc},
     {"Delete heap", 'D', *menu_delete_heap},
     {"Exit", 'E', *menu_exit}};  

  int exit_value = 6;
  
  do {
    print_heap(my_heap);
  } while(menu(item_array, ARRAY_SIZE(item_array)) != exit_value);
  
  return 0;
}


