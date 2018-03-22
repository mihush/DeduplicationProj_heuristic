//
// Created by Polina on 22-Feb-18.
//

#ifndef DEDUPLICATIONPROJ_HEURISTIC_OBJECTSTRUCTURES_H
#define DEDUPLICATIONPROJ_HEURISTIC_OBJECTSTRUCTURES_H
/* **************************************************** INCLUDES **************************************************** */
#include "List.h"
#include "HashTable.h"
#include "Utilities.h"
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
/* **************************************************** INCLUDES **************************************************** */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
/* ***************** START ****************** Structure Object Definitions ****************** START ***************** */
/*
 * Definition of a block structure:
 *            - block_sn -> a running index on all blocks read from the file system
 *            - block_id -> a hushed id as appears in the input file
 *            - block_size -> the size of a block
 *            - shared_by_num_files -> number of files sharing this block
 *            - files_array -> array of file serial number that share the same physical file
 */
struct block_t{
    unsigned long block_sn; // running index
    char* block_id; // Hashed
    unsigned int block_size;
    unsigned int shared_by_num_files;
    unsigned long* files_array;
    unsigned long* files_array_updated;
    unsigned long output_updated_idx;
};
typedef struct block_t *Block;

/*
 * Definition of a File structure:
 *            - file_sn -> a running index on all files read from the file system
 *            - file_id -> a hushed id as appears in the input file
 *            - file_depth -> the depth of the file in the hierarchical tree
 *            - dir_sn -> Serial number of the directory containing this file
 *            - num_base_objects -> number of blocks the file consists of
 *            - file_size -> the size of the file
 *            - blocks_list -> List of Block_info elements of blocks contained in the file
 */
struct file_t{
    int depth;
    char flag; // L - logical_file , P - physical_file
    unsigned long file_sn;
    char* file_id;
    unsigned long dir_sn;
    int num_base_objects;
    unsigned int file_size;
    unsigned long physical_sn;
    bool isMergedF;

    //For Block level deduplication
    Block* blocks_array;
    unsigned long ind_blocks;

    //For File level deduplication
    //Physical File - P flag
    unsigned int shared_by_num_files; // should be use only for flag = 'P'
    unsigned long* files_array; // array of file serial number that share the same physical file

    //For the Merged File
    HashTableF ht_base_objects;

};
typedef struct file_t *File;

/*
 * Definition of a Directory structure:
 *            - dir_sn         -> a running index on all directories read from the file system
 *            - dir_id         -> a hashed id as appears in the input file
 *            - parent_dir_sn  -> the sn of the parent directory in the hierarchy
 *            - dir_depth      -> the depth of the directory in the hierarchical tree
 *            - num_of_subdirs -> number of sub directories
 *            - dirs_array     -> list of directory sn contained in this directory
 *            - num_of_files   -> number of files contained in the directory
 *            - files_array    -> list of file sn contained in this directory
 *            - merged_file    -> ....
 */
struct dir_t{
    unsigned long dir_sn;
    char* dir_id;
    unsigned long parent_dir_sn;
    int depth;

    //Sub-Files
    unsigned short num_of_files;
    unsigned long* files_array; //array of serial numbers

    //Sub-Dirs
    unsigned short num_of_subdirs;
    unsigned long* dirs_array; // array of serial numbers

    //For heuristic part
    File merged_file;
};
typedef struct dir_t *Dir;
/* ****************** END ******************* Structure Object Definitions ******************* END ****************** */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
/* ******************* START ******************* Block STRUCT Functions ******************* START ******************* */
/*
 *  blockCreate - Creates a new Block with:
 *                      - a given serial number
 *                      - a hashed id
 *                      - creates an empty files list
 *                      - zeros the counter that contains the amount of files sharing this block
 *
 * @block_id   - the hashed id of the block
 * @block_sn   - serial number of the block
 * @block_size - the size of the block
 */
Block block_create(char* block_id , unsigned long block_sn , unsigned int block_size ,
                   unsigned short shared_by_num_files);

/*
 *  block_destroy - Destroys and frees space of a block structure
 *
 *  @block - pointer to the block structure to be destroyed
 */
void block_destroy(Block block);

/*
 *  block_get_SN - returns the SN of the block
 *
 *  @block - pointer to the block structure
 */
long block_get_SN(Block block);

/*
 *  block_get_ID - Returns the hashed id of the block
 *
 *  @block - pointer to the block structure
 */
char* block_get_ID(Block block);

/*
 *  block_add_file - adds the file containing the block to the files array saved in the block structure
 *
 *  @block   - pointer to the block structure to which we want to add the file
 *  @file_sn - the serial number of the file that contains the block
 */
//ErrorCode block_add_file(Block block , unsigned long file_sn);
ErrorCode add_blockptr_to_files(Block block , File* files_array , unsigned long file_sn);


/*
 *  print_block - Prints the data saved in the block structure
 *
 *  @block   - pointer to the block structure to be printed
 */
void print_block(Block block);

/*
 *  print_block_to_csv - ....
 *
 *  @block       - pointer to the block structure to be printed
 *  @output_line - ...
 */
void print_block_to_csv(Block block , char* output_line);

/* ******************** END ******************** Block STRUCT Functions ******************** END ******************** */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
/* ******************* START ******************* File STRUCT Functions ******************* START ******************** */

/*
 *  file_create - Creates a new file object with the input parameters
 *                      - file id - a hashed id as appears in the input file
 *                      - depth
 *                      -file sn - running index on all files in the filesystem
 *                      - dir sn
 *
 *
 * @file_id       - hashed id of the file
 * @file_sn       - serial number of the file object
 * @parent_dir_sn - ...
 * @num_of_blocks - ...
 * @num_of_files  - ...
 * @size          - the size of the file
 * @physical_sn   - in case of file level deduplication, there are 2 types of files - physical and logical
 * @dedup_type    -
 * @file_type     -
 */
