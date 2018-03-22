//
// Created by Polina on 08-Dec-17.
//

/*----------------- includes & Defines -----------------*/
#include <stdio.h>
#include <stdlib.h>
#include "List.h"

#define LIST_IS_NULL -1
#define INDEX_OF_FIRST 0

typedef struct node {
    ListElement data;
    struct node* next;
}*Node;

//List structure with list functions
struct List_t {
    Node first;
    Node last;
    Node iterator;
    CopyListElement copyElement;
    FreeListElement freeElement;
};
/*-------------- End - includes & Defines --------------*/

/* ---------- STATIC FUNCTION DECLARATIONS ------------ */

static int listGetIndexOfIterator(List list);
static void destroyNodes(Node toDestroy, FreeListElement freeFunction);
static Node nodeCreate(ListElement data, CopyListElement copyFunction);

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
    newList->freeElement = freeElement;
    newList->first = NULL;
    newList->last = NULL;
    newList->iterator = NULL;
    return newList;
}

void listDestroy(List list) {
    if (!list) {
        return;
    }
    destroyNodes(list->first, list->freeElement);
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

ListElement listGetLast(List list) {
    //Check for invalid arguments
    if (!list) {
        return NULL;
    }
    list->iterator = list->last;
    if (!list->iterator || !list->iterator->data) {
        return NULL;
    }
    return list->last->data;
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
    Node newNode = nodeCreate(element, list->copyElement);
    if (!newNode) {
        return LIST_OUT_OF_MEMORY;
    }
    //set next node of new node to the first node of list
    newNode->next = list->first;
    //and set the first node in the list as the new node
    list->first = newNode;
    //if we insert the first element
    //update also the last to point on this element
    if(!list->last){
        list->last = list->first;
    }
    return LIST_SUCCESS;
}

ListResult listInsertLast(List list, ListElement element) {
    //Check for invalid arguments
    if (!list || !element) {
        return LIST_NULL_ARGUMENT;
    }
    //create a new node for insertion in the end
    Node newNode = nodeCreate(element, list->copyElement);
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
    //if we want to update the list iterator TODO - check if hurt the list-foreach in implementation
    list->iterator = list->last; //TODO - might not needed this - so can be in remark
    return LIST_SUCCESS;
}


ListResult listClear(List list) {
    //Check Invalid arguments
    if (list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    //destroy all nodes
    destroyNodes(list->first,list->freeElement);
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
    /* size-nodesToEnd is the index of the iterator
     * example: iterator is at index 3, size is 5
     * nodesToEnd = 2
     * (since indexing starts from 0 we are at the
     * third node so third and fourth are counted)
     * 5-2=3
     */
    return listGetSize(list) - nodesToEnd;
}

/* @param toDestroy - the node from which to begin destroying all nodes after
 * @param freeFunction - the function with which to free the type "ListElement"
 * destroys the set of nodes that begins with the supplied node
 */
static void destroyNodes(Node toDestroy, FreeListElement freeFunction) {
    if (!toDestroy) {
        return;
    }
    destroyNodes(toDestroy->next, freeFunction);
    freeFunction(toDestroy->data);
    free(toDestroy);
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
/*----------------- End-  Functions ----------------*/
