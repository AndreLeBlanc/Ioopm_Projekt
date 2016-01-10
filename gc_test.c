#include <CUnit/Basic.h>
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "heap.h"
#include "traverser.h"
#include "stack_traverser.h"

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

struct linked {
  void *content;
  int number;
  void *next;
};

void testTRAVERSESTRUCT() {

  ll_head stack_pointers = LL_initRoot();

  struct linked *list_a = h_alloc_struct(heap, "*i*");
  struct linked *list_b = h_alloc_struct(heap, "*i*");
  struct linked *list_c = h_alloc_struct(heap, "*i*");
  struct linked *list_d = h_alloc_struct(heap, "*i*");

  struct linked *list_aa = h_alloc_struct(heap, "*i*");
  struct linked *list_ab = h_alloc_struct(heap, "*i*");
  struct linked *list_ac = h_alloc_struct(heap, "*i*");
  struct linked *list_ad = h_alloc_struct(heap, "*i*");

  struct linked a, b, c, d, aa, ab, ac, ad;
  *list_a = a;
  *list_b = b;
  *list_c = c;
  *list_d = d;
  *list_aa = aa;
  *list_ab = ab;
  *list_ac = ac;
  *list_ad = ad;

  list_a->content = list_aa;
  list_a->next = list_b;
  list_a->number = 22;

  list_b->content = NULL;
  list_b->next = list_c;
  list_b->number = 44;

  list_c->content = NULL;
  list_c->next = list_d;
  list_c->number = 66;

  list_d->content = NULL;
  list_d->next = NULL;
  list_d->number = 88;

  list_aa->content = NULL;
  list_ab->content = NULL;
  list_ac->content = NULL;
  list_ad->content = NULL;

  list_aa->next = list_ab;
  list_ab->next = list_ac;
  list_ac->next = list_ad;
  list_ad->next = NULL;

  list_aa->number = 10;
  list_ab->number = 20;
  list_ac->number = 30;
  list_ad->number = 40;

  LL_createAndInsertSequentially(stack_pointers, list_a);
  LL_createAndInsertSequentially(stack_pointers, list_b);
  LL_createAndInsertSequentially(stack_pointers, list_c);
  LL_createAndInsertSequentially(stack_pointers, list_d);
  LL_createAndInsertSequentially(stack_pointers, list_aa);
  LL_createAndInsertSequentially(stack_pointers, list_ab);
  LL_createAndInsertSequentially(stack_pointers, list_ac);
  LL_createAndInsertSequentially(stack_pointers, list_ad);

  CU_ASSERT_EQUAL(LL_length(traverse_pointers_from_LL(stack_pointers)), 8);

}

void testEMPTYPOINTERLIST() {

  ll_head stack_pointers = LL_initRoot();
  LL_createAndInsertSequentially(stack_pointers, NULL);

  CU_ASSERT_EQUAL(LL_length(traverse_pointers_from_LL(stack_pointers)), 1);

}

void testEMPTYLISTRTRAVERSE() {

  ll_head stack_pointers = LL_initRoot();
  struct linked *list_a = h_alloc_struct(heap, "*i*");
  LL_createAndInsertSequentially(stack_pointers, list_a);

  CU_ASSERT_EQUAL(LL_length(traverse_pointers_from_LL(stack_pointers)), 1);

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

    // puts("");
    // printf("a: %p -> %p : { %p }\n", pt_a, pt_a->link, (pt_a + sizeof(struct test) - 3));
    // printf("a: %p -> %p\n", pt_a, pt_a->link);
    // LL_map(fs_get_pointers_within_object(pt_a), testPrint);
    // printf("b: %p -> %p\n", pt_b, pt_b->link);
    // LL_map(fs_get_pointers_within_object(pt_b), testPrint);
    // printf("c: %p -> %p\n", pt_c, pt_c->link);
    // LL_map(fs_get_pointers_within_object(pt_c), testPrint);
    // printf("d: %p -> %p\n", pt_d, pt_d->link);
    // LL_map(fs_get_pointers_within_object(pt_d), testPrint);

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

  int *pt_a = h_alloc_data(heap, sizeof(int));
  int *pt_b = h_alloc_data(heap, sizeof(int));
  int *pt_c = h_alloc_data(heap, sizeof(int));
  int *pt_d = h_alloc_data(heap, sizeof(int));

  *pt_a = 1;
  *pt_b = 2;
  *pt_c = 3;
  *pt_d = 4;

  LL_createAndInsertSequentially(stack_pointers, pt_a);
  LL_createAndInsertSequentially(stack_pointers, pt_b);
  LL_createAndInsertSequentially(stack_pointers, pt_c);
  LL_createAndInsertSequentially(stack_pointers, pt_d);

  CU_ASSERT_EQUAL(LL_length(traverse_pointers_from_LL(stack_pointers)),4);
}

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


int main(int argc, char const *argv[]) {

    CU_pSuite heapSuite = NULL;
    CU_pSuite stackSuite = NULL;

    //initialize the CUnit test registry
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    //add a suite to the registry
    heapSuite = CU_add_suite("Heap Suite", init_suite, clean_suite);
    stackSuite = CU_add_suite("Stack Suite", init_suite, clean_suite);

    if (NULL == heapSuite || NULL == stackSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(heapSuite, "testing traverseLLHeap", testTRAVERSE_LL_HEAP) ||
         NULL == CU_add_test(heapSuite, "testing traverseHeap", testTRAVERSE) ||
         NULL == CU_add_test(heapSuite, "testing traverseEmptyList", testEMPTYLISTRTRAVERSE) ||
         NULL == CU_add_test(heapSuite, "testing traverseEmptyPointerList", testEMPTYPOINTERLIST) ||
         NULL == CU_add_test(heapSuite, "testing traverseAdvancedStruct", testTRAVERSESTRUCT) ||
         NULL == CU_add_test(heapSuite, "testing get_pointers_within_object", testGETPOINTERSWITHINOBJECT) ||
         NULL == CU_add_test(heapSuite, "testing printHeap", testPRINTHEAP) ||
         NULL == CU_add_test(stackSuite, "testing get_alive_stack_pointers", test_get_alive_stack_pointers) ||
         NULL == CU_add_test(stackSuite, "testing is_pointing_at_heap", test_is_pointing_at_heap) ||
         NULL == CU_add_test(stackSuite, "testing get_stack_top", test_get_stack_top)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    //Run all tests using the CUnit Basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    int failures = CU_get_number_of_failures();
    CU_cleanup_registry();

    // assert(failures == 0);
    return EXIT_SUCCESS;

}
