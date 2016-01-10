#include "stack_traverser.h"

#define Dump_registers()			\
  jmp_buf env;					\
  if (setjmp(env)) abort();			\

extern void **environ; // bottom of the stack

bool is_pointing_at_heap(void *ptr, heap_t *h/* , size_t bytes */) { 
  long *content = (long *)ptr;
  puts("is_pointing_at_heap är startad.");
  printf("Pointer contains: %ld\n", *(uintptr_t *)ptr);
  printf("Address on stack: %p\n", ptr);

  if (*(uintptr_t *)ptr >= (uintptr_t)get_heap_start(h) && *(uintptr_t *)ptr < (uintptr_t)get_heap_end(h)) {
    puts("Yes!!!!\n\n\n\n\n\n\n\n\n");
    return true;
  }
  else
    return false;
}

void *get_stack_top() {
  int top_of_stack;
  return &top_of_stack;
}

ll_node **get_alive_stack_pointers(heap_t *h) {
  printf("du är i get_alive_stack_pointers\n");
  void *top = get_stack_top();
  ll_node **root = LL_initRoot();

  // puts("Welcome to build_stack_list");
  int total = 0;
  int counter = 0;
  //  void *bottom = (void *)environ;

  if ((uintptr_t)top > (uintptr_t)environ) {
    puts("Stack grows upwards.");
    while ((uintptr_t *)top > (uintptr_t *)environ) {
      printf("Count %d: \ntop: %p\n", counter, top);
      if (is_pointing_at_heap(top, h)) {
	if (validate_object(top)) { // checks the pointers metadata to check whether it's pointing at an object or not
	  void **content = top;
	  ll_node *added_node = LL_createAndInsertSequentially(root, *content);
	  //printf("stackTop: %p\n", stackTop);,
	  printf("Stackpointer %p has valid metadata and was added to the list.\n", LL_getContent(added_node));
	  total++;
	}
      }
      --top;// -= sizeof(void *);
      counter++;
    }
  }
  else {
    printf("hej igen, stacken borde växa neråt nu va\n");
    while ((uintptr_t)top < (uintptr_t)environ) {
      puts("Stack grows downwards.");
      printf("Count %d: \ntop: %p\n", counter, top);
      if (is_pointing_at_heap(top, h)) {
	if (validate_object(top)) { // checks the pointers metadata to check whether it's pointing at an object or not
	  void **content = top;
	  ll_node *added_node = LL_createAndInsertSequentially(root, *content);
	  //printf("stackTop: %p\n", stackTop);,
	  printf("Stackpointer %p has valid metadata and was added to the list.\n", LL_getContent(added_node));
	  total++;
	}
      }
      ++top;// += sizeof(void *);
      counter++;
    }
  }
  puts("\n--------------------------------\n");
  printf("Total pointers added to the list: %d\n", total);
  puts("\n--------------------------------\n");
  //  create_stack_list(top, h, root);
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
    printf("\nCounter: %d\n%016\n", counter, (uintptr_t *)LL_getContent(iterator));
    counter++;
    if (LL_getPrevious(root, iterator)) {
      printf("%ld has previous pointer at %ld\n", *(uintptr_t *)LL_getContent(iterator), *(uintptr_t *)LL_getContent(LL_getPrevious(root, iterator)));
      printf("Difference between this pointer %ld and it's previous pointer %ld is %ld bytes\n", *(uintptr_t *)LL_getContent(iterator), *(uintptr_t *)LL_getContent(LL_getPrevious(root, iterator)), (*(uintptr_t *)LL_getContent(iterator) - *(uintptr_t *)LL_getContent(LL_getPrevious(root, iterator))));
    }
    if (LL_getNext(iterator))
      printf("%ld has next pointer at %ld\n", *(uintptr_t *)LL_getContent(iterator), *(uintptr_t *)LL_getContent(LL_getNext(iterator)));
    iterator = LL_getNext(iterator);
  }
}
*/ 
typedef struct haubir haubir_;

struct haubir {
  char *LeBron;
  int amazing;
};

void print_string(char *string) {
  printf("String: %s\n", string);
}


int main() {
  // create a new heap
  heap_t *new_heap = h_init(1024, true, 100.0);
  
  // allocate on this heap. For testing purposes
  // haubir_ *ptr = h_alloc_struct(new_heap, "cccc");
  // haubir_ *malin = h_alloc_struct(new_heap, "*i");

  // char *string = h_alloc_data(new_heap, 1);
  // strcpy(string, "heej");
  char *number = h_alloc_data(new_heap, 10);
  *number = 'c';

  // printf("Address on stack:  %p\n", ptr);
  
  // is_pointing_at_heap(ptr, new_heap);
  
  // Dump pointers from the registers to the stack, if any.
  Dump_registers();
  
  printf("hej där\n");
  
  // the list contains all alive pointers
  ll_node **root = get_alive_stack_pointers(new_heap);

  printf("First allocated pointer: %ld\n", (uintptr_t)number);
  printf("Number: %c\n", *number);
  // printf("String: %s\n", string);
  // print the list for debugging purposes
  print_stack_list(root);

  // Avreferera roten så får du en pekare till dess nod
  ll_node *print_node = *root;

  // Avreferera nodepekaren så får du en pekare till dess innehåll (heapaddressen)
  long *print_node_content = LL_getContent(print_node);
  
  char *c = (char *)print_node_content;

  printf("Content of heap pointer %ld is %c\n", print_node_content, *c);
  // deletes the heap we created
  h_delete(new_heap);
  
  return 0;
}
