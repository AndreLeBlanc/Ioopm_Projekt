#include <CUnit/Basic.h>
#include <assert.h>
#include <stdlib.h>

#include "gc.h"

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

    if ((NULL == CU_add_test(pSuite, "testing CUnit", testCUNITWORKS))) {
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
