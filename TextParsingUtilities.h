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
        block_destroy(blocks_array[i]);
    }

    if(dedup_type == 'F'){
        for (int i = 0; i < num_phys_file_objects; ++i) {
            file_destroy(physical_files_array[i]);
        }
    }

    //free files
    for(int i = 0 ; i < num_file_objects ; i++){
        file_destroy(files_array[i]);
    }

    //free directories
    for(int i = 0; i < num_dir_objects ; i++){
        dir_destroy(dirs_array[i]);
    }
    free(files_array);
    free(physical_files_array);
    free(dirs_array);
    free(blocks_array);
    return SUCCESS;
}

/*
 * readFileLine - Parses the input line into a file object to be saved
 *                 Returns a pointer to the file object
 *
 * @line - the line that represents a file object to be parsed
 * @tile_type - could be either P for physical or F for logical
 */
File readFileLine(char* line , char file_type){
    char* file_id;
    unsigned int depth = 0;
    unsigned long file_sn = 0;
    unsigned int size = 0;
    unsigned long physical_sn = 0;
    unsigned long num_of_blocks = 0;

    char* tok = strtok(line , ","); //reading the flag ("F")
    // reading the file_sn
    tok = strtok(line , ",");
    file_sn = atol(tok);

    // reading the file_id
    file_id = strtok(line , ",");

    //reading num_blocks
    tok = strtok(line , ",");
    num_of_blocks = atol(tok);

    //TODO update file creation
    File file = file_create(file_id , depth , file_sn , size , physical_sn);

    unsigned long block_sn = 0;
    unsigned int block_size = 0;
    for(int i=0 ; i< num_of_blocks ; i++){
        tok = strtok(line , ",");
        block_size = atol(tok);
        tok = strtok(line , ",");
        block_size = atoi(tok);
        //TODO add block to file
    }

    return file;
}

/*
 * readBlockLine - Parses the input line into a block object to be saved
 *                 Returns a pointer to the block object
 *
 * @line - the line that represents a block object to be parsed
 */
Block readBlockLine(char* line){
    char* block_id = NULL;
    unsigned long block_sn = 0;
    unsigned int block_size = 0;
    unsigned short num_of_files = 0;

    char* tok = strtok(line , ","); //reading the flag ("B")
    //reading block_sn
    tok = strtok(line , ",");
    block_sn = atol(tok);

    //reading Block_id
    block_id = strtok(line , ",");

    //reading num_of_files
    tok = strtok(line , ",");
    num_of_files = atoi(tok);

    Block block = block_create(block_id , block_sn , block_size);

    unsigned long file_sn = 0;
    for(int i=0 ; i<num_of_files ; i++){
        tok = strtok(line , ",");
        file_sn = atol(tok);
        //TODO add file to block
    }


    return block;
}

/*
 * readRootDirLine - Parses the input line into a directory object to be saved
 *                   Returns a pointer to the directory object
 *
 * @line - the line that represents a directory object to be parsed
 * @tile_type - could be either D for regular directory or R for Root directory
 */
Dir readRootDirLine(char* line, char dir_type){
    char* dir_id = NULL;
    unsigned int depth = 0;
    unsigned long dir_sn = 0;
    unsigned long num_of_sub_dirs = 0;
    unsigned long num_of_files = 0;

    char* tok = strtok(line , ","); //reading the flag ("D/R")
    //reading dir_sn
    tok = strtok(line , ",");
    dir_sn = atol(tok);

    //reading dir_id
    dir_id = strtok(line , ",");

    //reading num_of_sub_dirs
    tok = strtok(line , ",");
    num_of_sub_dirs = atol(tok);

    //reading num_of_files
    tok = strtok(line , ",");
    num_of_files = atol(tok);

    Dir directory = dir_create(dir_id , depth , dir_sn);

    //reading dir_sn
    unsigned long sub_dir_sn = 0;
    for(int i=0 ; i<num_of_sub_dirs ; i++){
        tok = strtok(line , ",");
        sub_dir_sn = atol(tok);
        //TODO add sub_dir to directory
    }

    //reading file_sn
    unsigned long sub_file_sn = 0;
    for(int i=0 ; i<num_of_files  ; i++){
        tok = strtok(line , ",");
        sub_file_sn = atol(tok);
        //TODO add file to directory
    }

    return directory;
}


#endif //DEDUPLICATIONPROJ_HEURISTIC_TEXTPARSINGUTILITIES_H
