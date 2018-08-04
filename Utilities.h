//
// Created by Polina on 22-Feb-18.
//

#ifndef DEDUPLICATIONPROJ_HEURISTIC_UTILITIES_H
#define DEDUPLICATIONPROJ_HEURISTIC_UTILITIES_H

typedef enum{
    SUCCESS,
    INVALID_INPUT,
    OUT_OF_MEMORY
} ErrorCode;
#include "List.h"
#include "memory_pool.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/********************************************* Definitions& Magic Numbers *********************************************/
/* Magic Numbers */
#define STR_OF_Z 12
#define DIR_NAME_LEN 11
#define DIR_NAME_HASH 10
#define BLOCK_ID_LEN 13
#define FILE_ID_LEN 30
#define BUFFER_SIZE 255
#define LETTERS_CHAR 48
#define LINE_SPACE 10
#define CHUNKE_ID_LEN 10
#define CHUNKE_SIZE_LEN 6
#define DIR_SIZE 0
#define FILE_ATTRIBUTE_DIRECTORY 0x00000010
#define ROOT_ID_LEN 8
#define FILE_NAME_LEN 5
#define FILE_SYSTEM_ID_LEN 3
#define NUM_OF_INPUT_FILE 208
#define MAX_LINE_LEN 1500

/* **************** START **************** block_info struct **************** START **************** */
/*
 * Definition of a block info structure:
 *                  - block_sn -> a running index on all blocks read from the file system
 *                  - block_id -> a hushed id as appears in the input file
 *                  - block_size -> the size of a block
 *                  - shared_by_num_files -> number of files sharing this block
 *                  - files_list -> list of hashed file ids containing this block
 */
struct block_info{ //helper struct
    int size;
    unsigned long block_sn;
};

typedef struct block_info* Block_Info;

static ListElement copy_block_info(ListElement block_info){
    assert(block_info);
    Block_Info bi = (Block_Info)(block_info);

    //allocate space for copy
    Block_Info bi_copy = malloc(sizeof(*bi_copy));
    if(bi_copy == NULL){
        return NULL;
    }

    bi_copy->size = bi->size;
    bi_copy->block_sn = bi->block_sn;

    return bi_copy;
}

static void free_block_info(ListElement block_info){
    free(block_info);
}

/* ***************** END ***************** block_info struct ***************** END ***************** */

#endif //DEDUPLICATIONPROJ_HEURISTIC_UTILITIES_H
