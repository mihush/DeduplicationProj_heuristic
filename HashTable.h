//
// Created by Michal Amsterdam on 15/08/2018.
//

#ifndef DEDUPLICATIONPROJ_HEURISTIC_HASHTABLE_H
#define DEDUPLICATIONPROJ_HEURISTIC_HASHTABLE_H

/* **************************************************** INCLUDES **************************************************** */
#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include "memory_pool.h"
/* **************************************************** INCLUDES **************************************************** */
/* ****************************************************************************************************************** */
/* **************************************************** DEFINES ***************************************************** */
typedef void* DataF;

/* **************************************************** DEFINES ***************************************************** */
/* ****************************************************************************************************************** */
/* ******************* START ******************** HashTable Definition ******************** START ******************* */
struct entryf_t {
    char *key;
    DataF data;
    struct entryf_t *next;//Chain-hashing solution. Pointer to the next element
};
typedef struct entryf_t *EntryF;

struct hashtablef_t {
    unsigned short size_table;
    EntryF *table; // array of pointers to Entries
};
typedef struct hashtablef_t *HashTableF;
/* ********************* END ******************** HashTable Definition********************* END ********************* */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
/* ******************** START ******************** HashTable Functions ******************** START ******************* */
/*
 * ht_createF - creates a hashtable for the requested type of objects (which determines its size)
 *
 * @type - can be one of 3 : 'B' for blocks , 'F' for files and 'D' for directories
 */
HashTableF ht_createF(unsigned short shared_by_num_files, PMemory_pool mem_pool);

/*
 * ht_hashF - Given a key (string) Generates a Hash Value by which it will be stored in the table
 *
 * @ht  - the hashtable in which the key will be saved
 * @key - the key for which we want to get the hashed value in the hashtable
 */
long int ht_hashF( HashTableF ht, char *key );

/*
 * ht_newpair - Creates a key-value pair
 *              the key is the file id and the value is NULL
 */
EntryF ht_newpairF(char *key , PMemory_pool mem_pool);

/*
 *  ht_setF - Insert a key-value pair into a hash table (General function thus
 *
 * @ht  - the hashtable to which the object will be added
 * @key - the hashed id of the file
 */
EntryF ht_setF(HashTableF ht, char *key, bool* object_exists , PMemory_pool mem_pool);

/*
 * ht_getF - Retrieve pointer for file element with corresponding key in hash table
 *
 * @ht  - the hashtable to which the object will be added
 * @key - the hashed id of the object
 */
DataF ht_getF(HashTableF ht, char *key );

/* ********************* END ********************* HashTable Functions ********************* END ******************** */

#endif //DEDUPLICATIONPROJ_HEURISTIC_HASHTABLE_H
