#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "heap.h"
#include "traverser.h"
#include "linked_list.h"

void printAddress(void *object) {
  printf("[%p]\n", object);
}

ll_head traverse_pointers_from_LL(ll_head pointers) {

    ll_head new_nodes = LL_initRoot();
    if(pointers != NULL && !LL_isEmpty(pointers)) {

        ll_node *cursor = *pointers;
        while(cursor != NULL) {

            if(LL_getContent(cursor) == NULL) {
              cursor = LL_getNext(cursor);
              continue;
            }

            if(LL_getContent(cursor) == NULL) {
              continue;
            }

            void **content = (void **)LL_getContent(cursor);
            char *metadata = md_get_format_string(LL_getContent(cursor));

            if(strcmp(metadata, NO_MD) == 0) {
              cursor = LL_getNext(cursor);
              continue;
            }

            ll_head pointers_in_obj = NULL;

            if(*content != NULL) {
              pointers_in_obj = fs_get_pointers_within_object(*content);
            }

            ll_head recursive_data = traverse_pointers_from_LL(pointers_in_obj);

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

        ll_node *nn_cursor = *new_nodes;
        while(nn_cursor != NULL) {
            LL_createAndInsertSequentially(pointers, nn_cursor);
            nn_cursor = LL_getNext(nn_cursor);
        }

        LL_deleteList(new_nodes);

        return pointers;

    } else {

        return NULL;

    }

}
