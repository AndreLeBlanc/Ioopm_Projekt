#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"
#include "traverser.h"
#include "linked_list.h"

/*
  Hallåjsan. Ni kanske vill hämta stackpekarna som pekar i heapen också!
  Bara att använda traverser_stack_list(heap_t *h) som finns i stack_traverser.c.
  Denna lista med pekare kommer vara i samma format som den i linked_list-modulen.
  /H
*/
// ll_head traverse_pointers_from_LL_bak(ll_head pointers) {
//
//     if(!LL_isEmpty(pointers)) {
//
//         //loop through all the pointers
//         ll_node* cursor = *pointers;
//         while(cursor != NULL) {
//             //grab pointers within object.
//             ll_head child_pointers = fs_get_pointers_within_object(cursor);
//             printf("\nCursor: %p\n", cursor);
//             printf("\nChild_pointers: %p\n", child_pointers);
//             if(child_pointers != NULL) {
//                 puts("Inside recursive call.");
//                 ll_head recursive_pointers = traverse_pointers_from_LL(child_pointers);
//                 if(recursive_pointers != NULL) {
//                     while(recursive_pointers != NULL) {
//                         printf("\nRec_cursor: %p\n", recursive_pointers);
//                         LL_createAndInsertSequentially(pointers, recursive_pointers);
//                         recursive_pointers = LL_getNext(recursive_pointers);
//                     }
//                 } else {
//                     while(child_pointers != NULL) {
//                         printf("\nChild_cursor: %p\n", child_pointers);
//                         LL_createAndInsertSequentially(pointers, child_pointers);
//                         child_pointers = LL_getNext(child_pointers);
//                     }
//                 }
//                 // LL_insertSequentially(pointers, traverse_pointers_from_LL(child_pointers)); //add to list and recursive call
//             }
//             cursor = LL_getNext(cursor);
//         }
//
//         return pointers;
//
//     } else {
//         return NULL;
//     }
//
// }

void printAddress_bak(void **object) {
  printf("[%p]\n", *object);
}

void printAddress(void *object) {
  printf("[%p]\n", object);
}

ll_head traverse_pointers_from_LL(ll_head pointers) {

    ll_head new_nodes = LL_initRoot();
    if(pointers != NULL && !LL_isEmpty(pointers)) {

      printf("\nPOINTERS AQUIRED\n");
      LL_map(pointers, printAddress);

        ll_node *cursor = *pointers;
        while(cursor != NULL) {

            if(LL_getContent(cursor) == NULL) {
              cursor = LL_getNext(cursor);
              continue;
            }

            printf("%p -> %p\n", cursor, LL_getContent(cursor));

            // puts("casting ints");
            // void *content = (void *)LL_getContent(cursor);

            if(LL_getContent(cursor) == NULL) {
              puts("fuck.");
            }
            puts("casting");
            void **content = (void **)LL_getContent(cursor);
            printf("metadata: %s\n", md_get_format_string(LL_getContent(cursor)));
            char *metadata = md_get_format_string(LL_getContent(cursor));

            if(strcmp(metadata, NO_MD) == 0) {
              cursor = LL_getNext(cursor);
              continue;
            }
            
            ll_head pointers_in_obj = NULL;

            if(*content != NULL) {
              puts("pointers");
              pointers_in_obj = fs_get_pointers_within_object(*content);
            }

            puts("recursive");
            ll_head recursive_data = traverse_pointers_from_LL(pointers_in_obj);

            if(pointers_in_obj != NULL) {
              puts("pointers_in_obj");
              LL_map(pointers_in_obj, printAddress);
            }

            if(recursive_data != NULL) {
              puts("Recursive_data");
              LL_map(recursive_data, printAddress);
            } else {
              puts("recdata: NULL");
            }

            if(recursive_data != NULL) {

                ll_node *recursive_cursor = *recursive_data;
                while(recursive_cursor != NULL) {
                    LL_createAndInsertSequentially(new_nodes, LL_getContent(recursive_cursor));
                    //WARNING, may fuck shit up.
                    recursive_cursor = LL_getNext(recursive_cursor);
                }

            }

            cursor = LL_getNext(cursor);
        }

        puts("[POINTERS BEFORE]");
        LL_map(pointers, printAddress);

        ll_node *nn_cursor = *new_nodes;
        while(nn_cursor != NULL) {
            //insertandCreateblabla
            LL_createAndInsertSequentially(pointers, nn_cursor);
            nn_cursor = LL_getNext(nn_cursor);
        }

        puts("[POINTERS]");
        LL_map(pointers, printAddress);

        return pointers;

    } else {

        return NULL;

    }

}
