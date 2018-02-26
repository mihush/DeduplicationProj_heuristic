//
// Created by Polina on 22-Feb-18.
//

#ifndef DEDUPLICATIONPROJ_HEURISTIC_OBJECTSTRUCTURES_H
#define DEDUPLICATIONPROJ_HEURISTIC_OBJECTSTRUCTURES_H

#include "List.h"
#include "HashTableF.h"
#include "Utilities.h"
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

/*
 * Definition of a block structure:
 *            - block_sn -> a running index on all blocks read from the file system
 *            - block_id -> a hushed id as appears in the input file
 *            - block_size -> the size of a block
 *            - shared_by_num_files -> number of files sharing this block
 *            - files_list -> list of hashed file ids containing this block
 */
struct block_t{
    unsigned long block_sn; // running index
    char* block_id; // Hashed
    unsigned int block_size;
    unsigned int shared_by_num_files;
    //HashTableF files_ht;
};
typedef struct block_t *Block;

/*
 * Definition of a File structure:
 *            - file_sn -> a running index on all files read from the file system
 *            - file_id -> a hushed id as appears in the input file
 *            - file_depth -> the depth of the file in the hierarchical tree
 *            - dir_sn -> Serial number of the directory containing this file
 *            - num_blocks -> number of blocks the file consists of
 *            - file_size -> the size of the file
 *            - blocks_list -> List of Block_info elements of blocks contained in the file
 */
struct file_t{
    char flag; // L - logical_file , P - physical_file
    unsigned long file_sn;
    char* file_id;
    unsigned int file_depth;
    unsigned long dir_sn;
    int num_blocks;
    unsigned int file_size;
    List blocks_list;
    //TODO Define HTF
    //HashTableF files_ht; // should be use only for flag = 'P'
    unsigned int num_files; // should be use only for flag = 'P'
    unsigned long physical_sn;

};
typedef struct file_t *File;

/*
 * Definition of a Directory structure:
 *            - dir_sn -> a running index on all directories read from the file system
 *            - dir_id -> a hushed id as appears in the input file
 *            - parent_dir_sn -> the sn of the parent directory in the hierarchy
 *            - dir_depth -> the depth of the directory in the hierarchical tree
 *            - num_of_subdirs -> number of sub directories
 *            - num_of_files -> number of files contained in the directory
 *            - dirs_list -> list of directory sn contained in this directory
 *            - files_list -> list of file sn contained in this directory
 */
struct dir_t{
    unsigned long dir_sn;
    char* dir_id;
    unsigned long parent_dir_sn;
    int dir_depth;
    unsigned short num_of_subdirs;
    unsigned short num_of_files;
    List dirs_list; // list of serial numbers
    List files_list; //list of serial numbers
};
typedef struct dir_t *Dir;


static ListElement copy_directory_info(ListElement directory_info){
    assert(directory_info);
    unsigned long* sn = (unsigned long*)(directory_info);
    unsigned long* sn_copy = malloc(sizeof(*sn_copy));
    if(sn_copy == NULL){
        printf("---> allocation failed at list_element copy_func\n");
        return NULL;
    }
    *sn_copy = *sn;
    return sn_copy;
}

static  void free_dir_info(ListElement dir_info){
    free(dir_info);
}

/* ****************************************** Function Declarations ******************************************** */


/*
 *  blockCreate - Creates a new Block with:
 *                      - a given serial number
 *                      - a hashed id
 *                      - creates an empty files list
 *                      - zeros the counter that contains the amount of files sharing this block
 */
Block block_create(char* block_id , unsigned long block_sn , unsigned int block_size);

/*
 *  block_destroy - Destroys and frees space of a block structure
 */
void block_destroy(Block block);

/*
 *  block_get_SN - returns the SN of the block
 */
long block_get_SN(Block block);

/*
 *  block_get_ID - Returns the hashed id of the block
 */
char* block_get_ID(Block block);

/*
 *  block_add_file - adds the file containing the block to the files list saved in the block
 */
ErrorCode block_add_file(Block block , File file);

/* *************** START ************** File STRUCT Functions *************** START **************** */
/*
 *  file_create - Creates a new file object with:
 *                      - file id - a hashed id as appears in the input file
 *                      - depth
 *                      -file sn - running index on all files in the filesystem
 *                      - dir sn
 *
 */
File file_create(char* file_id , unsigned int depth , unsigned long file_sn , unsigned int size ,
                 unsigned long physical_sn);

/*
 *  file_destroy - Destroys and frees space of a file structure
 */
void file_destroy(File file);

/*
 *  file_get_SN - returns the SN of the file
 */
unsigned long file_get_SN(File file);

/*
 * file_get_ID - returns the hashed ID of the file
 */
char* file_get_ID(File file);

/*
 *  file_get_depth - returns the depth of the file in the hierarchy
 */
unsigned int file_get_depth(File file);

/*
 *  file_get_num_blocks - returns the number of blocks the file contains
 */
int file_get_num_blocks(File file);

/*
 *
 */
ErrorCode file_set_parent_dir_sn(File file , unsigned long dir_sn);
/*
 *
 */
ErrorCode file_set_physical_sn(File file , unsigned long physical_file_sn);

/*
 *
 */
ErrorCode file_set_logical_flag(File file);


/*
 *
 */
ErrorCode file_add_block(File file , char* block_id , int block_size);


/* **************** END *************** File STRUCT Functions **************** END ***************** */
/* *************** START *************** Directory STRUCT Functions *************** START *************** */

/*
 * dir_create - Creates a new Directory object with:
 *                      - dir_id
 *                      - dir_sn
 *                      - depth
 */
Dir dir_create(char* dir_id , unsigned int depth , unsigned long dir_sn);

/*
 *
 */
ErrorCode dir_set_parent_dir_sn(Dir dir , unsigned long sn);

/*
 * dir_destroy - Destroy struct of Directory
 */
void dir_destroy(Dir dir);

/*
 * dir_get_SN - Return the sn of directory
 */
unsigned long dir_get_SN(Dir dir);

/*
 * dir_get_ID - Return the ID of directory
 */
char* dir_get_ID(Dir dir);
/*
 * dir_get_depth - Return the depth of the directory
 */
unsigned int dir_get_depth(Dir dir);

/* Adding file into the directory */
ErrorCode dir_add_file(Dir dir , unsigned long file_sn);

/* Adding sub_dir into the directory */
ErrorCode dir_add_sub_dir(Dir dir , unsigned long dir_sn);

/* **************** END **************** Directory STRUCT Functions **************** END **************** */
/* ****************************************** Function Declarations ******************************************** */

#endif //DEDUPLICATIONPROJ_HEURISTIC_OBJECTSTRUCTURES_H
