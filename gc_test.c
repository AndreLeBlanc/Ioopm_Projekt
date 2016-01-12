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
extern char **environ; // bottom of stack

struct test {
    void *link;
};

struct linked {
  void *content;
  int number;
  void *next;
};

int init_suite(void) {
    heap = h_init(100000, 1, 1.0);
    return 0;
}

int clean_suite(void) {
  h_delete(heap);
  return 0;
}

void test_traverse_stack_and_heap() {

  heap_t *h = h_init(2048, true, 1.0);
  h_alloc_data(h, sizeof(int));
  h_alloc_data(h, sizeof(int));
  h_alloc_data(h, sizeof(int));
  h_alloc_data(h, sizeof(int));
  ll_head stack_pts = get_alive_stack_pointers(h);
  ll_head heap_pts = traverse_pointers_from_LL(stack_pts);

  void *stack_cursor = *stack_pts;
  void *heap_cursor = *heap_pts;
  bool identical = true;
  while(stack_cursor != NULL) {
    if(LL_getContent(stack_cursor) != LL_getContent(heap_cursor)) {
      identical = false;
      break;
    }
    stack_cursor = LL_getNext(stack_cursor);
    heap_cursor = LL_getNext(heap_cursor);
  }

  int n_stack_pts = LL_length(stack_pts);
  int n_heap_pts = LL_length(heap_pts);

  CU_ASSERT_TRUE(
    identical &&
    n_stack_pts == n_heap_pts
  );

}

void test_traverse_stack_and_heap_empty() {

  heap_t *h = h_init(2048, true, 1.0);
  ll_head stack_pts = get_alive_stack_pointers(h);
  ll_head heap_pts = traverse_pointers_from_LL(stack_pts);

  CU_ASSERT_TRUE(
    LL_isEmpty(stack_pts) &&
    heap_pts == NULL
  );

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
  ll_head pointers = fs_get_pointers_within_object(object);
  CU_ASSERT_EQUAL(LL_length(pointers), 1);
  LL_deleteList(pointers);

}

//HACK, not sure what this function is actually doing except offsetting addresses? /V
void test_update_object_pointers() {
  void *pt_a = h_alloc_struct(heap, "***");
  ll_head pts_a = fs_get_pointers_within_object(pt_a);
  int n_pts_a = LL_length(pts_a);
  void *pt_ac = h_alloc_compact(heap, pt_a);
  //TODO, doesn't work atm, seg fault.
  // update_objects_pointers(pt_ac);
  ll_head pts_ac = fs_get_pointers_within_object(pt_ac);
  int n_pts_ac = LL_length(pts_ac);

  CU_FAIL();
  // CU_ASSERT_EQUAL(n_pts_a, n_pts_ac);
  LL_deleteList(pts_a);
  LL_deleteList(pts_ac);
}

void test_alloc_struct_all_types_lower() {
  void *test = h_alloc_struct(heap, "*ifcld3d3f4c4lfi3*");
  ll_head pointers = fs_get_pointers_within_object(test);
  CU_ASSERT_EQUAL(LL_length(pointers), 4);
  LL_deleteList(pointers);
}

