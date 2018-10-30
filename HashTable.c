//
// Created by Michal Amsterdam on 15/08/2018.
//
/* **************************************************** INCLUDES **************************************************** */
#include "HashTable.h"
/* **************************************************** INCLUDES **************************************************** */
/* ******************** START ******************** HashTable Functions ******************** START ******************* */

HashTable ht_create(unsigned int shared_by_num_files , PMemory_pool mem_pool) {
    HashTable ht = NULL;
    /* Allocate the table itself */
    ht = memory_pool_alloc(mem_pool , sizeof(*ht));
    if(!ht){ //check allocation was successful
        return NULL;
    }

    /* Allocate pointers to the head nodes */
    unsigned int size_of_table = sizeof(Entry)*(shared_by_num_files);
    ht->table = memory_pool_alloc(mem_pool , size_of_table);
    if(!(ht -> table)){ //check array of pointers was allocated successfully
        return NULL; //All is allocated in POOL - Nothing to Free
    }

    ht->size_table = shared_by_num_files;
    for(int i = 0; i < (ht->size_table) ; i++ ){
        (ht->table)[i] = NULL;
    }

    return ht;
}

unsigned int ht_hash( HashTable ht, char *key ) {
    unsigned int hashval = 0;
    int i = 0;

    //printf("%s\n" , key);
    /* Convert our string to an integer */
    while((hashval < UINT_MAX) && (i < strlen(key))){
        hashval = hashval << 8;
        hashval += key[i];
        i++;
    }

    return (hashval % (ht->size_table));
}

Entry ht_newpair(char *key , unsigned int data , PMemory_pool mem_pool){
    Entry newpair  = memory_pool_alloc(mem_pool , sizeof(*newpair));
    if(newpair == NULL){
        return NULL;
    }

    newpair->key = memory_pool_alloc(mem_pool , sizeof(char)*(strlen(key)+1));
    if(newpair->key == NULL){
        return NULL; //All is allocated in POOL - Nothing to Free
    }
    newpair->key = strcpy(newpair->key , key);
//    if(data == -1){
//        newpair->data = 0;
//    }else{
//        newpair->data = data;
//    }
    newpair->data = data;
    newpair->next = NULL;
    return newpair;
}

Entry ht_set(HashTable ht, char *key , bool* object_exists, unsigned int data , PMemory_pool mem_pool) {
    Entry newpair = NULL;
    Entry next = NULL;
    Entry last = NULL;

    //Changed From long int to unsigned int - 16.10
    unsigned int hash_key = ht_hash(ht , key);
    next = (ht->table)[hash_key];

    // Advance until get the end of the list OR first matching key
    while( next != NULL && next->key != NULL && strcmp( key, next->key ) != 0 ) {
        last = next;
        next = next->next;
    }

    /* There's already a pair. Let's replace that string. */
    if(next != NULL && next->key != NULL && strcmp( key, next->key ) == 0 && (next->data == data)) {
        //Return the pointer to the Block/File that already exists in the hash
        *object_exists = true;
        return next;
    } else { /* Nope, couldn't find it.  Time to grow a pair. */
        newpair = ht_newpair(key , data , mem_pool); //allocate new pair
        if(newpair == NULL){
            return NULL;
        }
        /* We're at the start of the linked list in this hash_key. */
        if( next == (ht->table)[hash_key] ){ // If we in an empty list
            newpair->next = next;
            (ht->table)[hash_key] = newpair;

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


/* ********************* END ********************* HashTable Functions ********************* END ******************** */

