//
// Created by Polina on 24-Feb-18.
//

#ifndef DEDUPLICATIONPROJ_HEURISTIC_TEXTPARSINGUTILITIES_H
#define DEDUPLICATIONPROJ_HEURISTIC_TEXTPARSINGUTILITIES_H

#include "ObjectStructures.h"
#include "memory_pool.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/********************************************* Definitions& Magic Numbers *********************************************/

/*
 * countRootsInInput - Counts the number of roots in the input file.
 *                     By roots we mean the number of file systems that created the input file.
 *
 * @line - the input line that contains the file names of the file systems that created the input
 */
int countRootsInInput(char* line);

/*
 * determine_Merged_File_Base_Object_HT_Size - Determines the Base Object Hash Table Size
 *                                             based on the total amount of the base objects in the input file.
 *
 * @num_base_objects - The amount of the base objects in the current input file based on
 *                     which the hashtable size will be determined
 * @dedup_type - Deduplication type of input file
 */
unsigned int determine_Merged_File_Base_Object_HT_Size(unsigned long num_base_objects , char dedup_type);

/*
 * readFileLine - 1) Parses the input line into a file object to be saved
 *                2) Returns a pointer to the created file object
 *
 * @input_file       - pointer to the input file that is being processed
 * @line             - input line that is currently being read
 * @memory_pool      - pointer to the memory pool on which the objects will be created on
 * @base_objects_arr - pointer to the Base Objects array of the entire system
 * @ht_size          - Hashtable size for the base objects that will be created as a result of the file
 */
File readFileLine(FILE* input_file, char* line, PMemory_pool memory_pool, Base_Object* base_objects_arr ,
                  unsigned int ht_size);

/*
 * readBaseObjectLine - 1) Parses the input line into a block object to be saved
 *                      2) Returns a pointer to the base object updated
 *
 * @input_file       - pointer to the input file that is being processed
 * @line             - input line that is currently being read
 * @memory_pool      - pointer to the memory pool on which the objects will be created on
 * @base_objects_arr - pointer to the Base Objects array of the entire system
 * @base_obj_filter_param_k   -  the filter rule param, which defines the base objects that will be included in the output
 */
Base_Object readBaseObjectLine(FILE* input_file, char *line, PMemory_pool memory_pool,
                                Base_Object* base_objects_arr, int base_obj_filter_param_k);

/*
 * readRootDirLine - 1) Parses the input line into a directory object to be saved
 *                   2) Returns a pointer to the directory object created
 *
 * @input_file       - pointer to the input file that is being processed
 * @line             - input line that is currently being read
 * @memory_pool      - pointer to the memory pool on which the objects will be created on
 */
Dir readDirLine(FILE* input_file, char* line , PMemory_pool memory_pool);

/*
 * add_base_object_to_merge_file - Givan a file adds all of it's base objects to the given merged file
 *                                  For each object updates the files it belongs to
 *
 * @merged_file     - Pointer to the merged file object
 * @file_to_insert  - Pointer to the file object whose base objects we want to move to the merged file
 * @memory_pool      - pointer to the memory pool on which the objects will be created on
 * @base_objects_arr - pointer to the Base Objects array of the entire system
 */
void add_base_object_to_merge_file(File merged_file, File file_to_insert, PMemory_pool memory_pool , Base_Object* base_object_array);

/*
 * move_files_to_output_array - Given an input directory moves all of the files that belong to it to the output_files_array
 *                                  - updates sns for each file (to avoid holes on the sns)
 *
 * @current_dir        - Pointer to the directory whose files we want to move to the output
 * @files_array        - array of files based on the processing of the input file
 * @output_files_array - array of files in the entire system that will go to output after processing
 * @output_files_idx   - running index for output_files_array
 * @memory_pool        - pointer to the memory pool on which the objects will be created on
 */
void move_files_to_output_array(Dir current_dir , File* files_array , File* output_files_array ,
                                unsigned long* output_files_idx, PMemory_pool memory_pool);

