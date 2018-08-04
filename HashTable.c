//
// Created by Polina on 05-Mar-18.
//

#include "HashTable.h"

/*
 * ht_hash - Given a key (string) Generates a Hash Value by which it will be stored in the table
 */
unsigned long ht_hashF( HashTableF ht, unsigned long sn ) {
    return sn % (ht->size_table);
}

/*
 * ht_newpair - Creates a key-value pair
 *                  - For block - size parameter will contain the block size
 *                  - For File - size parameter will be -1
 */
//EntryF ht_newpairF(unsigned long key , DataF bi , char flag){
//    EntryF newpair  = memory_pool_alloc(mem, (sizeof(*newpair)));
//    if(newpair == NULL){
//        return NULL;
//    }
//    newpair->key = key;
//    if( flag == 'B'){
//        newpair->data = copy_block_info(bi);
//    } else { // it's file level
//        newpair->data = NULL;
//    }
//    newpair->next = NULL;
//    return newpair;
//}

/*
 * ht_get - Retrieve pointer for block/file element with corresponding key in hash table.
 */
DataF ht_getF(HashTableF ht, unsigned long key ) {
    long int hash_key = ht_hashF(ht, key);
    EntryF pair = ht->table[hash_key];

    /* Step through the hash_key, looking for our value. */
    while( pair != NULL && key != pair->key) {
        pair = pair->next;
    }

    /* Did we actually find anything? */
    if( pair == NULL || key != pair->key ) {
        //didn't find anything
        return NULL;

    }
    //found the key - return the data
    return pair->data;
}

/*
 * ht_free - freeing all allocations of HashTable.
 */
//void hashTableF_destroy(HashTableF ht , char flag){
//    long num_of_elements = ht->num_of_elements;
//    //long size_of_lists = 0;
//    struct entryf_t* temp_to_free;
//    // Remove lists elements of each HashTable cell
//    for(int i = 0 ; i < num_of_elements ; i++){
//        // free each list element of cell i
//        while(ht->table[i]) {
//            temp_to_free = ht->table[i];
//            ht->table[i] = temp_to_free->next;
//
//            // Destroy elements fields
//            if(flag == 'B') {
//                free_block_info(temp_to_free->data);
//            }
//            free(temp_to_free);
//        }
//        assert(ht->table[i]==NULL);
//    }
//    free(ht->table);
//    free(ht);
//}