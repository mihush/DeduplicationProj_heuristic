//
// Created by Michal Amsterdam on 15/08/2018.
//

/*----------------- includes & Defines -----------------*/
#include "List.h"

#define LIST_IS_NULL -1

typedef struct node {
    ListElement data;
    struct node* next;
}*Node;

//List structure with list functions
struct List_t {
    Node first;
    Node last;
    Node iterator;
    int size;
    CopyListElement copyElement;
    CopyListElement_pool copyElement_pool;
    FreeListElement freeElement;
};
/*-------------- End - includes & Defines --------------*/

/* ---------- STATIC FUNCTION DECLARATIONS ------------ */

static int listGetIndexOfIterator(List list);
static void destroyNodes_NonRec(Node toDestroy, FreeListElement freeFunction);
static Node nodeCreate(ListElement data, CopyListElement copyFunction);
static Node nodeCreate_pool(ListElement data, CopyListElement_pool copyFunction , PMemory_pool mem_pool);

/* ----END---- STATIC FUNCTION DECLARATIONS ----END---- */
/*---------------- Structure Functions ----------------*/
List listCreate(CopyListElement copyElement, FreeListElement freeElement) {
    //Check for invalid arguments
    if (!copyElement || !freeElement) {
        return NULL;
    }
    List newList = malloc(sizeof(*newList));
    if (!newList) {
        return NULL;
    }
    newList->copyElement = copyElement;
    newList->copyElement_pool = NULL;
    newList->freeElement = freeElement;
    newList->first = NULL;
    newList->last = NULL;
    newList->iterator = NULL;
    newList->size = 0;
    return newList;
}

List listCreate_pool(CopyListElement_pool copyElement , FreeListElement freeElement , PMemory_pool mem_pool) {
    //Check for invalid arguments
    if (!copyElement || !freeElement) {
        return NULL;
    }

    List newList = memory_pool_alloc(mem_pool , sizeof(*newList));
    if (!newList) {
        return NULL;
    }
    newList->copyElement_pool = copyElement;
    newList->copyElement = NULL;
    newList->freeElement = freeElement;
    newList->first = NULL;
    newList->last = NULL;
    newList->iterator = NULL;
    newList->size = 0;
    return newList;
}

void listDestroy(List list) {
    if (!list) {
        return;
    }
    destroyNodes_NonRec(list->first, list->freeElement);
    free(list);
}

List listCopy(List list) {
    //Check for invalid arguments
    if (!list) {
        return NULL;
    }
    List copyList = listCreate(list->copyElement, list->freeElement);
    if (!copyList) {
        return NULL;
    }
    ListResult addResult;
    int size = listGetSize(list);
    Node iterator = list->first;
    //copy nodes one by one
    for (; size > 0; size--) {
        addResult = listInsertLast(copyList, iterator->data);
        // if failed to copy, destroy list to avoid leaks
        if (addResult != LIST_SUCCESS) {
            listDestroy(copyList);
            return NULL;
        }
        iterator = iterator->next;
    }
    //make iterator of copied list point to the same node as original list
    if (list->iterator) {
        copyList->iterator = copyList->first;
        int indexOfIterator = listGetIndexOfIterator(list);
        for (; indexOfIterator > 0; indexOfIterator--) {
            copyList->iterator = copyList->iterator->next;
        }
    }
    return copyList;
}

int listGetSize(List list) {
    //Check for invalid arguments
    if (list == NULL) {
        return LIST_IS_NULL;
    }
    int size = 0;
    Node iterator = list->first;
    //count number of nodes until reaching NULL
    while (iterator != NULL) {
        size++;
        iterator = iterator->next;
    }
    return size;
}

ListElement listGetFirst(List list) {
    //Check for invalid arguments
    if (!list) {
        return NULL;
    }
    list->iterator = list->first;
    if (!list->iterator || !list->iterator->data) {
        return NULL;
    }
    return list->first->data;
}

ListElement listGetNext(List list) {
    //Check for invalid arguments
    if (!list || !list->iterator) {
        return NULL;
    }
    list->iterator = list->iterator->next;
    //current node is the "next" node therefore we can call listGetCurrent
    //if we reached the end of the list - the next node will be NULL and
    //listGetCurrent will return NULL
    return listGetCurrent(list);
}

ListElement listGetCurrent(List list) {
    //Check for invalid arguments
    if (!list || !list->iterator) {
        return NULL;
    }
    return list->iterator->data;
}