File file_create(char* file_id , unsigned long file_sn ,unsigned long parent_dir_sn,
                 unsigned long num_of_blocks , unsigned long num_of_files,
                 unsigned int size , unsigned long physical_sn ,
                 char dedup_type , char file_type , bool isMerged);

/*
 *  file_destroy - Destroys and frees space of a file structure
 *
 *  @file - Pointer to the file object to be destroyed
 */
void file_destroy(File file);

/*
 *  file_get_SN - Returns the SN of the file
 *
 *  @file - Pointer to the file object
 */
unsigned long file_get_SN(File file);

/*
 * file_get_ID - Returns pointer to the hashed ID of the file
 *
 * @file - Pointer to the file object
 */
char* file_get_ID(File file);

/*
 *  file_get_depth - Returns the depth of the file in the hierarchy
 *
 *  @file - Pointer to the file object
 */
int file_get_depth(File file);

/*
 *  file_set_depth - update the depth of the file in the hierarchy
 *
 *  @file - Pointer to the file object
 */
void file_set_depth(File file, int depth);

/*
 *  file_get_num_base_objects - returns the number of blocks the file contains
 *
 *  @file - Pointer to the file object
 */
int file_get_num_base_objects(File file);

/*
 *  file_add_block - ....
 *
 *  @file       - Pointer to the file object
 *  @block_sn   - hashed id of the block
 *  @block_size - size of the block
 *  @idx        - ...
 */
ErrorCode file_add_block(File file , unsigned long block_sn , int block_size /*, int idx*/);

/*
 *  file_add_logical_file - ....
 *
 *  @file             - Pointer to the file object
 *  @logical_files_sn - ...
 *  @idx              - ...
 */
ErrorCode file_add_logical_file(File file , unsigned long logical_files_sn , int idx);

/*
 *  file_add_merged_block - ...
 *
 *  @file    - Pointer to the file object
 *  @bi      - ...
 *  @file_id - ...
 */
void file_add_merged_block(File file , Block block , char* file_id);

/*
 *  file_add_merged_physical - ...
 *
 *  @file            - Pointer to the file object
 *  @sn_of_physical  - ...
 *  @file_id         - ...
 */
void file_add_merged_physical(File file , unsigned long sn_of_physical , char* file_id);

/*
 *  print_file - ....
 *
 *  @file - Pointer to the file objectstructure to be printed
 */
void print_file(File file);


/*
 *  print_file_to_csv - ....
 *
 *  @file        - Pointer to the file object structure to be printed
 *  @output_line - ...
 */
void print_file_to_csv(File file, char* output_line);

/* ******************** END ******************** File STRUCT Functions ******************** END ********************* */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
/* ****************** START ****************** Directory STRUCT Functions ****************** START ****************** */

/*
 * dir_create - Creates a new Directory object with the parameters received as input
 *              Returns pointer to the new directory object created
 *
 *
 * @dir_id          - hashed id of the directory
 * @dir_sn          - serial number of the directory
 * @parent_dir_sn   - ...
 * @num_of_files    - ...
 * @num_of_sub_dirs - ...
 */
Dir dir_create(char* dir_id , unsigned long dir_sn, unsigned long parent_dir_sn ,
               unsigned long num_of_files , unsigned long num_of_sub_dirs);

/*
 * dir_destroy - Destroys struct of Directory
 *
 * @dir - Pointer to the directory structure that should be destroyed
 */
void dir_destroy(Dir dir);

/*
 * dir_get_SN - Return the serial number of directory
 *
 * @dir - pointer to the directory
 */
unsigned long dir_get_SN(Dir dir);

/*
 * dir_get_ID - Return pointer to the ID of directory
 *
 * @dir - pointer to the directory
 */
char* dir_get_ID(Dir dir);

/*
 * dir_get_depth - Return the depth of the directory
 *
 * @dir - pointer to the directory
 */
unsigned int dir_get_depth(Dir dir);

/*
 * dir_set_depth - updates the depth of the directory
 *
 * @dir - pointer to the directory
 */
void dir_set_depth(Dir dir , int depth);

/*
 * dir_add_file - Adds file to a directory object by saving its' ID in the files list of the directory
 *                and updates the files counter of the folder
 *
 *  @dir     - pointer to the directory
 *  @file_sn - the serial number of the file that should be added to the directory
 *  @idx     - ...
 */
ErrorCode dir_add_file(Dir dir , unsigned long file_sn , int idx);

/* Adding sub_dir into the directory */
/*
 * dir_add_sub_dir - Adds a sub-directory to a directory object by saving its' ID in the sub-directories list of
 *                   the directory and updates the files counter of the folder
 *
 *  @dir    - pointer to the directory
 *  @dir_sn - the serial number of the sub-directory that should be added to the directory
 *  @idx    - ...
 */
ErrorCode dir_add_sub_dir(Dir dir , unsigned long dir_sn , int idx);

/*
 *  print_dir - ....
 *
 *  @dir - Pointer to the directory object structure to be printed
 */
void print_dir(Dir dir);

/*
 *  print_dir_to_cvs - ....
 *
 *  @dir         -  Pointer to the directory object structure to be printed
 *  @output_line - ...
 */
void print_dir_to_cvs(Dir dir , char* output_line);
/* ******************* END ******************* Directory STRUCT Functions ******************* END ******************* */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
#endif //DEDUPLICATIONPROJ_HEURISTIC_OBJECTSTRUCTURES_H
