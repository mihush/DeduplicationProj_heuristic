//
// Created by Polina on 05-Mar-18.
//

#include "HashTable.h"


/* Create a new HashTable. */
HashTableF ht_createF() {
    HashTableF ht = NULL;

    /* Allocate the table itself */
    ht = malloc(sizeof(*ht));
    if(!ht){ //check allocation was successful
        printf("(HashTableF)--> Creating HashTable - Allocation Error (1) \n");
        return NULL;
    }

    ht->size_table = BLOCKS_INIT_SIZE;
    ht->num_of_elements = 0;

    /* Allocate pointers to the head nodes */
    ht -> table = malloc(sizeof(EntryF) * (ht->size_table));
    if(!ht -> table ){ //check array od pointers was allocated successfully
        printf("(HashTableF)--> Creating HashTable - Allocation Error (2) \n");
        free(ht);
        return NULL;
    }

    for(int i = 0; i < (ht->size_table) ; i++ ){
        ht->table[i] = NULL;
    }

    return ht;
}

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
EntryF ht_newpairF(unsigned long key , DataF bi , char flag){
    EntryF newpair  = malloc(sizeof(*newpair));
    if(newpair == NULL){
        return NULL;
    }
    newpair->key = key;
    if( flag == 'B'){
        newpair->data = copy_block_info(bi);
    } else { // it's file level
        newpair->data = NULL;
    }
    newpair->next = NULL;
    return newpair;
}

/*
 * ht_set - Insert a key-value pair into a hash table.
 */
EntryF ht_setF(HashTableF ht, unsigned long key , DataF bi , char flag , bool* object_exists) {
    EntryF newpair = NULL;
    EntryF next = NULL;
    EntryF last = NULL;

    //unsigned long key = bi->block_sn;
    long int hash_key = ht_hashF( ht , key);
    next = ht->table[hash_key];

    /* Advance until get the end of the list OR first matching key*/
    while( next != NULL && key != next->key ) {
        last = next;
        next = next->next;
    }

    /* There's already a pair. Let's replace that string. */
    if( next != NULL && key == next->key ) {
        //Return the pointer to the Block/File that already exists in the hash
        *object_exists = true;
        return next;
    } else { /* Nope, could't find it.  Time to grow a pair. */
        newpair = ht_newpairF(key , bi , flag); //allocate new pair
        if(newpair == NULL){
            printf("(HashTableF)--> Adding Pair to HT - Allocation Error (1) \n");
            return NULL;
        }
        /* We're at the start of the linked list in this hash_key. */
        if( next == ht->table[hash_key] ){ // If we in an empty list
            newpair->next = next;
            ht->table[hash_key] = newpair;

            /* We're at the end of the linked list in this hash_key. */
        } else if ( next == NULL ) {
            last->next = newpair;

        } else  { /* We're in the middle of the list. */
            newpair->next = next;
            last->next = newpair;
        }
        return newpair;
    }
}

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
void hashTableF_destroy(HashTableF ht , char flag){
    long num_of_elements = ht->num_of_elements;
    //long size_of_lists = 0;
    struct entryf_t* temp_to_free;
    // Remove lists elements of each HashTable cell
    for(int i = 0 ; i < num_of_elements ; i++){
        // free each list element of cell i
        while(ht->table[i]) {
            temp_to_free = ht->table[i];
            ht->table[i] = temp_to_free->next;

            // Destroy elements fields
            if(flag == 'B') {
                free_block_info(temp_to_free->data);
            }
            free(temp_to_free);
        }
        assert(ht->table[i]==NULL);
    }
    free(ht->table);
    free(ht);
}