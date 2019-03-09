//
// Created by Polina on 22-Feb-18.
//

#ifndef DEDUPLICATIONPROJ_HEURISTIC_OBJECTSTRUCTURES_H
#define DEDUPLICATIONPROJ_HEURISTIC_OBJECTSTRUCTURES_H
/* **************************************************** INCLUDES **************************************************** */
#include <stdbool.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "HashTable.h"
#include "memory_pool.h"

typedef enum{
    SUCCESS,
    INVALID_INPUT
} ErrorCode;

/* Magic Numbers */
#define FILE_ID_LEN 35
#define MAX_LINE_LEN (1024*32)
#define MERGED_FILE_ID 120
#define BIT_ARRAY_SIZE 200

/* **************************************************** INCLUDES **************************************************** */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
/* ***************** START ****************** Structure Object Definitions ****************** START ***************** */
/*
 * Definition of a base_object_structure:
 *            (base_object_structure will be either for a block (B level) or physical_file (F level))
 *            - sn -> serial number of the base objects
 *            - id -> a hushed id as appears in the input file
 *            - size -> the size of the base object
 *            - shared_by_num_files -> number of files sharing this base object
 *            - output_files_ht -> Hashtable of (file->id,sn) of files containing this block
 *
 *            Files containing the Base objects:
 *                  (Remember that a base object can belong to at mose that same number of files it belonged in the
 *                  original output or less if some were merged - hashtable here in order to not have
*                   duplicates when file contains the same base object multiple times)
 *            files_array_updated -> array of serial numbers of files containing this base object
 *            output_updated_idx  -> amount of objects currently in files_array_updated
 */
struct base_object_t{
    unsigned long sn; // running index
    char* id; // Hashed
    unsigned int size;

    unsigned int shared_by_num_files;
    HashTable output_files_ht; // Hash Table for the output files contain this block

    unsigned long* files_array_updated;
    unsigned long output_updated_idx;
    bool is_valid_by_k; //param which tells if the object need to be included in the output
};
typedef struct base_object_t *Base_Object;

/*
 * Definition of a File structure:
 *            - sn               -> serial number of the file object
 *            - id               -> a hushed id as appears in the input file
 *            - dir_sn           -> Serial number of the parent directory containing this file
 *            - num_base_objects -> number of blocks the file consists of
 *            - size             -> the size of the file (sum of base object sizes
 *
 *            For Regular files (that go to output as is):
 *            - base_objects_arr    -> array of pointers to base object structures that belong to the file
 *            - base_object_arr_idx -> amount of objects currently in base_objects_arr
 *
 *            For Merged files :
 *            - base_objects_hash_merged -> Hashtable of (id,sn) of base objects belonging to the file
 *            - mem_pool_mf              -> pointer to the memory pool on which the merged file object resides
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
    PMemory_pool_mf mem_pool_mf;
};
typedef struct file_t *File;

/*
 * Definition of a Directory structure:
 *            - sn         -> Serial number of the directory object (after recursion and updates)
 *            - original_sn -> Serial number of the directory object as received from the input file
 *            - id         -> a hashed id as appears in the input file
 *            - parent_dir_sn  -> the sn of the parent directory in the hierarchy
 *            - depth      -> the depth of the directory in the hierarchical tree
 *
 *            For files in the directory:
 *            - num_of_files        -> number of files contained in the directory (as received from the input file)
 *            - files_array         -> array of file sn contained in this directory (as received from the input file)
 *            - upd_files_array     -> array of updated file sns after merging process
 *            - upd_files_array_idx -> number of objects currently in upd_files_array
 *
 *            For sub directories in the directory:
 *            - num_of_subdirs        -> number of sub directories contained in the directory (as received from the input file)
 *            - dirs_array            -> array of dir sn contained in this directory (as received from the input file)
 *            - upd_subdirs_array     -> array of updated dir sns after merging process
 *            - upd_subdirs_array_idx -> number of objects currently in upd_subdirs_array
 *
 *            For merged file:
 *            - merged_file    -> Pointer to the merged file object of the directory
*                                 (all the files down in the hierarchy will be merged into this file object)
 */
struct dir_t{
    unsigned long sn;
    unsigned long original_sn;
    char* id;
    unsigned long parent_dir_sn;
    unsigned long original_parent_dir_sn;
    int depth;

    //Files in directory
    unsigned long num_of_files;
    unsigned long* files_array; //array of serial numbers
    unsigned long* upd_files_array; //array of serial numbers
    unsigned long upd_files_array_idx;


    //Sub-Dirs
    unsigned long num_of_subdirs;
    unsigned long* dirs_array; // array of serial numbers
    unsigned long* upd_subdirs_array; // array of serial numbers
    unsigned long upd_subdirs_array_idx;

    //For heuristic part
    File merged_file;
};
typedef struct dir_t *Dir;
/* ****************** END ******************* Structure Object Definitions ******************* END ****************** */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
/* ******************* START ******************* Block STRUCT Functions ******************* START ******************* */
/*
 *  base_object_create - Creates a new Base Object with:
 *                              - a given serial number
 *                              - block size
*                       Rest of the parameters will be filled in when actual base object line is read.
 *                      This object is created upon the first encounter with block sn whe reading file objects from input
 *
 * @block_sn    - serial number of the block
 * @block_size  - the size of the block
 * @memory_pool - pointer to a memory pool on which the object will be allocated
 */
