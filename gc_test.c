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
    get_allocation_page(heap);
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

void test_object_metadata() {
  void *test = h_alloc_data(heap, sizeof(int));
  md_set_bit_vector(test, 'a');
  md_set_forwarding_address(test, 0x1);
  md_set_copied_flag(test, true);
  md_set_format_string(test, "3*");

  CU_ASSERT_TRUE(
    md_get_bit_vector(test) == 'a' &&
    strcmp(md_get_format_string(test), "3*") == 0 &&
    md_get_copied_flag(test) == true  &&
    md_get_forwarding_address(test) == 0x1
  );
}

void test_metadata_check() {
  void *test = h_alloc_struct(heap, "3*");
  CU_ASSERT_STRING_EQUAL("3*", md_get_format_string(test));
}

void test_pointers_inside_multiple_pointers_struct() {
  void *test = h_alloc_struct(heap, "3*4i2*");
  CU_ASSERT_EQUAL(LL_length(fs_get_pointers_within_object(test)), 5);
}

void test_set_metadata_check() {
  void *test = h_alloc_struct(heap, "3*");
  md_set_format_string(test, "4*");
  CU_ASSERT_STRING_EQUAL("4*", md_get_format_string(test));
}

void test_is_valid_object_false() {
  char *invalid = "Hello World";
  CU_ASSERT_FALSE(validate_object(invalid));
}

void test_is_valid_object_true() {
  char *valid = h_alloc_data(heap, sizeof(char) * 2 + 1);
  strcpy(valid, "ab");
  *valid = "ab";
  CU_ASSERT_TRUE(validate_object(valid));
}

struct linked {
  void *content;
  int number;
  void *next;
};


void testINITHEAP() {
  heap_t *heap = h_init(2048, true, 1.0);
  //printf("total size heap: %d", heap->total_size);
  CU_ASSERT_TRUE(heap != NULL &&
		 heap->meta_p == heap &&
		 heap->unsafe_stack == true &&
		 //heap->total_size == 2048 &&
		 h_used(heap) == 0 &&
		 h_avail(heap) < 2048 &&
		 heap->gc_threshold == 1.0);

  h_delete(heap);


}
void testDELETEHEAP() {
}



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
  printf("[%p]\n", *(object));
}

//TODO, it fails.
void test_fs_get_object_size() {

  struct test *pt_a = h_alloc_struct(heap, "****iii****");
  size_t total = sizeof(void *) * 8 + 3 * sizeof(int);
  size_t object_size = fs_get_object_size(pt_a);
  printf("object: %d\ntotal: %d\n", object_size, total);
  CU_ASSERT_EQUAL(total, object_size);

}

void test_pointers_within_object() {

  struct test *pt_a = h_alloc_struct(heap, "*");
  void *pt_b = LL_getContent(LL_getNodeAtIndex(fs_get_pointers_within_object(pt_a), 0));
  CU_ASSERT_EQUAL(pt_a, pt_b);

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

  LL_createAndInsertSequentially(stack_pointers, pt_a);
  LL_createAndInsertSequentially(stack_pointers, pt_d);

  int length = LL_length(traverse_pointers_from_LL(stack_pointers));
  CU_ASSERT_EQUAL(length, 4);

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

  CU_pSuite initHeapSuite = NULL;
  CU_pSuite heapSuite = NULL;
  CU_pSuite stackSuite = NULL;

    //initialize the CUnit test registry
    if (CUE_SUCCESS != CU_initialize_registry()) {
        return CU_get_error();
    }

    //add a suite to the registry
    initHeapSuite = CU_add_suite("Init Heap Suite", init_suite, clean_suite);
    heapSuite = CU_add_suite("Heap Suite", init_suite, clean_suite);
    stackSuite = CU_add_suite("Stack Suite", init_suite, clean_suite);

    if (NULL == initHeapSuite || NULL == heapSuite || NULL == stackSuite) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(initHeapSuite, "testing init_h", testINITHEAP) ||
	NULL == CU_add_test(heapSuite, "testing traverseLLHeap", testTRAVERSE_LL_HEAP) ||
	NULL == CU_add_test(heapSuite, "testing traverseHeap", testTRAVERSE) ||
         NULL == CU_add_test(heapSuite, "testing traverseEmptyList", testEMPTYLISTRTRAVERSE) ||
         NULL == CU_add_test(heapSuite, "testing traverseEmptyPointerList", testEMPTYPOINTERLIST) ||
         NULL == CU_add_test(heapSuite, "testing traverseAdvancedStruct", testTRAVERSESTRUCT) ||
         NULL == CU_add_test(heapSuite, "testing test_object_metadata", test_object_metadata) ||
         NULL == CU_add_test(heapSuite, "testing test_is_valid_object_false", test_is_valid_object_false) ||
         NULL == CU_add_test(heapSuite, "testing test_pointers_inside_multiple_pointers_struct", test_pointers_inside_multiple_pointers_struct) ||
         NULL == CU_add_test(heapSuite, "testing test_set_metadata_check", test_set_metadata_check) ||
         NULL == CU_add_test(heapSuite, "testing test_is_valid_object_true", test_is_valid_object_true) ||
         NULL == CU_add_test(heapSuite, "testing test_fs_get_object_size", test_fs_get_object_size) ||
         NULL == CU_add_test(heapSuite, "testing test_metadata_check", test_metadata_check) ||
         NULL == CU_add_test(heapSuite, "testing get_pointers_within_object", testGETPOINTERSWITHINOBJECT) ||
         NULL == CU_add_test(heapSuite, "test_pointers_within_object", test_pointers_within_object) ||
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

    //removed because of gcov, if there's errors gcov won't run correctly. /V
    // assert(failures == 0);
    return EXIT_SUCCESS;

}
