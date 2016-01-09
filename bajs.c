#include "heap.h"
#include "heap.c"
#include "linked_list.c"
//#include "stack_traverser.c"

// ASSERT macros for use in test cases, and includes other framework headers.
#include <CUnit/CUnit.h>

// Error handing functions and data types. Included automatically by CUnit.h.
#include <CUnit/CUError.h>	

// Data type definitions and manipulation functions for the test registry, suites, and tests. Included automatically by CUnit.h.
#include <CUnit/TestDB.h>

// Data type definitions and functions for running tests and retrieving results. Included automatically by CUnit.h.
#include <CUnit/TestRun.h>	

// Automated interface with xml output.
#include <CUnit/Automated.h>

// Basic interface with non-interactive output to stdout.
#include <CUnit/Basic.h>

// Interactive console interface.
#include <CUnit/Console.h>

extern char **environ;

int init_suite() {
  return 0;
}

int clean_suite() {
  return 0;
}

void stack_top(){
  void *stack = NULL;
  printf("Stack top: %p\n", stack);
  printf("Environ:   %p\n", *environ);
}

int main() {
  stack_top();
  heap_t *new_heap = h_init(10240, true, 100.0);
  int *i = h_alloc_struct(new_heap, "*i");

/*  CU_pSuite pSuite = NULL;

    //initialize the CUnit test registry
  if (CUE_SUCCESS != CU_initialize_registry()) {
    return CU_get_error();
  }

  //add a suite to the registry
  pSuite = CU_add_suite("Shit", init_suite, clean_suite);
  if (NULL == pSuite) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_add_test(pSuite, "Testing HEJ == HEJ", (CU_ASSERT_EQUAL(5, 5)))) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  //Run all tests using the CUnit Basic interface
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  int failures = CU_get_number_of_failures();
  CU_cleanup_registry();
*/
/*  if (is_pointing_at_heap(i, new_heap))
    puts("Yeah man");
  else
  puts("Nah man.");*/
  return 0;
}
