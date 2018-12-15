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
 * determine_Merged_File_Base_Object_HT_Size - Determine The Base Object Hash Table Size
 *                                             based on the amount of the base objects in the input file.
 *
 * @num_base_objects - The amount of the base objects in the current input file based on
 *                     which the hashtable size will be determined
 */
unsigned int determine_Merged_File_Base_Object_HT_Size(unsigned long num_base_objects , char dedup_type , int goal_depth);

/*
 * readFileLine - Parses the input line into a file object to be saved
 *                 Returns a pointer to the file object
 *
 * @line - the line that represents a file object to be parsed
 * @file_type - could be either B for block_level or F for file_level
 */
File readFileLine(FILE* input_file, char* line, PMemory_pool memory_pool,
                  Base_Object* base_objects_arr , unsigned int ht_size);

/*
 * readBaseObjectLine - Parses the input line into a block object to be saved
 *                 Returns a pointer to the block object
 *
 * @line - the line that represents a block object to be parsed
 */
Base_Object readBaseObjectLine(FILE* input_file, char *line, PMemory_pool memory_pool, Base_Object* base_objects_arr);

/*
 * readRootDirLine - Parses the input line into a directory object to be saved
 *                   Returns a pointer to the directory object
 *
 * @line - the line that represents a directory object to be parsed
 * @tile_type - could be either D for regular directory or R for Root directory
 */
Dir readDirLine(FILE* input_file, char* line , PMemory_pool memory_pool);

/*
 *  add_base_object_to_merge_file - ...
 *
 *  @merged_file    - ...
 *  @file_to_insert - ...
 */
void add_base_object_to_merge_file(File merged_file, File file_to_insert, PMemory_pool memory_pool , Base_Object* base_object_array);

/*
 *  update_outputArray_and_sn - ...
 *
 *  @current_dir    - ...
 *  @files_array    - ...
 *  @output_files_array - ...
 *  @output_files_array - ...
 */
void update_outputArray_and_sn(Dir current_dir , File* files_array , File* output_files_array ,
                               unsigned long* output_files_idx);

/*
 * RECURSIVE !!!!
 * update_dir_values - ...
 *
 * @current_dir          - ...
 * @goal_depth           - ...
 * @dirs_array           - ...
 * @num_dirs             - ...
 * @files_array          - ...
 * @num_files            - ...
 * @base_objects_arr         - ...
 * @num_blocks           - ...
 * @physical_files_array - ...
 * @num_physical_files   - ...
 * @output_files_array   - ...
 * @output_files_idx     - ...
 * @output_dirs_array    - ...
 * @output_dirs_idx      - ...
 * @dedup_type           - ...
 */
void update_dir_values(FILE *files_output_result, Dir current_dir , int goal_depth, Dir* dirs_array, unsigned long num_dirs,
                       File* files_array,  unsigned long num_files, Base_Object* base_object_array, unsigned long num_base_object,
                       File* output_files_array , unsigned long* output_files_idx,
                       Dir* output_dirs_array , unsigned long* output_dirs_idx, int parent_depth,
                       unsigned int merged_file_ht_size , int* original_depth , int* max_mf_mempool_cnt,
                       PMemory_pool_mf merged_file_mem_pool ,PMemory_pool memory_pool);

/*
 *  calculate_depth_and_merge_files - ...
 *
 * @roots_array          - ...
 * @num_roots           - ...
 * @dirs_array           - ...
 * @num_dirs             - ...
 * @files_array          - ...
 * @num_files            - ...
 * @base_objects_arr         - ...
 * @num_blocks           - ...
 * @physical_files_array - ...
 * @num_physical_files   - ...
 * @output_files_array   - ...
 * @output_files_idx     - ...
 * @output_dirs_array    - ...
 * @output_dirs_idx      - ...
 * @dedup_type           - ...
 * @goal_depth           - ...
 */
void calculate_depth_and_merge_files(FILE* files_output_result, Dir* roots_array, int num_roots, Dir* dirs_array, unsigned long num_dirs,
                                     File* files_array,  unsigned long num_files,
                                     Base_Object* base_object_array, unsigned long num_base_object,
                                     int goal_depth, File* output_files_array , unsigned long* output_files_idx ,
                                     Dir* output_dirs_array , unsigned long* output_dirs_idx ,
                                     unsigned int merged_file_ht_size , int* original_depth ,int* max_mf_mempool_cnt, PMemory_pool memory_pool);
/*
 *
 *
 *
 */
void check_dir_has_child_files(Dir current_dir , Dir* dirs_array , bool* merged_file_needed);

/*
 * print_output_csv_header - printing the header titles and their corresponding values
 *
 * @results_file - the output result file
 * @dedup_type - type of deduplication
 * @input_files_list - string of all input files
 * @goal_depth - the heuristic goal depth
 * @run_time - total time program running
 * @num_files_input - number of files were in the input (means num of file objects)
 * @num_files_output - number of files in the output (means num of file objects witch left after deduplication)
 * @num_dirs_input - number of directories were in the input (means num of dir objects)
 * @num_dirs_output - number of directories in the output (means num of dir objects witch left after deduplication)
 *
 */
void print_output_csv_header(FILE *results_file, char dedup_type, char *input_files_list, int goal_depth,
                             unsigned long num_files_output,
                             unsigned long num_dirs_output,
                             unsigned long num_base_object , char* input_type);


// Function calculate a power x^y
unsigned int pow_aux(int x, int y);


void read_fragmented_line_File(FILE* input_file, char* line,int input_line_len ,PMemory_pool memory_pool,
                               Base_Object* base_objects_arr , File file_obj);
void read_fragmented_line_Dir(FILE* input_file, char* line, int input_line_len ,PMemory_pool memory_pool,
                              Dir dir_obj, unsigned long num_of_sub_dirs, unsigned long num_of_files);
void read_fragmented_line_Base_Object(FILE* input_file, char* line, int input_line_len ,PMemory_pool memory_pool,
                                      Base_Object base_obj, unsigned int shared_by_num_files);

#endif //DEDUPLICATIONPROJ_HEURISTIC_TEXTPARSINGUTILITIES_H