ListResult listInsertFirst(List list, ListElement element) {
    //NULL Arguments in the function
    if (list == NULL || element == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    //create a new node for insertion in the beginning
    Node newNode = nodeCreate(element , list->copyElement);
    if (!newNode) {
        return LIST_OUT_OF_MEMORY;
    }
    //set next node of new node to the first node of list
    newNode->next = list->first;
    //and set the first node in the list as the new node
    list->first = newNode;
    //if we insert the first element - update also the last to point on this element
    if(!list->last){
        list->last = list->first;
    }
    (list->size)++;
    return LIST_SUCCESS;
}

ListResult listInsertFirst_pool(List list, ListElement element , PMemory_pool mem_pool) {
    //NULL Arguments in the function
    if (list == NULL || element == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    //create a new node for insertion in the beginning
    Node newNode = nodeCreate_pool(element , list->copyElement_pool , mem_pool);
    if (!newNode) {
        return LIST_OUT_OF_MEMORY;
    }
    //set next node of new node to the first node of list
    newNode->next = list->first;
    //and set the first node in the list as the new node
    list->first = newNode;
    //if we insert the first element - update also the last to point on this element
    if(!list->last){
        list->last = list->first;
    }
    (list->size)++;
    return LIST_SUCCESS;
}

ListResult listInsertLast(List list, ListElement element) {
    //Check for invalid arguments
    if (!list || !element) {
        return LIST_NULL_ARGUMENT;
    }
    //create a new node for insertion in the end
    Node newNode = nodeCreate(element , list->copyElement);
    if (!newNode) {
        return LIST_OUT_OF_MEMORY;
    }
    //if we insert the first element
    if(!list->first && !list->last){
        //set next node of new node to the first node of list
        newNode->next = NULL;
        //and set the first node in the list as the new node
        list->last = newNode;
        list->first = list->last;
    } else { //one or more elements
        if(!list->last){ //sanity check we don't try to go to NULL arg
            return LIST_NULL_ARGUMENT;
        }
        newNode->next = NULL;
        list->last->next = newNode;
        list->last = newNode;
    }
    //if we want to update the list iterator
    list->iterator = list->last;
    (list->size)++;
    return LIST_SUCCESS;
}

ListResult listInsertLast_pool(List list, ListElement element, PMemory_pool mem_pool) {
    //Check for invalid arguments
    if (!list || !element) {
        return LIST_NULL_ARGUMENT;
    }
    //create a new node for insertion in the end
    Node newNode = nodeCreate_pool(element , list->copyElement_pool , mem_pool);
    if (!newNode) {
        return LIST_OUT_OF_MEMORY;
    }
    //if we insert the first element
    if(!list->first && !list->last){
        //set next node of new node to the first node of list
        newNode->next = NULL;
        //and set the first node in the list as the new node
        list->last = newNode;
        list->first = list->last;
    } else { //one or more elements
        if(!list->last){ //sanity check we don't try to go to NULL arg
            return LIST_NULL_ARGUMENT;
        }
        newNode->next = NULL;
        list->last->next = newNode;
        list->last = newNode;
    }
    //if we want to update the list iterator
    list->iterator = list->last;
    (list->size)++;
    return LIST_SUCCESS;
}

ListResult listClear(List list) {
    //Check Invalid arguments
    if (list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    //destroy all nodes
    destroyNodes_NonRec(list->first,list->freeElement);
    list->first = NULL;
    list->last = NULL;
    list->iterator = NULL;
    return LIST_SUCCESS;
}

/*------------- End - Structure Functions -------------*/

/* ------------------- STATIC FUNCTIONS ---------------------- */

/* @param list - the list from which to get the index of the iterator from
 * returns the index of the node in the list to which the iterator points to
 */
static int listGetIndexOfIterator(List list) {
    //calculate how many nodes from the iterator to the end(including)
    int nodesToEnd = 0;
    Node temp = list->iterator;
    while (temp) {
        nodesToEnd++;
        temp = temp->next;
    }
    /* size-nodesToEnd is the index of the iterator*/
    return listGetSize(list) - nodesToEnd;
}

static void destroyNodes_NonRec(Node toDestroy, FreeListElement freeFunction) {
    if (!toDestroy) {
        return;
    }

    Node current = toDestroy;
    Node next;
    while(current != NULL){
        next = current->next;
        freeFunction(current->data);
        free(current);
        current = next;
    }
}

/* @param data - the data to add to the node
 * @param copyFunction the function with which to copy the ListElement
 * Creates a new node with the supplied data using the copy function suitable
 * for the type of ListElement
 */
static Node nodeCreate(ListElement data, CopyListElement copyFunction) {
    if (!data) {
        return NULL;
    }
    Node newNode = malloc(sizeof(*newNode));

    if (!newNode) {
        return NULL;
    }
    newNode->data = copyFunction(data);
    if (!newNode->data) {
        free(newNode);
        return NULL;
    }
    newNode->next = NULL;
    return newNode;
}

static Node nodeCreate_pool(ListElement data, CopyListElement_pool copyFunction , PMemory_pool mem_pool) {
    if (!data) {
        return NULL;
    }

    Node newNode = memory_pool_alloc(mem_pool , sizeof(*newNode));
    if (!newNode) {
        return NULL;
    }
    newNode->data = copyFunction(data , mem_pool);
    if (!newNode->data) {
        free(newNode);
        return NULL;
    }
    newNode->next = NULL;
    return newNode;
}
/*----------------- End-  Functions ----------------*/
