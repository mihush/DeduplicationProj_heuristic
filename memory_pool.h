//
// Created by Michal Amsterdam on 01/07/2018.
//

#ifndef DEDUPLICATIONPROJ_HEURISTIC_MEMORY_POOL_H
#define DEDUPLICATIONPROJ_HEURISTIC_MEMORY_POOL_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>

#define POOL_INITIAL_SIZE 8192*8192
#define POOL_INITIAL_SIZE_MF 1024*1024

//typedef struct memory_pool_t
//{
//    uint32_t next_free_index; //index inside the current pool
//    uint32_t next_free_pool_index; //index inside next pool that is free
//    uint32_t *arr; // array
//    int arr_size; // array of size - configured depends on MF or not
//    struct memory_pool_t* next_pool; //pointer to the next allocated pool
//    struct memory_pool_t* next_free_pool_ptr; //pointer to the next allocated pool
//}Memory_pool, *PMemory_pool;

typedef struct memory_pool_t
{
    uint32_t next_free_index; //index inside the current pool
    uint32_t next_free_pool_index; //index inside next pool that is free
    uint32_t arr[POOL_INITIAL_SIZE]; // array
    struct memory_pool_t* next_pool; //pointer to the next allocated pool
    struct memory_pool_t* next_free_pool_ptr; //pointer to the next allocated pool
}Memory_pool, *PMemory_pool;

typedef struct memory_pool_mf_t
{
    uint32_t next_free_index; //index inside the current pool
    uint32_t next_free_pool_index; //index inside next pool that is free
    uint32_t arr[POOL_INITIAL_SIZE_MF]; // array
    struct memory_pool_mf_t* next_pool; //pointer to the next allocated pool
    struct memory_pool_mf_t* next_free_pool_ptr; //pointer to the next allocated pool
}Memory_pool_mf, *PMemory_pool_mf;


/*
	@Function:	memory_pool_init
	@Params:	pool -	Pointer to the pool struct to initialize.
	@Desc:		Pool struct will be initialized, allocated and the memory would be set to 0.
*/
void* memory_pool_init(PMemory_pool pool);


/*
	@Function:	memory_pool_alloc
	@Params:	pool -	Pointer to the pool from which to allocate memory from.
				size -	The size (in bytes) of memory to allocate.
	@Desc:		Memory of the requested size will be allocated from the pool,
                if required the pool would be extended.
				A pointer to the beginning of the memory would be place in the res pointer.
*/
void* memory_pool_alloc(PMemory_pool pool, uint32_t size);

/*
	@Function:	memory_pool_destroy
	@Params:	pool -	Pointer to the pool to destroy
	@Desc:		All memory of the pool will be freed.
*/
void memory_pool_destroy(PMemory_pool pool);


//----------- Memory Pool For Merged File GODDD -----------//
//Creates the struct of a Memory Pool that contains 4 values
void* memory_pool_mf_init(PMemory_pool_mf pool);

void* memory_pool_mf_alloc(PMemory_pool_mf pool, uint32_t size);

void memory_pool_mf_destroy(PMemory_pool_mf pool);



#endif //DEDUPLICATIONPROJ_HEURISTIC_MEMORY_POOL_H
