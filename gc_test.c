#include <CUnit/Basic.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "heap.h"
#include "traverser.h"

#include "gc.h"

heap_t *heap = NULL;

struct test {
    void *link;
};

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

void testGETPOINTERSWITHINOBJECT() {

  struct test *object = NULL;
  int number = 10;
  //allokera minne för object i vår heap.
  object = h_alloc_struct(heap, "*");
  //allokerar minne för int.
  int *persistent_number = h_alloc_data(heap, sizeof(int));
  *persistent_number = number;
  //slänger in int i vårt test-objekt.
  object->link = persistent_number;
  CU_ASSERT_EQUAL(LL_length(fs_get_pointers_within_object(object)), 1);

}

void testPrint(void **object) {
  // struct test* pointer = (struct test*)object;
  // printf("[%p]\n", ((struct test*)object)->link);
  printf("[%p]\n", *(object));
}

void testTRAVERSE_LL_HEAP() {

    ll_head stack_pointers = LL_initRoot();

    struct test *pt_a = h_alloc_struct(heap, "*");
    struct test *pt_b = h_alloc_struct(heap, "*");
    struct test *pt_c = h_alloc_struct(heap, "*");
    struct test *pt_d = h_alloc_struct(heap, "*");

    struct test a, b, c, d;
    *pt_b = b;
    *pt_c = c;
    *pt_a = a;
    *pt_d = d;

    pt_c->link = NULL;
    pt_b->link = pt_c;
    pt_a->link = pt_b;
    pt_d->link = NULL;

    puts("");
    // printf("a: %p -> %p : { %p }\n", pt_a, pt_a->link, (pt_a + sizeof(struct test) - 3));
    printf("a: %p -> %p\n", pt_a, pt_a->link);
    LL_map(fs_get_pointers_within_object(pt_a), testPrint);
    printf("b: %p -> %p\n", pt_b, pt_b->link);
    LL_map(fs_get_pointers_within_object(pt_b), testPrint);
    printf("c: %p -> %p\n", pt_c, pt_c->link);
    LL_map(fs_get_pointers_within_object(pt_c), testPrint);
    printf("d: %p -> %p\n", pt_d, pt_d->link);
    LL_map(fs_get_pointers_within_object(pt_d), testPrint);

    int objectsA = LL_length(fs_get_pointers_within_object(pt_a));
    printf("Objects in A: %d\n", objectsA);

    LL_createAndInsertSequentially(stack_pointers, pt_a);
    LL_createAndInsertSequentially(stack_pointers, pt_d);

    int length = LL_length(traverse_pointers_from_LL(stack_pointers));
    printf("length: %d\n", length);
    CU_ASSERT_EQUAL(length, 4);

}

void testPRINTHEAP() {
  // void *pointer = h_alloc_data(heap, 16);
  // void *pointer2 = h_alloc_struct(heap, "***iii");
  // print_traversed_heap(heap);
  CU_ASSERT_EQUAL(true ,true);
}

void testTRAVERSE() {
  ll_head stack_pointers = LL_initRoot();
  int a,b,c,d;
  LL_createAndInsertSequentially(stack_pointers, &a);
  LL_createAndInsertSequentially(stack_pointers, &b);
  LL_createAndInsertSequentially(stack_pointers, &c);
  LL_createAndInsertSequentially(stack_pointers, &d);

  CU_ASSERT_EQUAL(LL_length(traverse_pointers_from_LL(stack_pointers)),4);
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
         NULL == CU_add_test(pSuite, "testing traverseHeap", testTRAVERSE) ||
         NULL == CU_add_test(pSuite, "testing get_pointers_within_object", testGETPOINTERSWITHINOBJECT) ||
         NULL == CU_add_test(pSuite, "testing traverseLLHeap", testTRAVERSE_LL_HEAP) ||
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
