//
// Created by Polina on 24-Feb-18.
//

#ifndef DEDUPLICATIONPROJ_HEURISTIC_TEXTPARSINGUTILITIES_H
#define DEDUPLICATIONPROJ_HEURISTIC_TEXTPARSINGUTILITIES_H

#include "Utilities.h"
#include "ObjectStructures.h"
/*
 * readInputParams - Reads from the input_params.txt file the path of the input file and the type of
 *                   deduplication that it was created with
 *                   The input_params.txt file format is expected to be:
 *                              Full_Input_File_path
 *                              B/F
 *                   The Output type can be B for Block Level Deduplication or F for File level Deduplication
 *                   Make Sure The input_params File is in the same folder as the executable file
 *
 * @dedup_type - is a  pointer where the input read from file will be saved,
 *                it will be allocated inside the function and should be released from outside.
 * @input_file_path - is a  pointer where the input read from file will be saved,
 *                    it will be allocated inside the function and should be released from outside.
 */
ErrorCode readInputParams(char** input_file_path);

/*
 * countRootsInInput - Counts the number of roots in the input file.
 *                     By roots we mean the number of file systems that created the input file.
 *
 * @line - the input line that contains the file names of the file systems that created the input
 */
int countRootsInInput(char* line);

/*
 * freeStructuresArrays - frees all data objects that are in each of the input arrays
 *
 * @files_array - array of file objects
 * @physical_files_array - array of physical file objects
 * @dirs_array - array of directories objects
 * @base_objects_arr - array of block objects
 *
 * @num_file_objects - number of file objects in the files_array
 * @num_dir_objects - number of dir objects in the dirs_array
 * @num_block_objects - number of block objects in the base_objects_arr
 * @num_phys_file_objects - number of file objects in the physical_files_array
 *
 * @dedup_type - the type of deduplication that was performed on the input file
 */
//ErrorCode freeStructuresArrays(File* files_array , File* physical_files_array ,Dir* dirs_array , Block* blocks_array,
//                               unsigned long num_file_objects , unsigned long num_dir_objects ,
//                               unsigned long num_block_objects, unsigned long num_phys_file_objects ,
//                               char* dedup_type);

/*
 * readFileLine - Parses the input line into a file object to be saved
 *                 Returns a pointer to the file object
 *
 * @line - the line that represents a file object to be parsed
 * @file_type - could be either B for block_level or F for file_level
 */
File readFileLine(char* line, PMemory_pool memory_pool,
                  Base_Object* base_objects_arr);

/*
 * readBaseObjectLine - Parses the input line into a block object to be saved
 *                 Returns a pointer to the block object
 *
 * @line - the line that represents a block object to be parsed
 */
Base_Object readBaseObjectLine(char *line, File *files_array, PMemory_pool memory_pool, Base_Object* base_objects_arr);

/*
 * readRootDirLine - Parses the input line into a directory object to be saved
 *                   Returns a pointer to the directory object
 *
 * @line - the line that represents a directory object to be parsed
 * @tile_type - could be either D for regular directory or R for Root directory
 */
Dir readDirLine(char* line , PMemory_pool memory_pool);

/*
 *  aux_add_base_object_to_merge_file - ...
 *
 *  @merged_file    - ...
 *  @file_to_insert - ...
 */
void aux_add_base_object_to_merge_file(File merged_file, File file_to_insert);

/*
 *  merge_file_physical_files - ...
 *
 *  @merged_file    - ...
 *  @file_to_insert - ...
 */
//void merge_file_physical_files(File merged_file , File file_to_insert);

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
void update_dir_values(Dir current_dir , int goal_depth,
                       Dir* dirs_array, unsigned long num_dirs,
                       File* files_array,  unsigned long num_files,
                       Base_Object* base_object_array, unsigned long num_base_object,
                       File* output_files_array , unsigned long* output_files_idx,
                       Dir* output_dirs_array , unsigned long* output_dirs_idx,
                       PMemory_pool memory_pool);

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
void calculate_depth_and_merge_files(Dir* roots_array, int num_roots,
                                 Dir* dirs_array, unsigned long num_dirs,
                                 File* files_array,  unsigned long num_files,
                                 Base_Object* base_object_array, unsigned long num_base_object,
                                 int goal_depth, File* output_files_array , unsigned long* output_files_idx ,
                                 Dir* output_dirs_array , unsigned long* output_dirs_idx , PMemory_pool memory_pool);


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
void print_output_csv_header(FILE* results_file , char dedup_type , char* input_files_list, int goal_depth,
                             unsigned long num_files_input , unsigned long num_files_output,
                             unsigned long num_dirs_input , unsigned long num_dirs_output);

/*
 * clear_line - receives a line in dos format (New line is represented by \r\n
 *              and converts it to Unix format (only \n for New Line)
 *
 * @line - Pointer to the line to be cleared.
 */
void clear_line(char* line);

void print_all_files_to_csv(File* output_files_array , unsigned long output_files_idx);
#endif //DEDUPLICATIONPROJ_HEURISTIC_TEXTPARSINGUTILITIES_H
