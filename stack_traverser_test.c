#include <CUnit/CUnit.h>
#include <CUnit/CUError.h>    
#include <CUnit/TestDB.h>
#include <CUnit/TestRun.h>      
#include <CUnit/Automated.h>
#include <CUnit/Basic.h>
#include <CUnit/Console.h>
#include <assert.h>
#include "stack_traverser.h"

#define Dump_registers()			\
  jmp_buf env;					\
  if (setjmp(env)) abort();			\

void test_is_pointing_at_heap() {
  heap_t *new_heap1 = h_init(1024, true, 100.0);
  void *ptr = h_alloc_struct(new_heap1, "***i");
  void *data = h_alloc_data(new_heap1, sizeof(void));
  void *data1 = h_alloc_data(new_heap1, sizeof(int));
  int stackish;

  CU_ASSERT_PTR_NOT_NULL(ptr);
  CU_ASSERT_TRUE(is_pointing_at_heap(&ptr, new_heap1));
  CU_ASSERT_TRUE(is_pointing_at_heap(&data, new_heap1));
  CU_ASSERT_TRUE(is_pointing_at_heap(&data1, new_heap1));
  CU_ASSERT_FALSE(is_pointing_at_heap(&stackish, new_heap1));

  h_delete(new_heap1);
}

void test_get_alive_stack_pointers_with_int() {
  heap_t *new_heap = h_init(1024, true, 70);

  int *number = h_alloc_data(new_heap, sizeof(int)); 
  *number = 666;
  Dump_registers();

  ll_node **list = get_alive_stack_pointers(new_heap);

  CU_ASSERT_EQUAL(LL_length(list),1);

  free(list);
  h_delete(new_heap);
} 

void test_get_alive_stack_pointers_with_char() {
  heap_t *new_heap2 = h_init(1024, true, 70);

  char *c = h_alloc_data(new_heap2, sizeof(char)); 
  c = "h";

  Dump_registers();

  ll_node **list = get_alive_stack_pointers(new_heap2);

  CU_ASSERT_EQUAL(LL_length(list),1);
  
  free(list);
  h_delete(new_heap2);
} 

void test_get_alive_stack_pointers_with_struct(){ 
  heap_t *heap = h_init(2048, true, 60);
  
  void *structish = h_alloc_struct(heap,"**ii");

  Dump_registers(); 

  ll_node **list = get_alive_stack_pointers(heap);

  CU_ASSERT_EQUAL(LL_length(list),1); 
  
  free(list); 
  h_delete(heap);
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
    
  if (NULL == CU_add_test(pSuite, "Testing get_alive_stack_pointers_with_int", test_get_alive_stack_pointers_with_int)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_add_test(pSuite, "Testing get_alive_stack_pointers_with_char", test_get_alive_stack_pointers_with_char)) {
    CU_cleanup_registry();
    return CU_get_error();
  }   

  if (NULL == CU_add_test(pSuite, "Testing get_alive_stack_pointers_with_struct", test_get_alive_stack_pointers_with_struct)) {
    CU_cleanup_registry();
    return CU_get_error();
  }

  if (NULL == CU_add_test(pSuite, "Testing is_pointing_at_heap", test_is_pointing_at_heap)) {
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

