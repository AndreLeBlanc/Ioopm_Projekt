#ifndef __LINKEDLIST_H__
#define __LINKEDLIST_H__

/**
   @defgroup LL Linked List
   @{
*/

/**
   @brief Linked list node structure.

*/
typedef struct node ll_node;

// CREATE FUNCTIONS
/**
   @defgroup createFun Creation functions. 
   @{
 */

/**
   @brief Initializes the root
   @return Returns a double-pointer which will always point to a pointer to the root node. 
*/
ll_node** LL_initRoot();

/**
   @brief A creation function which will allocate memory for a node and set the content pointer as the parameter.
   @param A void pointer which points to an object which the node will refer to as its content.
   @returns A pointer to the node.
*/

ll_node* LL_createNode(void* content);

/**@}*/

// GET FUNCTIONS
/**
   @defgroup getFun Get Functions
   @{
*/

/**
   @brief A function that returns the next node in the list.
   @param toGetFrom The node to look for the next node in.
   @return The next node in the list.
*/
ll_node* LL_getNext(ll_node* toGetFrom);
/**
   @brief A function that returns the previous node in the list. It searches from the the top of the list until a node points to toGetFrom.
   @param root The root pointer of the list.
   @param toGetFrom A pointer to node to look for the previous node in.
   @return A pointer to the previous node in the list
*/
ll_node* LL_getPrevious(ll_node** root, ll_node* toGetFrom);
/**
   @brief A function that returns the node at a certain index in the list.
   @param root The root pointer of the list.
   @param int The number of the node to return. 
   @return A pointer to node at given index. 
*/
ll_node* LL_getNodeAtIndex(ll_node** root, int index);
/**
   @brief A function that returns the content pointer of a node.
   @param toGetFrom A pointer to the node of which to return the content pointer from. 
   @return The void pointer to the content which the node holds. 
   @note A pointer will often need to be typecast before being used. 
*/
void* LL_getContent(ll_node* toGetFrom);
/**
   @brief A function which finds the position of a given node. 
   @param root The root pointer of the list.
   @param toGetFrom A pointer to the node to get the index of. 
   @return The index value of the node. Where 
   @bug NOT IMPLEMENTED YET
*/
int LL_getNodeIndex(ll_node** root, ll_node* toGetFrom);


/**@}*/

// INSERT FUNCTIONS
/**
   @defgroup insFun Insert Functions
   @{
*/

/**
   @brief A function which inserts a node into the end of the list.
   @param root The root pointer of the list.
   @param toInsert A pointer to the newly inserted node. 
*/
void LL_insertSequentially(ll_node** root, ll_node* toInsert);
/**
   @brief A function which creates and inserts a node into the end of the list.
   @param root The root pointer of the list.
   @param content A pointer to the content which the new node will hold. 
   @return A pointer to the newly created and inserted node.
*/
ll_node* LL_createAndInsertSequentially(ll_node** root, void* content);

/**
   @brief A function which inserts a node comparatively into the list. It uses a pointer to a function to decide where to place it. 
   @param root The root pointer of the list.
   @param compareFunction A pointer to a function which takes two void pointers and returns an int. The function should return <1 when the node it to be placed further to the front or >1 when to be placed further to the end and 0 when the nodes are equal. 
   @param toInsert A pointer to the newly inserted node. 
   @note Inserting something that will cause compareFunction to return 0 (it is considered the same as another item in the list) will place it after the item already in the list. 
*/
void LL_insertComparatively(ll_node** root, ll_node* toInsert,
			 int(*compareFunction)(void*, void*));
/**
   @brief A function which inserts a node comparatively into the list. It uses a pointer to a function to decide where to place it. 
   @param root The root pointer of the list.
   @param content A pointer to the content which the new node will hold.
   @param compareFunction A pointer to a function which takes two void pointers and returns an int. The function should return <1 when the node it to be placed further to the front or >1 when to be placed further to the end and 0 when the nodes are equal. 
   @return A pointer to the newly created and inserted node. 
   @note Inserting something that will cause compareFunction to return 0 (it is considered the same as another item in the list) will place it after the item already in the list. 
*/
ll_node* LL_createAndInsertComparatively(ll_node** root, void* content,
					  int(*compareFunction)(void*, void*));

/**@}*/

// REMOVE FUNCTIONS
/**
   @defgroup remFun Remove Functions
   @{
*/

