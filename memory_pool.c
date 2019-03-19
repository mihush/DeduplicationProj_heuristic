//
// Created by Michal Amsterdam on 01/07/2018.
//
#include "memory_pool.h"

//Creates the struct of a Memory Pool that contains 4 values
void* memory_pool_init(PMemory_pool pool, int bundle_size){
    pool = memset(pool, 0, sizeof(Memory_pool));
    pool->mempool_cnt = 1;
    pool->mempool_init_size = POOL_INITIAL_SIZE_M;
//    if(bundle_size <= 5){
//        pool->mempool_init_size = POOL_INITIAL_SIZE_S;
//    } else {
//        pool->mempool_init_size = POOL_INITIAL_SIZE_M;
//    }
    pool->arr = calloc(1 , sizeof(uint32_t)*(pool->mempool_init_size)); //Allocating the array of the mempool
    return pool;
}

void* memory_pool_alloc(PMemory_pool pool, uint32_t size){
    void* res;
    /* size is in bytes, but we will allocate memory in sizeof(uint32)
     * chunks so we will need to calculate how much to allocate */
    if(pool->mempool_cnt == 0){
        pool->mempool_cnt = 1;
    }
    uint32_t size_in_uint32_uints = (size / sizeof(uint32_t));
    uint32_t size_of_uint32_to_alloc = (size % sizeof(uint32_t)) ? size_in_uint32_uints  + 1 : size_in_uint32_uints;

    PMemory_pool pool_to_alloc_from = pool; // Pointer to the first pool in the chain
    uint32_t pool_to_alloc_from_index = pool->next_free_pool_index; //Index of the next free pool in the chain - usually the last one

    /* we cannot allocate memory larger than the size of an empty pool */
    assert(size_of_uint32_to_alloc < pool_to_alloc_from->mempool_init_size);

    /* find the pool node that has empty memory in it */
    //Go to Last Pool (Which is supposed to have empty place in it)
    if(pool_to_alloc_from_index > 0){
        pool_to_alloc_from = pool->next_free_pool_ptr; //Get the next index of free pool
    }

    /* check that the pool has enough memory to allocate the requested size, if not,
     * allocate a new pool node */
    if (pool_to_alloc_from->mempool_init_size <= (pool->next_free_index + size_of_uint32_to_alloc)){
        pool_to_alloc_from->next_pool = calloc(1 , sizeof(Memory_pool)); //Allocating new pool node
        pool->arr = calloc(1 , sizeof(uint32_t)*(pool->mempool_init_size)); //Allocating the array of the mempool
        (pool->mempool_cnt)++;
        if (!pool_to_alloc_from->next_pool){
            printf("--> Error Allocating REGULAR Memory Pool\n");
            return NULL;
        }
        pool->next_free_index = 0;
        (pool->next_free_pool_index)++;

        pool->next_free_pool_ptr = pool_to_alloc_from->next_pool;
        pool_to_alloc_from = pool->next_free_pool_ptr;
    }

    /* place the address of the memory in res and update the free index */
    res = &(pool_to_alloc_from->arr[pool->next_free_index]);
    pool->next_free_index += size_of_uint32_to_alloc;
    return res;
}

void memory_pool_destroy(PMemory_pool pool) {
    PMemory_pool next_pool = NULL;
    PMemory_pool pool_to_free = pool->next_pool;
    free(pool->arr);
    /* Iterate over all pool nodes and free them */
    while (pool_to_free){
        free(pool_to_free->arr);
        next_pool = pool_to_free->next_pool;
        free(pool_to_free);
        pool_to_free = next_pool;
    }

    return ;
}

//----------- Memory Pool For Merged File GODDD -----------//
//Creates the struct of a Memory Pool that contains 4 values
void* memory_pool_mf_init(PMemory_pool_mf pool, int bundle_size){
    memset(pool, 0, sizeof(Memory_pool_mf));
    pool->mempool_cnt = 1;
    pool->mempool_init_size = POOL_INITIAL_SIZE_MF_L;
//    if(bundle_size <= 10){
//        pool->mempool_init_size = POOL_INITIAL_SIZE_MF_S;
//    } else if(bundle_size <= 25){
//        pool->mempool_init_size = POOL_INITIAL_SIZE_MF_M;
//    } else {
//        pool->mempool_init_size = POOL_INITIAL_SIZE_MF_L;
//    }
    pool->arr = calloc(1 , sizeof(uint32_t)*(pool->mempool_init_size)); //Allocating the array of the mempool

    return pool;
}

