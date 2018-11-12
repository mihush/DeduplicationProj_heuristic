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

/* **************************************************** DEFINES ***************************************************** */
/* ****************************************************************************************************************** */
/* ******************* START ******************** HashTable Definition ******************** START ******************* */
struct entry_t {
    char *key;
    unsigned int data;
    struct entry_t *next;//Chain-hashing solution. Pointer to the next element
};
typedef struct entry_t *Entry;

struct hashtable_t {
    unsigned int size_table;
    Entry *table; // array of pointers to Entries
};
typedef struct hashtable_t *HashTable;
/* ********************* END ******************** HashTable Definition********************* END ********************* */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
/* ******************** START ******************** HashTable Functions ******************** START ******************* */
/*
 * ht_createF - creates a hashtable for the requested type of objects (which determines its size)
 *
 * @type - can be one of 3 : 'B' for blocks , 'F' for files and 'D' for directories
 */
HashTable ht_create(unsigned int shared_by_num_files, PMemory_pool mem_pool, PMemory_pool_mf mem_pool_mf, bool isMerged);

/*
 * ht_hashF - Given a key (string) Generates a Hash Value by which it will be stored in the table
 *
 * @ht  - the hashtable in which the key will be saved
 * @key - the key for which we want to get the hashed value in the hashtable
 */
unsigned int ht_hash( HashTable ht, char *key );

/*
 * ht_newpair - Creates a key-value pair
 *              the key is the file id and the value is NULL
 */
Entry ht_newpair(char *key , unsigned int data , PMemory_pool mem_pool, PMemory_pool_mf mem_pool_mf, bool isMerged);

/*
 *  ht_setF - Insert a key-value pair into a hash table (General function thus
 *
 * @ht  - the hashtable to which the object will be added
 * @key - the hashed id of the file
 */
Entry ht_set(HashTable ht, char *key, bool* object_exists , unsigned int data ,
        PMemory_pool mem_pool, PMemory_pool_mf mem_pool_mf, bool isMerged);

/* ********************* END ********************* HashTable Functions ********************* END ******************** */

#endif //DEDUPLICATIONPROJ_HEURISTIC_HASHTABLE_H