/**
   @brief A function which removes a node from the list.
   @param root The root pointer of the list.
   @param toRemove A pointer to the node which is to be removed from the list. 
   @return A pointer to the node. NULL if nothing was removed. 
   @bug Can cause problems when removing a node that is not a part of the list, but still points to the list. 
*/
ll_node* LL_removePointer(ll_node** root, ll_node* toRemove);
/**
   @brief A function which removes a node from the list.
   @param root The root pointer of the list.
   @param index The index of the node to remove from the list. 
   @return A pointer to the node. NULL if nothing was removed. 
*/
ll_node* LL_removeIndex(ll_node** root, int index);

/**
   @brief A function which removes a node from a list and frees it. 
   @param root The root pointer of the list. 
   @param toRemove A pointer to the node which is to be deleted from the list.  
   @return A pointer to the content of the removed node.
*/
void* LL_deletePointer(ll_node** root, ll_node* toRemove);

/**
   @brief A function which removes a node from the list.
   @param root The root pointer of the list.
   @param index The index of the node to remove from the list. 
   @return A pointer to the content of the removed node.
*/
void* LL_deleteIndex(ll_node** root, int index);

/**
   @brief A function which removes a node from the list and frees both the node and the content. 
   @param root The root pointer of the list.
   @param toRemove A pointer to the node which is to be deleted from the list.  
*/
void LL_purgePointer(ll_node** root, ll_node* toRemove);

/**
   @brief A function which removes a node from the list and frees both the node and the content. 
   @param root The root pointer of the list.
   @param index The index of the node to remove from the lit. 
*/
void LL_purgeIndex(ll_node** root, int index);

/**
   @brief A function which removes all nodes in a list and frees the nodes. It will also delete the root. 
   @param root The root pointer of the list. 
*/
void LL_deleteList(ll_node** root);

/**
   @brief A function which removes all nodes in a list and frees the nodes and their content. It will also free the root.
   @param root The root pointer of the list. 
   @bug Only use this function if all the nodes contain memory allocated pointers. Stack-pointers will cause seg. faul. 
*/
void LL_purgeList(ll_node** root);

/**
   @brief A function which frees a node and its content.
   @param node A pointer to the node to free. 
   @note Be careful with this function, as it cause completely break a list if used on a node within a list. Use remove/delete/purge index/pointer instead. 
*/
void LL_purgeNode(ll_node* node);

/**@}*/

/************************************/
/*                                  */
/*  MISC FUNCTIONS                  */
/*                                  */
/************************************/
/**
   @defgroup miscFun Miscellaneous Functions
   @{
*/

/**
   @brief A function which moves a node to another part of the list. 
   @bug Not implemented yet. 
*/
ll_node* LL_move();
/**
   @brief A function which searches for a node using the given search function. When The function returns 0 it is considered found. 
   @param root The root pointer of the list.
   @param compareFunction A pointer to a function which takes two void pointers and returns an int. The function should return 0 when the nodes are equal. 
   @return A pointer to the node. NULL if no void is found. 
   @bug Not implemented yet. 
   @todo Implement root**
*/
ll_node* search(ll_node** root, int(*compareFunction)(void*, void*));

/**
   @brief A function which counts the number of nodes in given list.
   @param root The root pointer of the list.
   @return The number of nodes in given list.
*/
int LL_length(ll_node** root);

/**
   @brief A boolean function which checks if the list is empty. 
   @param root The root pointer of the list.
   @return 1 if the root is empty, 0 if it isn't.
*/
int LL_isEmpty(ll_node** root);

/**
   @brief A function which validates the content of the list using validateFunction. The list will be assumed to be valid until proven not to be, in which case the validation check ends. 
   @param validateFunction A function which will validate the content of the node. Should return 0 in the case it is not valid and 1 when it is valid. First void parameter should be the content of the node. Second void pointer should be data needed to validate.
   @param validationData Any data needed to validate. This will be sent as second parameter in validationFunction. 
   @return 1 if the list is valid and 0 if it is not. 
*/
int LL_isValid(ll_node** root, int(*validateFunction)(void*, void*), void* validationData);

/**
   @brief A function which will run mapFunction on every node in the list. 
   @param root The root pointer of the list.
   @param mapFunction A function which runs on the content of a node. Does not return anything so it is mostly a side-effect function. 
*/
void LL_map(ll_node** root, void(*mapFunction)(void*));

/**
   @brief A function which compares content of nodes in list until compareFunction returns 0. compareFunction will use content as one of its parameters and the node's content as the other parameter. 
   @param root The root pointer of the list.
   @param content
   @param compareFunction 
   @return A pointer to the found node
*/
ll_node* LL_search(ll_node** root, void* content, int(*compareFunction)(void*, void*));

/**@}*/ // miscFun
/**@}*/

#endif
