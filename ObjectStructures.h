//
// Created by Polina on 22-Feb-18.
//

#ifndef DEDUPLICATIONPROJ_HEURISTIC_OBJECTSTRUCTURES_H
#define DEDUPLICATIONPROJ_HEURISTIC_OBJECTSTRUCTURES_H
/* **************************************************** INCLUDES **************************************************** */
#include <assert.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "HashTable.h"
#include "memory_pool.h"


typedef enum{
    SUCCESS,
    INVALID_INPUT
} ErrorCode;

/* Magic Numbers */
#define FILE_ID_LEN 30
#define MAX_LINE_LEN (1024*32)
#define MERGED_FILE_ID 120

/* **************************************************** INCLUDES **************************************************** */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
/* ***************** START ****************** Structure Object Definitions ****************** START ***************** */
/*
 * Definition of a base_object_structure:
 *            base_object_structure will be either for a block (B level) or physical_file (F level)
 *            - sn -> a running index on all base objects read from the file system
 *            - id -> a hushed id as appears in the input file
 *            - size -> the size of the object
 *            - shared_by_num_files -> number of files sharing this block
 *            - files_array -> array of file serial number that share the same physical file
 */
//struct base_object_t{
struct base_object_t{
    unsigned long sn; // running index
    char* id; // Hashed
    unsigned int size;

    unsigned int shared_by_num_files;
    HashTable output_files_ht; // Hash Table for the output files contain this block

    unsigned long* files_array_updated;
    unsigned long output_updated_idx;
};
typedef struct base_object_t *Base_Object;

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
struct file_t{ // Only logical file
    unsigned long sn;
    char* id;
    unsigned long dir_sn;

    unsigned int num_base_objects;
    unsigned int size;

    //For Regular Files that go to the output as is
    Base_Object* base_objects_arr;
    unsigned long base_object_arr_idx;

    //For Merged File only (indicate if object already inserted)
    HashTable base_objects_hash_merged;
    bool* objects_bin_array;
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
    unsigned long sn;
    char* id;
    unsigned long parent_dir_sn;
    int depth;

    //Files in directory
    unsigned long num_of_files;
    unsigned long* files_array; //array of serial numbers
    unsigned long* upd_files_array; //array of serial numbers
    unsigned short upd_files_array_idx;


    //Sub-Dirs
    unsigned long num_of_subdirs;
    unsigned long* dirs_array; // array of serial numbers
    unsigned long* upd_subdirs_array; // array of serial numbers
    unsigned short upd_subdirs_array_idx;

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
Base_Object base_object_create(unsigned long base_object_sn, unsigned int base_object_size, PMemory_pool memory_pool);

Base_Object base_object_update(Base_Object base_object, char *base_object_id,
                               unsigned short shared_by_num_files, PMemory_pool memory_pool);

/*
 *  print_block_to_csv - ....
 *
 *  @block       - pointer to the block structure to be printed
 *  @output_line - ...
 */
void print_base_object_to_csv(Base_Object base_object, char* output_line, char dedup_type , FILE* csv_output_file);

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
File file_create(unsigned long sn ,char* id , unsigned long parent_dir_sn,
                 unsigned int num_base_object, unsigned int size ,
                 bool isMerged , PMemory_pool memory_pool);

/*
 *  print_file_to_csv - ....
 *
 *  @file        - Pointer to the file object structure to be printed
 *  @output_line - ...
 */
void print_file_to_csv(File file, char* output_line , FILE* csv_output_file);

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
               unsigned long num_of_files , unsigned long num_of_sub_dirs , PMemory_pool memory_pool);

/*
 * add_file_sn_to_dir - Adds file to a directory object by saving its' ID in the files list of the directory
 *                and updates the files counter of the folder
 *
 *  @dir     - pointer to the directory
 *  @file_sn - the serial number of the file that should be added to the directory
 *  @idx     - ...
 */
ErrorCode add_file_sn_to_dir(Dir dir, unsigned long file_sn, int idx);

/* Adding sub_dir into the directory */
/*
 * add_sub_dir_sn_to_dir - Adds a sub-directory to a directory object by saving its' ID in the sub-directories list of
 *                   the directory and updates the files counter of the folder
 *
 *  @dir    - pointer to the directory
 *  @dir_sn - the serial number of the sub-directory that should be added to the directory
 *  @idx    - ...
 */
ErrorCode add_sub_dir_sn_to_dir(Dir dir, unsigned long dir_sn, int idx);

/*
 *  print_dir_to_csv - ....
 *
 *  @dir         -  Pointer to the directory object structure to be printed
 *  @output_line - ...
 */
void print_dir_to_csv(Dir dir, char *output_line , FILE* csv_output_file);
/* ******************* END ******************* Directory STRUCT Functions ******************* END ******************* */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
#endif //DEDUPLICATIONPROJ_HEURISTIC_OBJECTSTRUCTURES_H
