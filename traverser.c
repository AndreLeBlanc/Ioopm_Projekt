#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"
#include "traverser.h"
#include "linked_list.h"

void printAddress(void *object) {
  printf("\t[%p]\n", object);
}

ll_head traverse_pointers_from_LL(ll_head pointers) {

    ll_head new_nodes = LL_initRoot();
    ll_head pointers_in_obj = NULL;

    if(pointers != NULL && !LL_isEmpty(pointers)) {

        ll_node *cursor = *pointers;
        while(cursor != NULL) {

            if(LL_getContent(cursor) == NULL) {
              cursor = LL_getNext(cursor);
              continue;
            }

            void **content = (void **)LL_getContent(cursor);
            char *metadata = md_get_format_string(LL_getContent(cursor));

            if(strcmp(metadata, NO_MD) == 0) {
              cursor = LL_getNext(cursor);
              continue;
            }

            if(*content != NULL) {
              pointers_in_obj = fs_get_pointers_within_object(*content);
            }

            traverse_pointers_from_LL(pointers_in_obj);

            if(pointers_in_obj != NULL) {

                ll_node *recursive_cursor = *pointers_in_obj;
                while(recursive_cursor != NULL) {
                    void *next = LL_getNext(recursive_cursor);
                    void *content = LL_deletePointer(pointers_in_obj, recursive_cursor);
                    LL_createAndInsertSequentially(new_nodes, content);
                    recursive_cursor = next;
                }


            }

            cursor = LL_getNext(cursor);
        }

        ll_node *nn_cursor = *new_nodes;
        while(nn_cursor != NULL) {
            void *next = LL_getNext(nn_cursor);
            void *content = LL_deletePointer(new_nodes, nn_cursor);
            LL_createAndInsertSequentially(pointers, content);
            nn_cursor = next;
        }


        free(pointers_in_obj);
        free(new_nodes);

        return pointers;

    } else {

      free(new_nodes);
      return LL_initRoot();

    }

}