void test_alloc_struct_all_types_upper() {
  void *test = h_alloc_struct(heap, "IIIIDDD");
  ll_head pointers = fs_get_pointers_within_object(test);
  CU_ASSERT_TRUE(LL_isEmpty(pointers));
  LL_deleteList(pointers);
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

void test_small_heap_init() {
  heap_t *small_heap = h_init(0, true, 0.0);
  CU_ASSERT_PTR_NULL(small_heap);
}

void test_metadata_check() {
  void *test = h_alloc_struct(heap, "3*");
  CU_ASSERT_STRING_EQUAL("3*", md_get_format_string(test));
}

void test_pointers_inside_multiple_pointers_struct() {
  void *test = h_alloc_struct(heap, "3*4i2*");
  ll_head pointers = fs_get_pointers_within_object(test);
  CU_ASSERT_EQUAL(LL_length(pointers), 5);
  LL_deleteList(pointers);
}

void test_set_metadata_check() {
  void *test = h_alloc_struct(heap, "3*");
  md_set_format_string(test, "4*");
  CU_ASSERT_STRING_EQUAL("4*", md_get_format_string(test));
}

void test_is_valid_object_false() {
  char *invalid = "Hello World";
  CU_ASSERT_FALSE(validate_object(invalid, heap));
}

void test_is_valid_object_true() {
  char *valid = h_alloc_data(heap, sizeof(char) * 2 + 1);
  CU_ASSERT_TRUE(validate_object(valid, heap));
}

void test_is_devalidate_object_false() {
  char *valid = h_alloc_data(heap, sizeof(char) * 2 + 1);
  devalidate(valid, heap);
  CU_ASSERT_FALSE(validate_object(valid, heap));
}

void testINITHEAP() {
  heap_t *heap = h_init(2048, true, 1.0);
  //printf("total size heap: %d", heap->total_size);
  CU_ASSERT_TRUE(heap != NULL &&
		 heap->meta_p == heap &&
		 heap->unsafe_stack == true &&
		 //heap->total_size == 2048 &&
		 h_used(heap) == 0 &&
		 h_avail(heap) < 2048 &&
		 heap->gc_threshold == 1.0
   );

  h_delete(heap);

}

void testALLOCDATA() {
  int *object = NULL;
  object = h_alloc_data(heap, sizeof(int));
  CU_ASSERT_TRUE(validate_object(object, heap));
}

void testALLOCCOMPACT() {
  void *object = h_alloc_struct(heap, "i*");
  void *compact = h_alloc_compact(heap, object);
  CU_ASSERT_TRUE(validate_object(compact, heap));
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

  ll_head pointers = traverse_pointers_from_LL(stack_pointers);

  CU_ASSERT_EQUAL(LL_length(pointers), 8);

  LL_deleteList(stack_pointers);
  if(!LL_isEmpty(pointers)) {
    LL_deleteList(pointers);
  }

}

void testEMPTYPOINTERLIST() {

  ll_head stack_pointers = LL_initRoot();
  LL_createAndInsertSequentially(stack_pointers, NULL);

  ll_head pointers = traverse_pointers_from_LL(stack_pointers);
  CU_ASSERT_EQUAL(LL_length(pointers), 1);

  LL_deleteList(stack_pointers);
  if(!LL_isEmpty(pointers)) {
    LL_deleteList(pointers);
  }

}

void testEMPTYLISTRTRAVERSE() {

  ll_head stack_pointers = LL_initRoot();
  struct linked *list_a = h_alloc_struct(heap, "*i*");
  LL_createAndInsertSequentially(stack_pointers, list_a);

  ll_head pointers = traverse_pointers_from_LL(stack_pointers);
  CU_ASSERT_EQUAL(LL_length(pointers), 1);

  LL_deleteList(stack_pointers);
  if(!LL_isEmpty(pointers)) {
    LL_deleteList(pointers);
  }

}

//TODO, it fails.
void test_fs_get_object_size() {

  struct test *pt_a = h_alloc_struct(heap, "****iii****");
  size_t total = sizeof(void *) * 8 + 3 * sizeof(int);
  size_t object_size = fs_get_object_size(pt_a);
  CU_ASSERT_EQUAL(total, object_size);

}

void test_pointers_within_object() {

  struct test *pt_a = h_alloc_struct(heap, "*");
  ll_head pointers = fs_get_pointers_within_object(pt_a);
  void *pt_b = LL_getContent(LL_getNodeAtIndex(pointers, 0));
  CU_ASSERT_EQUAL(pt_a, pt_b);
  LL_deleteList(pointers);

}

void testTRAVERSE_LL_HEAP() {

  ll_head stack_pointers = LL_initRoot();

  struct test *pt_a = h_alloc_struct(heap, "*");
  struct test *pt_b = h_alloc_struct(heap, "*");
  struct test *pt_c = h_alloc_struct(heap, "*");
  struct test *pt_d = h_alloc_struct(heap, "*");
  struct test *pt_e = h_alloc_struct(heap, "*");

  struct test a, b, c, d, e;
  *pt_b = b;
  *pt_c = c;
  *pt_a = a;
  *pt_d = d;
  *pt_e = e;

  pt_c->link = pt_d;
  pt_b->link = pt_c;
  pt_a->link = pt_b;
  pt_d->link = NULL;
  pt_e->link = NULL;

  LL_createAndInsertSequentially(stack_pointers, pt_a);
  LL_createAndInsertSequentially(stack_pointers, pt_e);

  traverse_pointers_from_LL(stack_pointers);
  int length = LL_length(stack_pointers);

  puts("\n\n\t[ALLOC]");
  printf("\tpt_a: %p -> %p\n", pt_a, pt_a->link);
  printf("\tpt_b: %p -> %p\n", pt_b, pt_b->link);
  printf("\tpt_c: %p -> %p\n", pt_c, pt_c->link);
  printf("\tpt_d: %p -> %p\n", pt_d, pt_d->link);
  printf("\tpt_e: %p -> %p\n\n", pt_e, pt_e->link);
  puts("\t[RESULT]");
  LL_map(stack_pointers, printAddress);
  puts("\n");

  CU_ASSERT_EQUAL(length, 5);
  //what about LL_map(stack_pointers, free_method_of_some_sort)
  LL_deleteList(stack_pointers);

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
  LL_deleteList(stack_pointers);

}

void test_get_alive_stack_pointers() {
  void *top = get_stack_top();
  void *bottom = (void *)environ;
  heap_t *new_heap = h_init(1024, true, 100.0);

  h_alloc_data(new_heap, 32);
  h_alloc_data(new_heap, 32);
  h_alloc_data(new_heap, 32);

  ll_head chars = get_alive_stack_pointers_in_range(new_heap, top, bottom);
  CU_ASSERT_EQUAL(LL_length(chars), 3);

  h_alloc_struct(new_heap, "*i");
  h_alloc_struct(new_heap, "*i*i");
  h_alloc_struct(new_heap, "*i*ii");

  ll_head nums = get_alive_stack_pointers_in_range(new_heap, top, bottom);
  CU_ASSERT_EQUAL(LL_length(nums), 6);

  h_delete(new_heap);
  LL_deleteList(chars);
  LL_deleteList(nums);
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

    if (NULL == CU_add_test(initHeapSuite, "testing init_h", testALLOCDATA) ||
      	NULL == CU_add_test(initHeapSuite, "testing h_alloc_data", testINITHEAP) ||
      	NULL == CU_add_test(initHeapSuite, "testing h_alloc_compact", testALLOCCOMPACT) ||
      	NULL == CU_add_test(heapSuite, "testing traverseLLHeap", testTRAVERSE_LL_HEAP) ||
      	NULL == CU_add_test(heapSuite, "testing traverseHeap", testTRAVERSE) ||
        NULL == CU_add_test(heapSuite, "testing traverseEmptyList", testEMPTYLISTRTRAVERSE) ||
        NULL == CU_add_test(heapSuite, "testing traverseEmptyPointerList", testEMPTYPOINTERLIST) ||
        NULL == CU_add_test(heapSuite, "testing traverseAdvancedStruct", testTRAVERSESTRUCT) ||
        NULL == CU_add_test(heapSuite, "testing test_traverse_stack_and_heap_empty", test_traverse_stack_and_heap_empty) ||
        NULL == CU_add_test(heapSuite, "testing test_object_metadata", test_object_metadata) ||
        NULL == CU_add_test(heapSuite, "testing test_traverse_stack_and_heap", test_traverse_stack_and_heap) ||
        NULL == CU_add_test(heapSuite, "testing test_update_object_pointers", test_update_object_pointers) ||
        NULL == CU_add_test(heapSuite, "testing test_alloc_struct_all_types_upper", test_alloc_struct_all_types_upper) ||
        NULL == CU_add_test(heapSuite, "testing test_alloc_struct_all_types_lower", test_alloc_struct_all_types_lower) ||
        NULL == CU_add_test(heapSuite, "testing test_small_heap_init", test_small_heap_init) ||
        NULL == CU_add_test(heapSuite, "testing test_is_devalidate_object_false", test_is_devalidate_object_false) ||
        NULL == CU_add_test(heapSuite, "testing test_is_valid_object_false", test_is_valid_object_false) ||
        NULL == CU_add_test(heapSuite, "testing test_pointers_inside_multiple_pointers_struct", test_pointers_inside_multiple_pointers_struct) ||
        NULL == CU_add_test(heapSuite, "testing test_set_metadata_check", test_set_metadata_check) ||
        NULL == CU_add_test(heapSuite, "testing test_is_valid_object_true", test_is_valid_object_true) ||
        NULL == CU_add_test(heapSuite, "testing test_fs_get_object_size", test_fs_get_object_size) ||
        NULL == CU_add_test(heapSuite, "testing test_metadata_check", test_metadata_check) ||
        NULL == CU_add_test(heapSuite, "testing get_pointers_within_object", testGETPOINTERSWITHINOBJECT) ||
        NULL == CU_add_test(heapSuite, "test_pointers_within_object", test_pointers_within_object) ||
        NULL == CU_add_test(stackSuite, "testing get_alive_stack_pointers", test_get_alive_stack_pointers) ||
        NULL == CU_add_test(stackSuite, "testing get_stack_top", test_get_stack_top)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    //Run all tests using the CUnit Basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    // int failures = CU_get_number_of_failures();
    CU_cleanup_registry();

    //removed because of gcov, if there's errors gcov won't run correctly. /V
    // assert(failures == 0);
    return EXIT_SUCCESS;

}
