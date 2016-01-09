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
#include "stack_traverser.h"

void test_is_pointing_at_heap() {
  heap_t *new_heap = h_init(1024, true, 100.0);
  void *ptr = h_alloc_struct(new_heap, "***i");

  CU_ASSERT_PTR_NOT_NULL(ptr);
  CU_ASSERT_EQUAL(is_pointing_at_heap(ptr, new_heap), true);

  h_delete(new_heap);
}

void test_get_alive_stack_pointers() {
  heap_t *new_heap = h_init(1024, true, 100.0);
  char *ptr = h_alloc_struct(new_heap, "cccc");
  ptr = "heej";
  
  ll_node **test_root = get_alive_stack_pointers(new_heap);
  CU_ASSERT_PTR_NOT_NULL(test_root);
  CU_ASSERT_PTR_NOT_NULL(ptr);
  CU_ASSERT_EQUAL(LL_getContent(*test_root), ptr);
  CU_ASSERT_EQUAL(LL_getContent(*test_root), "heej");

  int *number = h_alloc_struct(new_heap, "*i");
  *number = 666;
  test_root = get_alive_stack_pointers(new_heap);
  CU_ASSERT_PTR_NOT_NULL(number);
  CU_ASSERT_EQUAL(LL_getContent(LL_getNext(*test_root)), *number);

  h_delete(new_heap);
} 

void test_get_stack_top(){ 
  void* top = get_stack_top(); 
  CU_ASSERT_PTR_NOT_NULL(top);
}

int init_suite(void) {
  //create a new stack
  return 0;
}

int clean_suite(void) {
  //free memory and shit.
  return 0;
}

int main() {
  
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

  if (NULL == CU_add_test(pSuite, "Testing get_stack_top", test_get_stack_top)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_add_test(pSuite, "Testing is_pointing_at_heap", test_is_pointing_at_heap)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_add_test(pSuite, "Testing get_alive_stack_pointers", test_get_alive_stack_pointers)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  //Run all tests using the CUnit Basic interface
  CU_basic_set_mode(CU_BRM_VERBOSE);
  CU_basic_run_tests();
  int failures = CU_get_number_of_failures();
  CU_cleanup_registry();
  
  assert(failures == 0);
  return 0;
}