/*
 * RECURSIVE !!!!
 * update_dir_values - The heart of the process - goes over the directory hierarchy and
 *                                1)  moves objects above goal depth to output
 *                                2) files below goal depth are merged into merged files directories are not moved to output
 *
 * @files_output_result  - Pointer to csv output file for merged files
 * @current_dir          - pointer to current directory being processed
 * @goal_depth           - goal depth defined in the beginning of the execution
 * @dirs_array           - array of directories based on the processing of the input file
 * @num_dirs             - number of objects in dirs_array
 * @files_array          - array of files based on the processing of the input file
 * @num_files            - number of objects in files_array
 * @base_objects_arr     - pointer to the Base Objects array of the entire system
 * @num_blocks           - number of objects in base_objects_arr
 * @output_files_array   - array of files that are going to output after processing
 * @output_files_idx     - number of objects in output_files_array
 * @output_dirs_array    - array of directories that are going to output after processing
 * @output_dirs_idx      - number of objects in output_dirs_array
 * @parent_depth         - depth of the parent directory from previous processing
 * @merged_file_ht_size  - size of base objects hashtable inside a merged file (calculated once in the main)
 * @original_depth       - depth of original input (here for debugging purposes)
 * @max_mf_mempool_cnt   - number of merged file memory pools maximally allocated (here for debugging purposes)
 * @merged_file_mem_pool - pointer to memory pool for objects related to merged file
 * @memory_pool          - pointer to memory pool for objects NOT related to merged files
 */
void update_dir_values(FILE *files_output_result, Dir current_dir, int goal_depth, Dir* dirs_array, unsigned long num_dirs,
                       File* files_array,  unsigned long num_files, Base_Object* base_object_array, unsigned long num_base_object,
                       File* output_files_array , unsigned long* output_files_idx,
                       Dir* output_dirs_array , unsigned long* output_dirs_idx, int parent_depth,
                       unsigned int merged_file_ht_size , int* original_depth , int* max_mf_mempool_cnt,
                       PMemory_pool_mf merged_file_mem_pool ,PMemory_pool memory_pool);

/*
 * !!! Auxiliary Function !!!
 * calculate_depth_and_merge_files - Starts the processing of the hierarchy for each of the root directories
 *                                1)  creates and destroys eventually the merged file memory pool
 *                                      (Created once in here, and only reseted inside recursion)
 *
 * @files_output_result  - Pointer to csv output file for merged files
 * @dirs_array           - array of directories based on the processing of the input file
 * @num_dirs             - number of objects in dirs_array
 * @files_array          - array of files based on the processing of the input file
 * @num_files            - number of objects in files_array
 * @base_objects_arr     - pointer to the Base Objects array of the entire system
 * @num_blocks           - number of objects in base_objects_arr
 * @goal_depth           - goal depth defined in the beginning of the execution
 * @output_files_array   - array of files that are going to output after processing
 * @output_files_idx     - number of objects in output_files_array
 * @output_dirs_array    - array of directories that are going to output after processing
 * @output_dirs_idx      - number of objects in output_dirs_array
 * @merged_file_ht_size  - size of base objects hashtable inside a merged file (calculated once in the main)
 * @original_depth       - depth of original input (here for debugging purposes)
 * @max_mf_mempool_cnt   - number of merged file memory pools maximally allocated (here for debugging purposes)
 * @memory_pool          - pointer to memory pool for objects NOT related to merged files
 */
void calculate_depth_and_merge_files(FILE* files_output_result, Dir* roots_array, int num_roots, Dir* dirs_array, unsigned long num_dirs,
                                     File* files_array,  unsigned long num_files,
                                     Base_Object* base_object_array, unsigned long num_base_object,
                                     int goal_depth, File* output_files_array , unsigned long* output_files_idx ,
                                     Dir* output_dirs_array , unsigned long* output_dirs_idx ,
                                     unsigned int merged_file_ht_size , int* original_depth ,int* max_mf_mempool_cnt,
                                     PMemory_pool memory_pool);

