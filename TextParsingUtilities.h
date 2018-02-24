//
// Created by Polina on 24-Feb-18.
//
#include "Utilities.h"
#include "ObjectStructures.h"
#ifndef DEDUPLICATIONPROJ_HEURISTIC_TEXTPARSINGUTILITIES_H
#define DEDUPLICATIONPROJ_HEURISTIC_TEXTPARSINGUTILITIES_H

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
ErrorCode readInputParams(char** input_file_path){
    if(input_file_path == NULL){ //Check for invalid Input
        return INVALID_INPUT;
    }

    char line[1024];
    FILE* input_params_file = fopen("input_params.txt","r");
    if(input_params_file == NULL){ //check the file was opened successfully - if not terminate
        printf("(Parser)--> Can't open input file/s =[ \n");
        return OUT_OF_MEMORY;
    }

    //Read First Line - Input File Path
    fgets(line , 1024 , input_params_file);
    *input_file_path = calloc(strlen(line) + 1 , sizeof(char));
    strcpy(*input_file_path , line);

    //Close The input_params_file
    fclose(input_params_file);
    return SUCCESS;
}

/*
 * countRootsInInput - Counts the number of roots in the input file.
 *                     By roots we mean the number of file systems that created the input file.
 *
 * @line - the input line that contains the file names of the file systems that created the input
 */
int countRootsInInput(char* line){
    int roots = 1 , idx = 0;
    if(line == NULL){
        return 0;
    }

    while(line[idx] !='\n'){
        if(line[idx] == ','){
            roots++;
        }
        idx++;
    }

    return roots;

}

/*
 * freeStructuresArrays - frees all data objects that are in each of the input arrays
 *
 * @files_array - array of file objects
 * @physical_files_array - array of physical file objects
 * @dirs_array - array of directories objects
 * @blocks_array - array of block objects
 *
 * @num_file_objects - number of file objects in the files_array
 * @num_dir_objects - number of dir objects in the dirs_array
 * @num_block_objects - number of block objects in the blocks_array
 * @num_phys_file_objects - number of file objects in the physical_files_array
 *
 * @dedup_type - the type of deduplication that was performed on the input file
 */
ErrorCode freeStructuresArrays(File* files_array , File* physical_files_array ,Dir* dirs_array , Block* blocks_array,
                               unsigned long num_file_objects , unsigned long num_dir_objects ,
                               unsigned long num_block_objects, unsigned long num_phys_file_objects , char dedup_type){
    if(files_array == NULL || (physical_files_array == NULL && dedup_type == 'F')||
            dirs_array == NULL || (blocks_array == NULL && dedup_type == 'B')){
        return INVALID_INPUT;
    }

    //free all blocks or physical files
    for (int i = 0; i < num_block_objects; ++i) {
        //TODO block_destroy(blocks_array[i]);
    }
    if(dedup_type == 'F'){
        for (int i = 0; i < num_phys_file_objects; ++i) {
            //TODO file_destroy(physical_files_array[i]);
        }
    }

    //free files
    for(int i = 0 ; i < num_file_objects ; i++){
        //TODO file_destroy(files_array[i]);
    }

    //free directories
    for(int i = 0; i < num_dir_objects ; i++){
        //TODO dir_destroy(dirs_array[i]);
    }
    return SUCCESS;
}
#endif //DEDUPLICATIONPROJ_HEURISTIC_TEXTPARSINGUTILITIES_H
