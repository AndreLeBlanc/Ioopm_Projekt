#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <unistd.h>
#include <setjmp.h>

#define Dump_registers()			\
  jmp_buf env;					\
  if (setjmp(env)) abort();			\

extern char **environ;

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

void check(int depth) {
  int *i;
  char *c;
  char *ptr = malloc(1);
  printf("stack at %p, heap at %p\n", c, &ptr);
  c = ptr;
  printf("c = ptr;\nstack at %p, heap at %p\n\n", c, &ptr);
  if (depth == 0)
    return;
  else
    check(depth - 1);
}

void menu() {
  int choice;
  printf("How deep u wanna go nigg?\n");
  scanf("%d", &choice);
  check(choice);
    return;
}

void *get_stack_top() {
  int top_of_stack;
  return &top_of_stack;
}

typedef struct haubir_ haubir;

struct haubir_{
  char *malin;
  int LeBron;
  void *marco;
  void *two;
} __attribute__((aligned));

int main() {
  printf("Sizeof(Void pekare): %ld\n", sizeof(void *));
  printf("Sizeof(Char pekare): %ld\n", sizeof(char *));
  printf("Sizeof(Int pekare): %ld\n", sizeof(int *));
  printf("Sizeof(Void): %ld\n", sizeof(void));
  printf("Sizeof(Char): %ld\n", sizeof(char));
  printf("Sizeof(Int): %ld\n", sizeof(int));

  printf("\nSizeof(Haubir): %ld\n", sizeof(haubir));
  haubir ny;
  printf("Sizeof(Ny): %ld\n", sizeof(ny));
  printf("Sizeof(Char *malin): %ld\n", sizeof(ny.malin));
  printf("Sizeof(Int LeBron): %ld\n", sizeof(ny.LeBron));
  printf("Sizeof(Void *marco): %ld\n", sizeof(ny.marco));
  printf("Sizeof(Void *two): %ld\n", sizeof(ny.two));
  return 0;
}

/*
void function() {
  int stack2;

  printf("stack2:  %15p\n", &stack2);
}

int function_test() {
  int stack;
  void *heap = malloc(1);
  void *heap2 = malloc(1);


  printf("program: %15p\n", function_test);
  printf("heap:    %15p\n", heap);
  printf("heap2:   %15p\n", heap2);
  printf("stack:   %15p\n", &stack);
  function();
  return 0;
}

int stack[5];
void push();
int pop();
void traverse();
bool is_empty();
int top_element();
int ptr = 0;

int main() {
  int element, choice;

  for (;;)
    {
      printf("Stack Operations.\n");
      printf("1. Insert into stack (Push operation).\n");
      printf("2. Delete from stack (Pop operation).\n");
      printf("3. Print top element of stack.\n");
      printf("4. Check if stack is empty.\n");
      printf("5. Traverse the stack.\n");
      printf("6. Exit.\n");
      printf("Enter your choice.\n");
      scanf("%d", &choice);

      switch (choice) {
      case 1:
	if (ptr == 5)
	  printf("Error: Overflow\n\n");
	else {
	  printf("Enter the value to insert.\n");
	  scanf("%d", &element);
	  push(element);
	}
	break;

      case 2:
	if (ptr == 0)
	  printf("Error: Underflow.\n");
	else {
	  element = pop();
	  printf("Element removed from stack is %d.\n", element);
	}
	break;

      case 3:
	if (!is_empty()) {
	  element = top_element();
	  printf("Element at the top of stack is %d\n\n", element);
	}
	else
	  printf("Stack is empty.\n\n");
	break;

      case 4:
	if (is_empty())
	  printf("Stack is empty.\n\n");
	else
	  printf("Stack is not empty.\n\n");
	break;

      case 5:
	traverse();
	break;

      case 6:
	exit(0);
      }
    }
}

void push(int value) {
  stack[ptr] = value;
  ptr++;
}

int pop() {
  ptr--;
  return stack[ptr];
}

void traverse() {
  int d;

  if (ptr == 0) {
    printf("Stack is empty.\n\n");
    return;
  }

  printf("There are %d elements in stack.\n", ptr);

  for (d = ptr - 1; d >= 0; d--)
    printf("%d\n", stack[d]);

  printf("\n");
  for (d = 0; d < 5; d++)
    printf("Stack index %d contains: %d\n", d, stack[d]);
  printf("\n");
}

bool is_empty() {
  if (ptr == 0)
    return true;
  else
    return false;
}

int top_element() {
  return stack[ptr - 1];
}
*/