/*
 * check_dir_has_child_files - checks if directory has files somewhere down the hierarchy -
 *                               ->returns true if yes, false otherwise
 *
 * @current_dir        - Pointer to directory which we want to check if has files down the hierarchy
 * @dirs_array         - array of directories based on the processing of the input file
 * @merged_file_needed - flag variable will be true if there are files in some directory down the hierarchy
 *                       otherwise will be false
 */
void check_dir_has_child_files(Dir current_dir , Dir* dirs_array , bool* merged_file_needed);

/*
 * print_output_csv_header - printing the header titles and their corresponding values
 *
 * @results_file - the output result file
 * @dedup_type - type of deduplication
 * @input_files_list - string of all input files
 * @goal_depth - the heuristic goal depth
 * @num_files_output - number of files in the output (means num of file objects witch left after deduplication)
 * @num_dirs_input - number of directories were in the input (means num of dir objects)
 * @num_dirs_output - number of directories in the output (means num of dir objects witch left after deduplication)
 * @input_type - chunks or cintainers
 */
void print_output_csv_header(FILE *results_file, char dedup_type, char *input_files_list, int goal_depth, unsigned long num_files_output,
                             unsigned long num_dirs_output,unsigned long num_base_object , char* input_type);


// Function calculate a power x^y
unsigned int pow_aux(int x, int y);

// Input file may contain extremely long lines -  over 22M chars
// The Solution is to read each line in fragments of buffer with const size
// For each line read the following conditions will be checked and treated accordingly:
//          - Buffer Contains the Entire Line.
//          - Line was cut in the middle : Either end with a comma or a sign or number or with a new line sign.
// fgets will read until the first new line, maximum bytes to read at once, or EOF, which ever is sent first
/*
 * read_fragmented_line_File - Reads in input line of file object in case it's bigger from the buffer for the
 *                             input line and can't be read at once
 *                             -> Called from readFileLine if needed
 *
 * @input_file       - pointer to the input file that is being processed
 * @line             - input line that is currently being read
 * @input_line_len   - length of the initial line read from the input
 * @memory_pool      - pointer to the memory pool on which the objects will be created on
 * @base_objects_arr - pointer to the Base Objects array of the entire system
 * @file_obj         - Pointer to the file object created from readFileLine already
 */
void read_fragmented_line_File(FILE* input_file, char* line,int input_line_len ,PMemory_pool memory_pool,
                               Base_Object* base_objects_arr , File file_obj);

/*
 * read_fragmented_line_File - Reads in input line of directory object in case it's bigger from the buffer for the
 *                             input line and can't be read at once
 *                             -> Called from readDirLine if needed
 *
 * @input_file      - pointer to the input file that is being processed
 * @line            - input line that is currently being read
 * @input_line_len  - length of the initial line read from the input
 * @memory_pool     - pointer to the memory pool on which the objects will be created on
 * @dir_obj         - Pointer to the directory object created from readDirLine already
 * @num_of_sub_dirs - number of sub directories this directory contains
 * @num_of_files    - number of files this directory contains
 */
void read_fragmented_line_Dir(FILE* input_file, char* line, int input_line_len ,PMemory_pool memory_pool,
                              Dir dir_obj, unsigned long num_of_sub_dirs, unsigned long num_of_files);

/*
 * fix_base_object_sn_after_filter_k - Change the sn's of the base_objects (Blocks) after using the filter rule (k)
 *
 * @base_objects_arr     - pointer to the Base Objects array of the entire system
 * @num_base_object      - number of objects in base_objects_arr
 */
unsigned long fix_base_object_sn_after_filter_k(Base_Object* base_object_array, unsigned long num_base_object);


#endif //DEDUPLICATIONPROJ_HEURISTIC_TEXTPARSINGUTILITIES_H
