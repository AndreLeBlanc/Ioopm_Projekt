#include <CUnit/Basic.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include "heap.h"
#include "traverser.h"

#include "gc.h"

heap_t *heap = NULL;

int init_suite(void)
{
    //create a new stack
    heap = h_init(2048, 1, 1.0);
    return 0;
}

int clean_suite(void) {
  //free memory and shit.
  return 0;
}

void testPRINTHEAP() {
  void *pointer = h_alloc_data(heap, 16);
  void *pointer2 = h_alloc_struct(heap, "***iii");
  print_traversed_heap(heap);
  CU_ASSERT_EQUAL(true ,true);
}

void testCUNITWORKS() {
  CU_ASSERT_EQUAL((1+1), 2);
}

void testPRINTSTACK() {
  // print_stack();
  CU_ASSERT_EQUAL(true, true);
}

int main(int argc, char const *argv[]) {

    CU_pSuite pSuite = NULL;

    //initialize the CUnit test registry
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    //add a suite to the registry
    pSuite = CU_add_suite("Heap Suite", init_suite, clean_suite);
    if (NULL == pSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ((NULL == CU_add_test(pSuite, "testing CUnit", testCUNITWORKS)) ||
         NULL == CU_add_test(pSuite, "testing printStack", testPRINTSTACK) ||
         NULL == CU_add_test(pSuite, "testing printHeap", testPRINTHEAP)) {
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
