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

#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>

#include "stack_traverser.h"

void test_function() {
  puts("Just for testing.");
}

char *test_heap_grows_upwards() {
  void *first = malloc(1);
  void *second = malloc(1);

  CU_ASSERT_EQUAL((heap_grows_upwards()), (first < second));
}

void test_is_pointing_at_heap() {
  heap_t *new_heap = h_init(1024, true, 100.0);
  void *ptr = h_alloc_struct(new_heap, "***i");

  CU_ASSERT_PTR_NOT_NULL(ptr);
  CU_ASSERT_EQUAL(is_pointing_at_heap(ptr, new_heap), true);
}

void test_get_stack_top() {
  void *top = get_stack_top();
  CU_ASSERT(*(int*)top);
}

void test_stack_grows_from_top() {
  int stack1;
  int stack2;

  CU_ASSERT_EQUAL(stack_grows_from_top(), (&stack1 < &stack2));
}

void test_traverse_stack_list() {
  
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


  // CUnit tests
  CU_pSuite pSuite = NULL;
  
  CU_initialize_registry();

  CU_add_suite("Haubir", init_suite, cleanup_suite);

  CU_add_test(pSuite, "Test", test_function);

  CU_console_run_tests();
  CU_cleanup_registry();
  return 0;
}


int init_suite(void)
{
  //create a new stack
  return 0;
}

int clean_suite(void) {
  //free memory and shit.
  return 0;
}

void testCUNITWORKS() {
  CU_ASSERT_EQUAL((1+1), 2);
}

void testPRINTSTACK() {
  print_stack();
  CU_ASSERT_EQUAL(true, true);
}

int main(int argc, char const *argv[]) {

  CU_pSuite pSuite = NULL;

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

  if ((NULL == CU_add_test(pSuite, "testing CUnit", testCUNITWORKS)) ||
      NULL == CU_add_test(pSuite, "testing printStack", testPRINTSTACK)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  //Run all tests using the CUnit Basic interface
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  int failures = CU_get_number_of_failures();
  CU_cleanup_registry();

  assert(failures == 0);
  return EXIT_SUCCESS;

}