Base_Object base_object_create(unsigned long base_object_sn, unsigned int base_object_size, PMemory_pool memory_pool);

/*
 *  base_object_update - Updates a Base Object that was already created with:
 *                              - id
 *                              - number of files sharing this base object in the input
*                       Rest of the parameters will be filled in when actual base object line is read.
 *                      This object is created upon the first encounter with block sn whe reading file objects from input
 *
 * @base_object         - pointer to originally created base object
 * @base_object_id      - hashed id of the base object
 * @shared_by_num_files - number of files sharing this base object
 * @memory_pool         - pointer to a memory pool on which the base object was be allocated
 */
Base_Object base_object_update(Base_Object base_object, char *base_object_id,
                               unsigned int shared_by_num_files, PMemory_pool memory_pool);

/*
 *  print_base_object_to_csv - Prints Base Object information to output csv file
 *
 *  @base_object     - pointer to the block structure to be printed
 *  @output_line     - pointer to the allocated memory for output line (Single allocation from main)
 *  @dedup_type      - Deduplication type for current processing (B/F) (which affects the format of the printing
 *  @csv_output_file - pointer to the output csv file
 */
void print_base_object_to_csv(Base_Object base_object, char* output_line, char dedup_type , FILE* csv_output_file);

/* ******************** END ******************** Block STRUCT Functions ******************** END ******************** */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
/* ******************* START ******************* File STRUCT Functions ******************* START ******************** */

/*
 *  file_create - Creates a new file object with the input parameters
 *                      - sn
 *                      - id
 *                      - parent_dir_sn
 *                      - num_base_object
 *                      - size
 *                      - isMerged
 *                      - ht_size
 *                      - memory_pool
 *                      - memory_pool_mf

 * @sn              - hashed id of the file
 * @id              - serial number of the file object
 * @parent_dir_sn   - serial number of parent directory
 * @num_base_object - number of base objects this file contained in the input
 * @size            - size of the file (sum of base_object sizes)
 * @isMerged        - flag of True for a merged file object or false otherwise
 * @ht_size         - size of hashtable to be allocated in the merged file to save the base objects
 * @memory_pool     - Pointer to a memory pool for regular objects
 * @memory_pool_mf  - Pointer to memory pool for merged file objects
 */
File file_create(unsigned long sn ,char* id , unsigned long parent_dir_sn,
                 unsigned int num_base_object, unsigned int size ,
                 bool isMerged ,unsigned int ht_size, PMemory_pool memory_pool,
                 PMemory_pool_mf memory_pool_mf);

/*
 *  print_file_to_csv - Prints File object information to output csv file
 *
 *  @file            - Pointer to the file object structure to be printed
 *  @output_line     - Pointer to the allocated memory for output line (Single allocation from main)
 *  @csv_output_file - Pointer to the output csv file
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
 * @dir_id          - hashed id of the directory
 * @dir_sn          - serial number of the directory
 * @parent_dir_sn   - serial number of parent directory
 * @num_of_files    - number of file contained in this directory
 * @num_of_sub_dirs - number of directories contained in this directory
 * @memory_pool - pointer to a memory pool on which the object will be allocated
 */
Dir dir_create(char* dir_id , unsigned long dir_sn, unsigned long parent_dir_sn ,
               unsigned long num_of_files , unsigned long num_of_sub_dirs , PMemory_pool memory_pool);

/*
 * add_file_sn_to_dir - Adds file sn to a directory object by saving its' sn in the files array of the directory
 *                      and updates the files counter of the directory
 *
 *  @dir     - pointer to the directory object
 *  @file_sn - the serial number of the file that should be added to the directory
 *  @idx     - location in the files array of the directory where the sn should be saved
 */
ErrorCode add_file_sn_to_dir(Dir dir, unsigned long file_sn, int idx);


/*
 * add_sub_dir_sn_to_dir - Adds a sub-directory to a directory object by saving its' ID in the sub-directories list of
 *                   the directory and updates the files counter of the folder
 *
 *  @dir    - pointer to the directory objects
 *  @dir_sn - the serial number of the sub-directory that should be added to the directory
 *  @idx    - location in the sub_dirs array of the directory where the sn should be saved
 */
ErrorCode add_sub_dir_sn_to_dir(Dir dir, unsigned long dir_sn, int idx);

/*
 *  print_dir_to_csv - Prints Directory object information to output csv file
 *
 *  @dir             - Pointer to the directory object structure to be printed
 *  @output_line     - Pointer to the allocated memory for output line (Single allocation from main)
 *  @csv_output_file - Pointer to the output csv file
 */
void print_dir_to_csv(Dir dir, char *output_line , FILE* csv_output_file);

/*
 * blocks_filter_rule - function which returns if a block answer a threshold number
 *                      of 0-s in the most significant bits
 * @blocks_filter_param_k   - the threshold parameter, k
 * @id                      - the block id
 * return:              ---> true - if there are at lest k 0-s bits in the most significant bits
 *                      ---> false - otherwise
 */
bool blocks_filter_rule(int blocks_filter_param_k, char* id);

/* ******************* END ******************* Directory STRUCT Functions ******************* END ******************* */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
#endif //DEDUPLICATIONPROJ_HEURISTIC_OBJECTSTRUCTURES_H
