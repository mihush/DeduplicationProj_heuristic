//
// Created by Polina on 05-Mar-18.
//

#ifndef DEDUPLICATIONPROJ_HEURISTIC_HASHTABLE_H
#define DEDUPLICATIONPROJ_HEURISTIC_HASHTABLE_H


#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "Utilities.h"
/* **************************************************** INCLUDES **************************************************** */
/* ****************************************************************************************************************** */
/* **************************************************** DEFINES ***************************************************** */
#define GROWTH_FACTOR 2
#define INIT_SIZE 5007
#define BLOCKS_IN_FILE_SIZE 300
#define BLOCKS_INIT_SIZE 20000
//#define BLOCKS_INIT_SIZE 1000000
typedef void* DataF;

struct entryf_t {
    unsigned long key;
    DataF data;
    struct entryf_t *next;//Chain-hashing solution. ptr to the next element
};
typedef struct entryf_t *EntryF;

struct hashtablef_t {
    long size_table;
    long num_of_elements;
    EntryF *table; // array of pointers to Entries
};
typedef struct hashtablef_t *HashTableF;

/* **************** END *************** HashTable Definition **************** END **************** */
/* *********************************************************************************************** */
/* *********************************************************************************************** */
/* *************** START ************** HashTable Functions *************** START **************** */

HashTableF ht_createF();
unsigned long ht_hashF(HashTableF ht, unsigned long sn);
EntryF ht_newpairF(unsigned long key , DataF bi , char flag);
EntryF ht_setF(HashTableF ht, unsigned long key , DataF bi , char flag , bool* object_exists);
DataF ht_getF(HashTableF ht, unsigned long key );
void hashTableF_destroy(HashTableF ht , char flag);

#endif //DEDUPLICATIONPROJ_HEURISTIC_HASHTABLE_H
