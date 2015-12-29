/* 
- kolla igenom alla stackpekare
- kolla metadatan för varje objekt som pekas på
- om metadata finns så ska pekaren skickas vidare till heaptraverseringen
- sålla bort pekare som inte pekar på något
- returnera alla giltliga pekare till heaptraverseringen i form av en länkad lista.
*/

/*
  Kompilera med:
  gcc -std=c11 -Wall -ggdb -o stack_traverser stack_traverser.c

  tills vidare.
*/

#include "stack_traverser.h"

#define Dump_registers()			\
  jmp_buf env;					\
  if (setjmp(env)) abort();			\

extern char **environ; // bottom of the stack

void fuckOff(){}
void endiannessTestAux(){}

void endiannessTest() { // this function investigates whether the stack grows upwards or downwards.
  int stack;
  void *heap = malloc(1);
  void *heap2 = malloc(1);
  Dump_registers();
  puts("");
  printf("printf:             %15p\n", printf);
  printf("puts:               %15p\n", puts);
  printf("fuckOff:            %15p\n", fuckOff);
  printf("endiannessTestAux:  %15p\n", endiannessTestAux);
  printf("endiannessTest:     %15p\n", endiannessTest);
  printf("heap:     %15p\n", heap);
  printf("heap2:    %15p\n", heap2);
  printf("stack:    %15p\n", &stack);
  int stack2;
  printf("stack2:   %15p\n", &stack2);
}

bool heap_grows_upwards() {
  void *first = malloc(1);
  void *second = malloc(1);
  if (first < second) {
    free(first);
    free(second);
    return true;
  }
  else {
    free(first);
    free(second);
    return false;
  }  
}

void allocate_on_heap() {
  
}

bool is_pointing_at_heap(void *ptr, heap_t *h) {
  if (heap_grows_upwards()) {
    if (ptr > h->user_start_p && ptr < h->end_p) {
      return true;
    }
    else {
      return false;
    }
  }
  else {
    if (ptr < h->user_start_p && ptr > h->end_p) {
      return true;
    }
    else {
      return false;
    }
  }
}

void *get_stack_top() {
  int top_of_stack;
  int *top = &top_of_stack;
  return top;
}

bool stack_grows_from_top() {
  int stack1;
  int stack2;

  if (&stack1 < &stack2) {
    printf("stack_grows_from_top == true\n");
    return true;
  }
  else {
    printf("stack_grows_from_top == false\n");  
    return false;
  }
}

void build_stack_list(int flag, void *top, heap_t *h, ll_node **root) {
  puts("Welcome to create_stack_list");
  int counter = 0;
  if (flag == 0) {
    while (top > environ) {
      printf("top: %p\n", top);
      if (is_pointing_at_heap(top, h)) {
	if (validate_object(top)) { // checks the pointers metadata to check whether it's pointing at an object or not
	  ll_node *stackTop = LL_createAndInsertSequentially(root, top);
	  //printf("stackTop: %p\n", stackTop);,
	  printf("Count %d: New stackpointer %p has valid metadata and was added to the list.\n", counter, stackTop->nodeContent);
	}
      }
      top -= sizeof(void *);
      counter++;
    }
  }
  else {
    while (top < environ) { 
      puts("Stack grows downwards.\n");
      printf("top: %p\n", top);
      if (is_pointing_at_heap(top, h)) {
	if (validate_object(top)) { // checks the pointers metadata to check whether it's valid or not
	  ll_node *stackTop = LL_createAndInsertSequentially(root, top);
	  //printf("stackTop: %p\n", stackTop);,
	  printf("Count %d: New stackpointer %p has valid metadata and was added to the list.\n", counter, stackTop->nodeContent);
	}
      }
      top += sizeof(void *);
      counter++;
    }
  }
}

ll_node **traverse_stack_list(heap_t *h) {
  printf("du är i traverse stack list\n");
  void *top = get_stack_top();
  ll_node **root = LL_initRoot();
  int counter = 0;

  if (!stack_grows_from_top()) {
    puts("Stack grows upwards.\n");
    // prinft((*(int *)top,*(int *)environ)
    build_stack_list(0, top, h, root);
  }
  
  else { 
    printf("hej igen, stacken borde växa neråt nu va\n");
    build_stack_list(1, top, h, root);
  }  
  return root;  
}

void print_stack_list(ll_node **root) {
  puts("Printing list of alive stackpointers:");
  ll_node *iterator = *root;
  int counter = 0;
  
  while (iterator) {
    printf("\nCounter: %d\n%p\n", counter, iterator->nodeContent);
    counter++;
    if (iterator->previous) {
      printf("%04x has previous pointer at %04x\n", iterator->nodeContent, iterator->previous->nodeContent);
      printf("Difference between this %04x and %04x is %04x\n", iterator->nodeContent, iterator->previous->nodeContent, (iterator->nodeContent - iterator->previous->nodeContent));
    }
    if (iterator->next)
      printf("%04x has next pointer at %04x\n", iterator->nodeContent, iterator->next->nodeContent);
    iterator = iterator->next;
  }
}


int main() {
  // create a new heap
  heap_t *new_heap = h_init(1024, true, 100.0);
  
  // allocate on this heap. For testing purposes
  void *ptr = h_alloc_struct(new_heap, "***i"); 

  // Dump pointers from the registers to the stack, if any.
  Dump_registers();
  
  printf("hej där\n");

  // the list contains all alive pointers
  ll_node **root = traverse_stack_list(new_heap); 

  // print the list for debugging purposes
  print_stack_list(root); 

  // Just a test to see how the stack and the heap grow on the platform we're on
  endiannessTest();

  // deletes the heap we created
  h_delete(new_heap);
  
  return 0;
}