void* memory_pool_mf_alloc(PMemory_pool_mf pool, uint32_t size){
    void* res;
    if(pool->mempool_cnt == 0){
        pool->mempool_cnt = 1;
    }
    /* size is in bytes, but we will allocate memory in sizeof(uint32)
     * chunks so we will need to calculate how much to allocate */
    uint32_t size_in_uint32_uints = (size / sizeof(uint32_t));
    uint32_t size_of_uint32_to_alloc = (size % sizeof(uint32_t)) ? size_in_uint32_uints  + 1 : size_in_uint32_uints;

    PMemory_pool_mf pool_to_alloc_from = pool; // Pointer to the first pool in the chain
    uint32_t pool_to_alloc_from_index = pool->next_free_pool_index; //Index of the next free pool in the chain - usually the last one

    /* we cannot allocate memory larger than the size of an empty pool */
    assert(size_of_uint32_to_alloc < pool->mempool_init_size);

    /* find the pool node that has empty memory in it */
    //Go to Last Pool (Which is supposed to have empty place in it)
    if(pool_to_alloc_from_index > 0){
        pool_to_alloc_from = pool->next_free_pool_ptr; //Get the next index of free pool
    }

    /* check that the pool has enough memory to allocate the requested size, if not,
     * allocate a new pool node */
    if (pool->mempool_init_size <= (pool->next_free_index + size_of_uint32_to_alloc)){
        pool_to_alloc_from->next_pool = calloc(1 , sizeof(Memory_pool_mf)); //Allocating new pool node
        pool->arr = calloc(1 , sizeof(uint32_t)*(pool->mempool_init_size)); //Allocating the array of the mempool

        (pool->mempool_cnt)++;
        if (!pool_to_alloc_from->next_pool){
            printf("--> Error Allocating MF Memory Pool\n");
            return NULL;
        }
        pool->next_free_index = 0;
        (pool->next_free_pool_index)++;

        pool->next_free_pool_ptr = pool_to_alloc_from->next_pool;
        pool_to_alloc_from = pool->next_free_pool_ptr;
    }

    /* place the address of the memory in res and update the free index */
    res = &(pool_to_alloc_from->arr[pool->next_free_index]);
    pool->next_free_index += size_of_uint32_to_alloc;
    return res;
}

void memory_pool_mf_destroy(PMemory_pool_mf pool) {
    PMemory_pool_mf next_pool = NULL;
    PMemory_pool_mf pool_to_free = pool->next_pool;
    free(pool->arr);

    /* Iterate over all pool nodes and free them */
    while (pool_to_free){
        free(pool_to_free->arr);
        next_pool = pool_to_free->next_pool;
        free(pool_to_free);
        pool_to_free = next_pool;
    }

    return ;
}

void memory_pool_mf_reset(PMemory_pool_mf pool) {
    PMemory_pool_mf next_pool = NULL;
    PMemory_pool_mf pool_to_free = pool->next_pool;
    int num_of_mempools = pool->mempool_cnt;
    int mem_pool_init_size_old = pool->mempool_init_size;


    free(pool->arr);
    for(int i = 0 ; i < num_of_mempools ; i++){
        if(i == 0){
            pool = memset(pool, 0, sizeof(Memory_pool_mf));
            pool->mempool_init_size = mem_pool_init_size_old;
            pool->arr = calloc(1 , sizeof(uint32_t)*(pool->mempool_init_size)); //Allocating the array of the mempool
        } else {
            free(pool_to_free->arr);
            next_pool = pool_to_free->next_pool;
            free(pool_to_free);
            pool_to_free = next_pool;
        }
    }
    pool->mempool_cnt = 1;

    return ;
}