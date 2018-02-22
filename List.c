//
// Created by Polina on 22-Feb-18.
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
    Node iterator;
    CopyListElement copyElement;
    FreeListElement freeElement;
};
/*-------------- End - includes & Defines --------------*/

/* ---------- STATIC FUNCTION DECLARATIONS ------------ */

static int indexOfMax(ListElement* array, int lengthOfArray, CompareListElements compareElement);
static void maxSort(ListElement* a, int n, CompareListElements compareElement);
static int listGetIndexOfIterator(List list);
static void destroyNodes(Node toDestroy, FreeListElement freeFunction);
static Node nodeCreate(ListElement data, CopyListElement copyFunction);
static Node listGetNodeBeforeIterator(List list);

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
ListResult listRestartIterator(List list) {
    if (!list) {
        return LIST_NULL_ARGUMENT;
    }
    list->iterator = list->first;
    return LIST_SUCCESS;
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

List listFilter(List list, FilterListElement filterElement, ListFilterKey key) {
    //Check for invalid arguments
    if (!list || !filterElement || !key) {
        return NULL;
    }
    List filtered = listCreate(list->copyElement, list->freeElement);
    if (!filtered) {
        return NULL;
    }
    ListResult addResult;
    Node iterator = list->first;
    //check all nodes to see if they fit the key
    //if they do, add them to the filtered list
    while (iterator) {
        if (filterElement(iterator->data, key)) {
            addResult = listInsertLast(filtered, iterator->data);
            if (addResult != LIST_SUCCESS) {
                listDestroy(filtered);
                return NULL;
            }
        }
        iterator = iterator->next;
    }
    filtered->iterator = filtered->first;
    return filtered;
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
    Node newNode = nodeCreate(element, list->copyElement);
    if (!newNode) {
        return LIST_OUT_OF_MEMORY;
    }
    //set next node of new node to the first node of list
    newNode->next = list->first;
    //and set the first node in the list as the new node
    list->first = newNode;
    return LIST_SUCCESS;
}

ListResult listInsertLast(List list, ListElement element) {
    //Check for invalid arguments
    if (!list || !element) {
        return LIST_NULL_ARGUMENT;
    }
    //if first node is null the list is empty so enter new node as first
    Node iterator = list->first;
    if (!iterator) {
        return listInsertFirst(list, element);
    }
    //move to the next until the next node is null
    while (iterator->next) {
        iterator = iterator->next;
    }
    //set the next node, which is null, to be the new node
    iterator->next = nodeCreate(element, list->copyElement);
    if (!iterator->next) {
        return LIST_OUT_OF_MEMORY;
    }
    return LIST_SUCCESS;
}

ListResult listInsertBeforeCurrent(List list, ListElement element) {
    //Check for invalid arguments
    if (list == NULL || element == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    if (!list->iterator) {
        return LIST_INVALID_CURRENT;
    }
    int indexOfIterator = listGetIndexOfIterator(list);

    //if indexOfIterator = 0 then the current node is the first
    if (indexOfIterator == INDEX_OF_FIRST) {
        ListResult InsertResult = listInsertFirst(list, element);
        if (InsertResult != LIST_SUCCESS) {
            return InsertResult;
        }
    } else {
        Node nodeBefore = listGetNodeBeforeIterator(list);
        Node newNode = nodeCreate(element, list->copyElement);
        if (!newNode) {
            return LIST_OUT_OF_MEMORY;
        }
        newNode->next = list->iterator;
        nodeBefore->next = newNode;
    }
    return LIST_SUCCESS;
}

ListResult listInsertAfterCurrent(List list, ListElement element) {
    //Check for invalid arguments
    if (!list || !element) {
        return LIST_NULL_ARGUMENT;
    }
    if (!list->iterator) {
        return LIST_INVALID_CURRENT;
    }

    int size = listGetSize(list);
    int nodesFromEnd = size - listGetIndexOfIterator(list);
    //iterator is not in last place
    if (nodesFromEnd > 0) {
        Node newNode = nodeCreate(element, list->copyElement);
        if (!newNode) {
            return LIST_OUT_OF_MEMORY;
        }
        newNode->next = list->iterator->next;
        list->iterator->next = newNode;
    } else {
        //if iterator is in last place we can simply use listInsertLast
        return listInsertLast(list, element);
    }
    return LIST_SUCCESS;
}

ListResult listRemoveCurrent(List list) {

    //Check for invalid arguments
    if (list == NULL) {
        return LIST_NULL_ARGUMENT;
    }

    //the iterator is invalid - doesn't point to a NODE
    if (list->iterator == NULL) {
        return LIST_INVALID_CURRENT;
    }
    //save the pointer to the Node to remove
    Node toRemove = list->iterator;
    //if toRemove is first node point first to the next node
    if (listGetIndexOfIterator(list) == INDEX_OF_FIRST) {
        list->first = toRemove->next;
    } else {
        Node nodeBefore = listGetNodeBeforeIterator(list);
        //skip the node to be removed
        nodeBefore->next = toRemove->next;
    }
    if (toRemove->data) {
        list->freeElement(toRemove->data);
    }
    free(toRemove);

    // the iterator should point to null at the end
    list->iterator = NULL;

    return LIST_SUCCESS;
}

ListResult listSort(List list, CompareListElements compareElement) {
    //Check for invalid arguments
    if (!list || !compareElement) {
        return LIST_NULL_ARGUMENT;
    }

    int size = listGetSize(list);
    ListElement* array = malloc(size * sizeof(ListElement));
    if (!array) {
        return LIST_OUT_OF_MEMORY;
    }

    //save all the data in each node to an array
    Node iterator = list->first;
    for (int i = 0; i < size; i++) {
        array[i] = list->copyElement(iterator->data);
        iterator = iterator->next;
    }

    //sort the array using compare function supplied for the type of ListElement
    maxSort(array, size, compareElement);

    //save the sorted ListElements into the Nodes in the List
    iterator = list->first;
    for (int i = 0; i < size; i++) {
        list->freeElement(iterator->data);
        iterator->data = array[i];
        iterator = iterator->next;
    }
    free(array);
    return LIST_SUCCESS;
}

ListResult listClear(List list) {
    //Check Invalid arguments
    if (list == NULL) {
        return LIST_NULL_ARGUMENT;
    }
    //destroy all nodes
    destroyNodes(list->first,list->freeElement);
    list->first=NULL;
    list->iterator=NULL;
    return LIST_SUCCESS;
}
/*------------- End - Structure Functions -------------*/

/* ------------------- STATIC FUNCTIONS ---------------------- */

/* @param array - array of ListElements to sort
 * @lengthOfArray - the length of the array
 * @compareElement - function to compare between two ListElements
 * returns the index of the max ListElement in a given array according to a
 * supplied compare function
 */
static int indexOfMax(ListElement* array, int lengthOfArray,
                      CompareListElements compareElement) {
    int indexOfMax = 0;
    for (int i = 1; i < lengthOfArray; i++)
        if (compareElement(*(array + i), *(array + indexOfMax)) > 0) {
            indexOfMax = i;
        }
    return indexOfMax;
}

/* @param array - array of ListElements to sort
 * @lengthOfArray - the length of the array
 * @compareElement - function to compare between two ListElements
 * sorts an array of ListElements according to a supplied function from
 * smallest to biggest
 */
static void maxSort(ListElement* array, int lengthOfArray,
                    CompareListElements compareElement) {
    for (int length = lengthOfArray; length > 1; length--) {
        int indexOfmax = indexOfMax(array, length, compareElement);
        ListElement tmp = *(array + length - 1);
        *(array + length - 1) = *(array + indexOfmax);
        *(array + indexOfmax) = tmp;
    }
}

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

/* @param list the list from which to get the node from
 * returns the node before the node to which the iterator points at
 */
static Node listGetNodeBeforeIterator(List list) {
    int indexOfIterator = listGetIndexOfIterator(list);
    Node nodeBefore = list->first;
    for (; indexOfIterator > 1; indexOfIterator--) {
        nodeBefore = nodeBefore->next;
    }
    return nodeBefore;
}

/*----------------- End-  Functions ----------------*/
