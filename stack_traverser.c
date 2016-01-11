#include "stack_traverser.h"
#include <string.h>

#define Dump_registers()			\
  jmp_buf env;					\
  if (setjmp(env)) abort();			\

void *get_stack_top() {
  int top_of_stack;
  return &top_of_stack;
}

ll_head get_alive_stack_pointers(heap_t *h, void *top, void *bottom) {

  Dump_registers();
  ll_node **root = LL_initRoot();

  if (top > bottom) {
    while (top > bottom) {
      void **content = top;
      if (validate_object(*content, h)) { // checks the pointers metadata to check whether it's pointing at an object or not
        LL_createAndInsertSequentially(root, *content);
        total++;
      }
      --top;
    }
  } else {
    while (top < bottom) {
      void **content = top;
      if (validate_object(*content, h)) { // checks the pointers metadata to check whether it's pointing at an object or not
        ll_node *stackTop = LL_createAndInsertSequentially(root, *content);
        total++;
      }
      ++top;
    }
  }
  return root;
}

void print_address(void *listobject){
  printf("\n%d\n", listobject);
}

void print_stack_list(ll_node **list) {
  puts("Printing list of alive stackpointers:");
  LL_map(list, print_address);
}

/*
void print_stack_list(ll_node **root) {
  puts("Printing list of alive stackpointers:");
  ll_node *iterator = *root;
  int counter = 0;

  while (iterator) {
    printf("\nCounter: %d\n%ld\n", counter, *(uintptr_t *)LL_getContent(iterator));
    counter++;
    if (LL_getPrevious(root, iterator)) {
      printf("%p has previous pointer at %p\n", LL_getContent(iterator), LL_getContent(LL_getPrevious(root, iterator)));
      printf("Difference between this pointer %ld and it's previous pointer %ld is %ld\n", *(uintptr_t *)LL_getContent(iterator), *(uintptr_t *)LL_getContent(LL_getPrevious(root, iterator)), (*(uintptr_t *)LL_getContent(iterator) - *(uintptr_t *)LL_getContent(LL_getPrevious(root, iterator))));
    }
    if (LL_getNext(iterator))
      printf("%p has next pointer at %p\n", LL_getContent(iterator), LL_getContent(LL_getNext(iterator)));
    iterator = LL_getNext(iterator);
  }
}
*/

typedef struct haubir haubir_;

struct haubir {
  char *LeBron;
  int amazing;
};

void set_haubir_string(char *string, haubir_ *to_modify) {
  strcpy(to_modify->LeBron,string);
}

void set_haubir_int(int number, haubir_ *to_modify) {
  to_modify->amazing = number;
}

/*
int main() {
   // create a new heap
   heap_t *new_heap = h_init(1024, true, 100.0);

   // allocate on this heap. For testing purposes
   // haubir_ *ptr = h_alloc_struct(new_heap, "cccci");
   // set_haubir_string("heej", ptr);
   // set_haubir_int(45, ptr);

   char *boll = "malin";
   boll = h_alloc_data(new_heap, 10);

   //haubir_ *malin = h_alloc_struct(new_heap, "*i");
   char *malin = "malin";
   malin = h_alloc_data(new_heap, 10);

   char *string = "heej";
   string = h_alloc_data(new_heap, 100);

   char *ptr = "ptr";
   ptr = h_alloc_data(new_heap, 100);
   printf("Allocated pointer: %ld\n", (uintptr_t)ptr);

   // printf("Address on stack:  %p\n", ptr);

   //  is_pointing_at_heap(ptr, new_heap);

   // Dump pointers from the registers to the stack, if any.
   Dump_registers();

   printf("hej där\n");

   // the list contains all alive pointers
   ll_node **root = get_alive_stack_pointers(new_heap);

   printf("haubir_ *malin: %ld\n", (uintptr_t)malin);
   printf("char *string: %ld\n", (uintptr_t)string);
   printf("void *ptr: %ld\n", (uintptr_t)ptr);
   printf("char *string: %ld\n", (uintptr_t)boll);

   // ll_node *bajs = LL_getNodeAtIndex(root, 2);
   // char *bajstest = (char *)LL_getContent(bajs);
   // if (bajstest) {
   //   printf("Bajstest: %s", bajstest);
   //   char *outprint = strdup(bajstest);
   //   printf("Outprint: %s", outprint);
   // }
   //haubir_ *strukt = LL_getContent(bajs);
   //haubir_ bajsar = *strukt;
   //printf("Bajsar: %s\nBajsar: %d\n", bajsar.LeBron, bajsar.amazing);
   // print the list for debugging purposes
   print_stack_list(root);


   // deletes the heap we created
   h_delete(new_heap);

   return 0;
}
*/
