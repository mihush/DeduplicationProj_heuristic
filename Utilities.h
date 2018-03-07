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
#define NUM_OF_INPUT_FILE 208
#define MAX_LINE_LEN 15000
/* **************** START **************** object_info struct **************** START **************** */

///*
// * Definition of a object_info structure:
// *
// *
// *
// *
// *
// *
// */
//struct object_info{ //helper struct
//    char* object_id;
//    unsigned long object_sn;
//    char* parent_dir_id;
//    char object_type; //can be either 'F' or 'D'
//};
//typedef struct object_info* Object_Info;
//
//
//Object_Info object_info_create(char* id , unsigned long sn , char* parent_id , char type){
//    Object_Info oi = malloc(sizeof(*oi));
//    if(oi == NULL){
//        return NULL;
//    }
//    //Set numeric Values
//    oi->object_sn = sn;
//    oi->object_type = type;
//
//    //Set object_id
//    oi->object_id = malloc(sizeof(char)*(strlen(id) +1));
//    if(oi->object_id == NULL){
//        free(oi);
//        return NULL;
//    }
//    strcpy(oi->object_id  , id);
//
//    //Set parent_dir_id
//    oi->parent_dir_id = malloc(sizeof(char)*(strlen(parent_id) +1));
//    if(oi->object_id == NULL){
//        free(oi->parent_dir_id);
//        free(oi);
//        return NULL;
//    }
//    strcpy(oi->parent_dir_id  , parent_id);
//
//    return oi;
//}
//
//
//static ListElement object_info_copy(ListElement object_info){
//    assert(object_info);
//    Object_Info oi = (Object_Info)(object_info);
//    Object_Info oi_copy = malloc(sizeof(*oi_copy));
//    if(oi_copy == NULL){
//        return NULL;
//    }
//    //Copy numeric Values
//    oi_copy->object_sn = oi->object_sn;
//    oi_copy->object_type = oi->object_type;
//
//    //copy object_id
//    oi_copy->object_id = malloc(sizeof(char)*(strlen(oi->object_id) +1));
//    if(oi_copy->object_id == NULL){
//        free(oi_copy);
//        return NULL;
//    }
//    strcpy(oi_copy->object_id  , oi->object_id);
//
//    //copy parent_dir_id
//    oi_copy->parent_dir_id = malloc(sizeof(char)*(strlen(oi->parent_dir_id) +1));
//    if(oi_copy->object_id == NULL){
//        free(oi_copy->parent_dir_id);
//        free(oi_copy);
//        return NULL;
//    }
//    strcpy(oi_copy->parent_dir_id  , oi->parent_dir_id);
//
//    return oi_copy;
//}
//
//static void object_info_destroy(ListElement object_info){
//    free(((Object_Info)(object_info))->object_id);
//    free(((Object_Info)(object_info))->parent_dir_id);
//    free(object_info);
//}


/* ***************** END ***************** object_info struct ***************** END ***************** */
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
