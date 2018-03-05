////
//// Created by Polina on 22-Feb-18.
////
//
//#ifndef DEDUPLICATIONPROJ_HEURISTIC_HASHTABLE_H
//#define DEDUPLICATIONPROJ_HEURISTIC_HASHTABLE_H
//
///* *************** START ************** HashTable Definition *************** START *************** */
///****************************** INCLUDES *****************************/
//#include <stdlib.h>
//#include <stdio.h>
//#include <limits.h>
//#include <string.h>
//
//#include "List.h"
//
//
///****************************** DEFINES *****************************/
//#define INIT_SIZE 5007
//#define BLOCKS_INIT_SIZE 20000
//#define FILES_INIT_SIZE 10000
//#define DIRS_INIT_SIZE 6000
//#define PHYSICAL_FILES_SIZE 10000
//typedef void* Data;
//
//struct entry_t {
//    char *key;
//    Data data;
//    struct entry_t *next;//Chain-hashing solution. ptr to the next element
//};
//typedef struct entry_t *Entry;
//
//struct hashtable_t {
//    long size_table;
//    long num_of_elements;
//    Entry *table; // array of pointers to Entries
//};
//typedef struct hashtable_t *HashTable;
//
///* **************** END *************** HashTable Definition **************** END **************** */
///* *********************************************************************************************** */
///* *********************************************************************************************** */
///* *************** START ************** HashTable Functions *************** START **************** */
///* Create a new HashTable */
//HashTable ht_create(char type) {
//    HashTable ht = NULL;
//    /* Allocate the table itself */
//    ht = malloc(sizeof(*ht));
//    if(!ht){ //check allocation was successful
//        printf("(HashTable)--> Creating HashTable - Allocation Error (1) \n");
//        return NULL;
//    }
//    switch(type){
//        case 'B': //Blocks
//            ht->size_table = BLOCKS_INIT_SIZE;
//            break;
//        case 'F': //Logical Files
//            ht->size_table = FILES_INIT_SIZE;
//            break;
//        case 'D': //Directories
//            ht->size_table = DIRS_INIT_SIZE;
//            break;
//        default:
//            ht->size_table = INIT_SIZE; //Shouldn't really get here
//            break;
//    }
//    ht->num_of_elements = 0;
//
//    /* Allocate pointers to the head nodes */
//    ht -> table = malloc(sizeof(Entry) * (ht->size_table));
//    if(!ht -> table ){ //check array od pointers was allocated successfully
//        printf("(HashTable)--> Creating HashTable - Allocation Error (2) \n");
//        free(ht);
//        return NULL;
//    }
//
//    for(int i = 0; i < (ht->size_table) ; i++ ){
//        ht->table[i] = NULL;
//    }
//    printf("(HashTable)--> Created HashTable Successfully of size %lu \n" , ht->size_table);
//    return ht;
//}
//
///*
// * ht_hash - Given a key (string) Generates a Hash Value by which it will be stored in the table
// */
//long int ht_hash( HashTable ht, char *key ) {
//    unsigned long int hashval;
//    int i = 0;
//
//    /* Convert our string to an integer */
//    while((hashval < ULONG_MAX) && (i < strlen(key))){
//        hashval = hashval << 8;
//        hashval += key[i];
//        i++;
//    }
//
//    return hashval % (ht->size_table);
//}
//
///*
// * ht_newpair - Creates a key-value pair
// *                  - For block - size parameter will contain the block size
// *                  - For File - size parameter will be -1
// */
//Entry ht_newpair(char *key, unsigned int depth , unsigned long sn , unsigned int size , char flag ,
//                 unsigned long physical_sn){
//    Entry newpair  = malloc(sizeof(*newpair));
//    if(newpair == NULL){
//        printf("(HashTable)--> Creating new pair - Allocation Error (1) \n");
//        return NULL;
//    }
//
//    newpair->key = malloc(sizeof(char)*(strlen(key)+1));
//    if(newpair->key == NULL){
//        printf("(HashTable)--> Creating new pair - Allocation Error (2) \n");
//        free(newpair);
//        return NULL;
//    }
//    strcpy(newpair->key , key);
//
//    if(flag == 'B'){ // save the data object
//        printf("(HashTable)--> Creating new pair - BLOCK \n");
//        newpair->data = block_create(key , sn, size);
//    }else if( flag == 'D'){
//        printf("(HashTable)--> Creating new pair - DIRECTORY \n");
//        newpair->data = dir_create(key , depth , sn);
//    }
//    else if(flag == 'F'){ //This is a file object
//        printf("(HashTable)--> Creating new pair - FILE \n");
//        newpair->data = file_create(key , depth , sn , size , physical_sn);
//    }
//
//    if(newpair->data == NULL) {
//        printf("(HashTable)--> Failed to allocate new pair \n");
//        free(newpair->key);
//        free(newpair);
//        return NULL;
//    }
//    newpair->next = NULL;
//    return newpair;
//}
//
///*
// * ht_set - Insert a key-value pair into a hash table.
// *          possible flags are:
// *              - F - for logical files
// *              - B - for blocks
// *              - D - for directories
// *              - P - for
// */
//Data ht_set(HashTable ht, char *key, unsigned int depth , unsigned long sn , unsigned int size , char flag,
//            bool* object_exists , unsigned long physical_sn) {
//    Entry newpair = NULL;
//    Entry next = NULL;
//    Entry last = NULL;
//
//    long int hash_key = ht_hash( ht , key );
//    next = ht->table[hash_key];
//
//    /* Advance until get the end of the list OR first matching key*/
//    while( next != NULL && next->key != NULL && strcmp( key, next->key ) != 0 ) {
//        last = next;
//        next = next->next;
//    }
//
//    /* There's already a pair. Let's replace that string. */
//    if( next != NULL && next->key != NULL && strcmp( key, next->key ) == 0 ) {
//        //Return the pointer to the Block/File that already exists in the hash
//        *object_exists = true;
//        return next->data;
//    } else { /* Nope, could't find it.  Time to grow a pair. */
//        newpair = ht_newpair(key, depth , sn, size, flag , physical_sn); //allocate new pair
//        if(newpair == NULL){
//            printf("(HashTable)--> Adding Pair to HT - Allocation Error (1) \n");
//            return NULL;
//        }
//        /* We're at the start of the linked list in this hash_key. */
//        if( next == ht->table[hash_key] ){ // If we in an empty list
//            newpair->next = next;
//            ht->table[hash_key] = newpair;
//
//            /* We're at the end of the linked list in this hash_key. */
//        } else if ( next == NULL ) {
//            last->next = newpair;
//
//        } else  { /* We're in the middle of the list. */
//            newpair->next = next;
//            last->next = newpair;
//        }
//        return newpair->data;
//    }
//}
//
///*
// * ht_get - Retrieve pointer for block/file element with corresponding key in hash table.
// */
//Data ht_get( HashTable ht, char *key ) {
//    long int hash_key = ht_hash(ht, key);
//    Entry pair = ht->table[hash_key];
//
//    /* Step through the hash_key, looking for our value. */
//    while( pair != NULL && pair->key != NULL && strcmp( key, pair->key ) != 0 ) {
//        pair = pair->next;
//    }
//
//    /* Did we actually find anything? */
//    if( pair == NULL || pair->key == NULL || strcmp( key, pair->key ) != 0 ) {
//        //didn't find anything
//        return NULL;
//
//    }
//    //found the key - return the data
//    return pair->data;
//}
//
///*
// * Destroy the data obj
// */
//void data_destroy(Data data, char flag){
//    switch (flag){
//        case 'F':
//            file_destroy((File)data);
//            break;
//        case 'D':
//            dir_destroy((Dir)data);
//            break;
//        case 'B':
//            block_destroy((Block)data);
//            break;
//            //TODO add case of Physical file
//    }
//}
//
///*
// * ht_free - freeing all allocations of HashTable.
// */
//void hashTable_destroy(HashTable ht , char flag){
//    long num_of_elements = ht->num_of_elements;
//    //long size_of_lists = 0;
//    struct entry_t* temp_to_free;
//    // Remove lists elements of each HashTable cell
//    for(int i = 0 ; i < num_of_elements ; i++){
//        // free each list element of cell i
//        while(ht->table[i]) {
//            temp_to_free = ht->table[i];
//            ht->table[i] = temp_to_free->next;
//
//            // Destroy elements fields
//            data_destroy(temp_to_free->data , flag);
//            free(temp_to_free->key);
//            free(temp_to_free);
//        }
//        assert(ht->table[i] == NULL);
//    }
//    free(ht->table);
//    free(ht);
//}
//
///*
// * file_compare_to_File - files are considered identical if have the same blocks
// *                          1 - check sizes
// *                          2 - check amount of blocks
// *                          3 - check first block , second block, etc ....
// *                         returns false if physical file already exists
// *                         returns true if no physical file exists
// */
////compare between two files (files are considered identical if have the same blocks)
////bool file_compare(HashTable ht_files , HashTable ht_physical_files , File file ,
////                  unsigned long* physical_files_sn){
////    assert(file);
////    Entry pair = NULL;
////    bool physical_file_exist = true;
////    printf("File to check is: %s \n", file->file_id);
////    for(int i = 0 ; i < (ht_files->size_table) ;i++){
////        pair = ht_files->table[i];
////        while( pair != NULL && pair->key != NULL) {
////            File temp_file = ((File)(pair->data));
////            printf("Current file checked is: %s\n", temp_file->file_id);
////            if(strcmp(file->file_id , temp_file->file_id) == 0){ //It's the same file
////                pair = pair->next;
////                continue;
////            }
////            if(file->file_size != temp_file->file_size){ //Compare by sizes
////                pair = pair->next;
////                continue;
////            }
////            printf("---> sizes are the same\n");
////            if(file->num_blocks != temp_file->num_blocks){ //Compare by amount of blocks
////                pair = pair->next;
////                continue;
////            }
////            printf("---> Amount of blocks is the same\n");
////
////            //Compare each block
////            Object_Info temp_oi;
////            Block_Info temp_file_blocks = listGetFirst(temp_file->blocks_list);
////            LIST_FOREACH(Block_Info , iter ,file->blocks_list){
////                if(strcmp(iter->id , temp_file_blocks->id) != 0){
////                    temp_oi = listGetFirst(temp_file->blocks_list);
////                    temp_oi = listGetFirst(file->blocks_list);
////                    physical_file_exist = false;
////                    break;
////                }
////                temp_file_blocks = listGetNext(temp_file->blocks_list);
////            }
////            temp_oi = listGetFirst(temp_file->blocks_list);
////            temp_oi = listGetFirst(file->blocks_list);
////            if(physical_file_exist == true) { // physical file already exits
////                printf("---> found an identical file\n");
////                file_set_logical_flag(file);
////                ht_setF(temp_file->files_ht, file->file_id);
////                (temp_file->num_files)++;
////                file_set_physical_sn(file , temp_file->physical_sn);
////                (*physical_files_sn)--;
////            }
////            pair = pair->next;
////        }
////    }
////
////    return true;
////}
//
//Data file_compare(HashTable ht_files , HashTable ht_physical_files , File file ,File file_obj_p,
//                  unsigned long* physical_files_sn){
//    assert(file && file_obj_p);
//    File temp_file = NULL;
//    bool physical_file_exist = false ,  blocks_differ = false;
//    Block_Info first_block = (Block_Info)listGetFirst(file->blocks_list);
//    char* first_block_id = first_block->id;
//    long int hash_key = ht_hash(ht_physical_files , first_block_id);
//    printf("--> File to check is: %s \n", file->file_id);
//    printf("----> first block id is: %s \n", first_block_id);
//
//    Entry current = ht_physical_files->table[hash_key]; //get the cell int the hashtable for the possible file
//    if(current == NULL){
//        printf("---> hashing key is : %d\nThe list is empty =[ \n" , hash_key);
//    }
//    //go over all files in the cell found above
//    while(current != NULL && current->key != NULL){
//        temp_file = ((File)(current->data));
//        printf("Current file checked is: %s\n", temp_file->file_id);
//        if(strcmp(file->file_id , temp_file->file_id) == 0){ //It's the same file
//            current = current->next;
//            continue;
//        }
//        if(file->file_size != temp_file->file_size){ //Compare by sizes
//            current = current->next;
//            continue;
//        }
//        printf("---> sizes are the same\n");
//        if(file->num_blocks != temp_file->num_blocks){ //Compare by amount of blocks
//            current = current->next;
//            continue;
//        }
//        printf("---> Amount of blocks is the same\n");
//
//        //Compare each block
//        Object_Info temp_oi;
//        Block_Info temp_file_blocks = listGetFirst(temp_file->blocks_list);
//        LIST_FOREACH(Block_Info , iter ,file->blocks_list){
//            if(strcmp(iter->id , temp_file_blocks->id) != 0){
//                temp_oi = listGetFirst(temp_file->blocks_list);
//                temp_oi = listGetFirst(file->blocks_list);
//                blocks_differ = true; // the blocks aren't the same
//                break;
//            }
//            temp_file_blocks = listGetNext(temp_file->blocks_list);
//        }
//        temp_oi = listGetFirst(temp_file->blocks_list);
//        temp_oi = listGetFirst(file->blocks_list);
//        if(blocks_differ == true){
//            //advance to the next cell
//            current = current->next;
//            continue;
//        } else { // We have found a match
//            physical_file_exist = true;
//            break;
//        }
//    } /* Finished searching for a physical file*/
//
//    /*-------------------- Adding the file to hash table -----------------------------*/
//    if(physical_file_exist == true) { // physical file already exits - add file to ht_files only
//        printf("---> found an identical physical file\n");
//        file_set_logical_flag(file);
//        ht_setF(temp_file->files_ht, file->file_id); // add logical file to the files ht of the pyhsical we found
//        (temp_file->num_files)++;
//        file_set_physical_sn(file , temp_file->physical_sn); // set the physical sn of the logical file to be the one of the physical stored
//        (*physical_files_sn)--;
//
//    } else { //add file only to ht_physical_files and to ht_files
//        // hash by first block id
//        hash_key = ht_hash(ht_physical_files , first_block_id);
//        printf(" --> Added file to physical ht\n---> hashing key is : %d\n", hash_key);
//        Entry ent = ht_physical_files->table[hash_key];
//
//        Entry newpair  = malloc(sizeof(*newpair));
//        assert(newpair);
//        newpair->key = malloc(sizeof(char)*(strlen(first_block_id)+1));
//        assert(newpair->key);
//        newpair->key = strcpy(newpair->key , first_block_id);
//        newpair->data = file_obj_p;
//
//        //Add the file in the head of the list
//        newpair->next = ent;
//        ht_physical_files->table[hash_key] = newpair;
//    }
//
//    /*-------------------- Adding the file to logical hash table anyway -----------------------------*/
//    printf("--> Now adding the logical file to the ht_files\n");
//    char* key = malloc(sizeof(char)*(strlen(file->file_id)+1));
//    strcpy(key , file->file_id);
//    Entry newpair_l  = malloc(sizeof(*newpair_l));
//    if(newpair_l == NULL){
//        printf("(HashTable)--> Adding Pair to HT - Allocation Error (1) \n");
//        return NULL;
//    }
//    newpair_l->key = malloc(sizeof(char)*(strlen(file->file_id)+1));
//    newpair_l->key = strcpy(newpair_l->key , file->file_id);
//    newpair_l->data = file;
//
//    Entry curr = NULL , last = NULL;
//    long int hash_key_f = ht_hash( ht_files , file->file_id );
//    curr = ht_files->table[hash_key_f];
//    /* Advance until get the end of the list OR first matching key*/
//    while( curr != NULL ) {
//        if(curr->key != NULL){
//            break;
//        }
//        if(strcmp( key, curr->key ) != 0){
//            printf("--> (1) - %s\n", curr->key );
//            last = curr;
//            curr = curr->next;
//        } else{
//            printf("--> (7) \n");
//            break;
//        }
//    }
//    printf("--> (1.1)\n");
//    /* There's already a pair. Let's replace that string. */
//    if( curr != NULL && curr->key != NULL && strcmp( key, curr->key ) == 0 ) {
//        //Return the pointer to the Block/File that already exists in the hash
//        printf("--> (2)\n");
//        return curr->data;
//    } else { /* Nope, could't find it.  Time to grow a pair. */
//        printf("--> (3)\n");
//        /* We're at the start of the linked list in this hash_key. */
//        if( curr == ht_files->table[hash_key_f] ){ // If we in an empty list
//            printf("--> (4)\n");
//            newpair_l->next = curr;
//            ht_files->table[hash_key_f] = newpair_l;
//
//            /* We're at the end of the linked list in this hash_key. */
//        } else if ( curr == NULL ) {
//            printf("--> (5)\n");
//            last->next = newpair_l;
//
//        } else  { /* We're in the middle of the list. */
//            printf("--> (6)\n");
//            newpair_l->next = curr;
//            last->next = newpair_l;
//        }
//        return newpair_l->data;
//    }
//}
//
///* **************** END *************** HashTable Functions **************** END ***************** */
//
//
//#endif //DEDUPLICATIONPROJ_HEURISTIC_HASHTABLE_H
