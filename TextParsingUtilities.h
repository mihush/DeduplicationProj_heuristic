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

    char line[MAX_LINE_LEN];
    FILE* input_params_file = fopen("input_params.txt","r");
    if(input_params_file == NULL){ //check the file was opened successfully - if not terminate
        printf("(Parser)--> Can't open input file/s =[ \n");
        return OUT_OF_MEMORY;
    }

    //Read First Line - Input File Path
    fgets(line , MAX_LINE_LEN , input_params_file);
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
                               unsigned long num_block_objects, unsigned long num_phys_file_objects ,
                               char* dedup_type){
    if(files_array == NULL || (physical_files_array == NULL && dedup_type[0] == 'F')||
            dirs_array == NULL || (blocks_array == NULL && dedup_type[0] == 'B')){
        return INVALID_INPUT;
    }

    //free all blocks or physical files
    for (int i = 0; i < num_block_objects; ++i) {
        block_destroy(blocks_array[i]);
    }

    if(dedup_type[0] == 'F'){
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
 * @file_type - could be either B for block_level or F for file_level
 */
File readFileLine(char* line , char* dedup_type){
    File file = NULL;
    char* file_type;
    char* file_id;
    unsigned int depth = 0;
    unsigned long file_sn = 0;
    unsigned long parent_dir_sn = 0;
    unsigned int size = 0;

    unsigned long num_of_blocks = 0;
    unsigned long num_of_files = 0;

    unsigned long physical_sn = 0;

    char* tok = NULL;
    char sep[2] = ",";

    file_type = strtok(line , sep); //reading the flag ("F" / "P")

    // reading the file_sn
    tok = strtok(NULL , sep);
    file_sn = atol(tok);
    printf(" -- file_sn - %lu \n", file_sn);
    // reading the file_id
    file_id = strtok(NULL , sep);
    printf(" -- file_id - %s \n", file_id);

    //Block Level
    // F - file_sn     - file_id     - dir_sn    - num_blocks - block1_sn - block1_size - block2_sn - block2_size ....
    //File Level
    // P - physical_sn - physical_id - num_files - file1_sn - file2_sn .....
    // F - file_sn     - file_id     - dir_sn    - num_blocks - physical1_sn - physical1_size

    if(strcmp(dedup_type , "B") == 0){ //Block level deduplication
        //reading parent_sir_sn
        tok = strtok(NULL , sep);
        parent_dir_sn = atol(tok);
        printf(" -- parent_dir - %lu \n", parent_dir_sn);
        //reading num_blocks
        tok = strtok(NULL , sep);
        num_of_blocks = atol(tok);
        printf(" -- num_blocks --  %lu \n", num_of_blocks);

        file = file_create(file_id ,file_sn , parent_dir_sn,
                           num_of_blocks , 0,
                           0 , 0,
                           dedup_type[0] , file_type[0]);
        printf(" -- blocks list -- \n");
        unsigned long block_sn = 0;
        unsigned int block_size = 0;
        for(int i = 0 ; i < num_of_blocks ; i++){
            tok = strtok(NULL , sep);
            block_sn = atol(tok);
            printf("%lu " , block_sn);
            tok = strtok(NULL , sep);
            block_size = atoi(tok);
            file_add_block(file , block_sn , block_size , i);
        }
        printf("\n");

    } else { //File level deduplication
        if(strcmp(file_type , "P") == 0){ //Physical File
            //reading num_of_files
            tok = strtok(NULL , sep);
            num_of_files = atol(tok);

            file = file_create(file_id ,file_sn , parent_dir_sn,
                               0 , num_of_files,
                               0 , 0,
                               dedup_type[0] , file_type[0]);

            unsigned long logical_files_sn = 0;
            for(int i = 0 ; i < num_of_files ; i++){
                tok = strtok(NULL , sep);
                logical_files_sn = atol(tok);
                file_add_logical_file(file , logical_files_sn , i);
            }

        } else { //Logical File
            //reading parent_sir_sn
            tok = strtok(NULL , sep);
            parent_dir_sn = atol(tok);

            //reading num_blocks
            tok = strtok(NULL , sep);
            num_of_blocks = atol(tok);

            //reading physical_sn
            tok = strtok(NULL , sep);
            physical_sn = atol(tok);

            //reading physical_size
            tok = strtok(NULL , sep);
            size = atoi(tok);

            file = file_create(file_id , file_sn , parent_dir_sn ,
                               num_of_blocks , 0 ,
                               size , physical_sn,
                               dedup_type[0] , file_type[0]);

        }
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
    char* tok = NULL;
    char sep[2] = ",";

    tok = strtok(line , sep); //reading the flag ("B")
    //reading block_sn
    tok = strtok(NULL , sep);
    block_sn = atol(tok);

    //reading Block_id
    block_id = strtok(NULL , sep);

    //reading num_of_files
    tok = strtok(NULL , sep);
    num_of_files = atoi(tok);

    Block block = block_create(block_id , block_sn , block_size , num_of_files);

    unsigned long file_sn = 0;
    for(int i=0 ; i<num_of_files ; i++){
        tok = strtok(NULL , sep);
        file_sn = atol(tok);
        block_add_file(block , file_sn);
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
Dir readDirLine(char* line, char dir_type){
    char* dir_id = NULL;
    unsigned int depth = 0;
    unsigned long dir_sn = 0;
    unsigned long parent_dir_sn = 0;
    unsigned long num_of_sub_dirs = 0;
    unsigned long num_of_files = 0;
    char* tok = NULL;
    char sep[2] = ",";

    tok = strtok(line , sep); //reading the flag ("D/R")
    //reading dir_sn
    tok = strtok(NULL , sep);
    dir_sn = atol(tok);

    //reading dir_id
    dir_id = strtok(NULL , sep);

    //reading parent_dir_sn
    tok = strtok(NULL , sep);
    parent_dir_sn = atol(tok);

    //reading num_of_sub_dirs
    tok = strtok(NULL , sep);
    num_of_sub_dirs = atol(tok);

    //reading num_of_files
    tok = strtok(NULL , sep);
    num_of_files = atol(tok);

    Dir directory = dir_create(dir_id , dir_sn , parent_dir_sn ,
                                num_of_files , num_of_sub_dirs);

    //reading dir_sn
    unsigned long sub_dir_sn = 0;
    for(int i = 0 ; i < num_of_sub_dirs ; i++){
        tok = strtok(NULL , sep);
        sub_dir_sn = atol(tok);
        printf("%lu\n" , sub_dir_sn);
        //add sub_dir to directory
        dir_add_sub_dir(directory ,sub_dir_sn , i);
    }

    //reading file_sn
    unsigned long sub_file_sn = 0;
    for(int i = 0 ; i < num_of_files  ; i++){
        tok = strtok(NULL , sep);
        sub_file_sn = atol(tok);
        printf("%lu\n" , sub_file_sn);
        //add file to directory
        dir_add_file(directory , sub_file_sn , i);
    }

    return directory;
}

/*
 *  merge_file_blocks - ...
 *
 *  @merged_file    - ...
 *  @file_to_insert - ...
 */
void merge_file_blocks(File merged_file , File file_to_insert){
    for(int i = 0 ; i < file_to_insert->num_base_objects ; i++){
        file_add_merged_block(merged_file , (file_to_insert->blocks_array)[i]);
    }
}

/*
 *  merge_file_physical_files - ...
 *
 *  @merged_file    - ...
 *  @file_to_insert - ...
 */
void merge_file_physical_files(File merged_file , File file_to_insert){
    file_add_merged_physical(merged_file , file_to_insert->physical_sn);
}

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
 * @blocks_array         - ...
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
                       Block* blocks_array, unsigned long num_blocks,
                       File* physical_files_array, unsigned long num_physical_files,
                       File* output_files_array , unsigned long* output_files_idx,
                       Dir* output_dirs_array , unsigned long* output_dirs_idx,
                       char dedup_type){
    int current_depth = dir_get_depth(current_dir);

    // STOP CONDITIONS - stop if you have reached the leaves meaning a folder with no subdirs or files
    if(current_dir->num_of_subdirs == 0){ //
        if(current_dir->num_of_files == 0){
            printf("-------> No more file or directories ... \n");
            return;
        } else { //There are still some files in the directory
            printf("-------> No more directories BUT there are files to process ... \n");
            if(current_depth < (goal_depth - 1)){
                // add all of the files to output_files_array
                for(int f = 0 ; f < current_dir->num_of_files  ; f++){
                    output_files_array[*output_files_idx] = files_array[(current_dir->files_array)[f]];
                    (*output_files_idx)++;
                }
                printf("---------> Haven't reached the goal depth - just save the files \n");
//                if(current_depth == (goal_depth -1)){ //create
//                    current_dir -> merged_file = dirs_array[current_dir->dir_sn]->merged_file;
//                }
            } else { // current_depth >= (goal_depth -1)
                //add all the file blocks to the merged file - meaning current_dir->mergedFile
                if(current_depth == (goal_depth -1)){ //Create merged file
                    if(dedup_type == 'B'){
                        current_dir->merged_file = file_create("sarit_hadad" , *output_files_idx , current_dir->dir_sn ,
                                                               0 , 0 , 0 , 0 , 'B' , 'F');
                    } else {
                        current_dir->merged_file = file_create("sarit_hadad_queen" , *output_files_idx , current_dir->dir_sn ,
                                                               0 , 0 , 0 , 0 , 'F' , 'L');
                    }
                    output_files_array[*output_files_idx] = current_dir->merged_file;
                    (*output_files_idx)++;
                }
                assert(current_dir->merged_file);
                printf("---------> Reached the goal depth merge the files into directory no.%lu \n" , current_dir->dir_sn);
                for(int f = 0 ; f < current_dir->num_of_files  ; f++){
                    //merge all blocks of files_array[(current_dir->files_array)[f]] to current_dir->merged_file
                    if(dedup_type == 'B'){
                        merge_file_blocks(current_dir->merged_file , files_array[(current_dir->files_array)[f]]);
                    } else {
                        merge_file_physical_files(current_dir->merged_file , files_array[(current_dir->files_array)[f]]);
                    }
                    //Destroy the file that was merged
//                    file_destroy(files_array[(current_dir->files_array)[f]]);
//                    files_array[(current_dir->files_array)[f]] = NULL;
                }

            }
            return;
        }
    }

    printf("----> Updating values for directory : %lu \n" , current_dir->dir_sn);
    //CALCULATE RECURSION
    // Get Here ONLY if we have dirs to process !!!!!!!!!
    if(current_depth < (goal_depth - 1)){ //we HAVEN'T Reached the desired depth
        printf("------> Current dir depth %d < %d (goal depth) \n" , current_dir->depth , (goal_depth -1));

        //add all of the files to output_files_array
        for(int f = 0 ; f < current_dir->num_of_files  ; f++){
            output_files_array[*output_files_idx] = files_array[(current_dir->files_array)[f]];
            (*output_files_idx)++;
        }

        //Update all directory depths
        //Add all of the directories to the output_dirs_array
        //For each directory - call update_dir_values
        for(int d = 0 ; d < current_dir->num_of_subdirs ; d++){
            printf(" -- >Current Subdir : %lu \n" ,dirs_array[(current_dir->dirs_array)[d]]->dir_sn);

            if((current_dir->dirs_array)[d] == current_dir->dir_sn){
                continue;
            }
            //update dir depth
            dir_set_depth(dirs_array[(current_dir->dirs_array)[d]] ,(current_dir->depth + 1));

            //add dir to output_dirs_array
            output_dirs_array[*output_dirs_idx] = dirs_array[(current_dir->dirs_array)[d]];
            (*output_dirs_idx)++;

            //For each directory - call update_dir_values

            update_dir_values(dirs_array[(current_dir->dirs_array)[d]] ,goal_depth,
                              dirs_array, num_dirs, files_array, num_files,
                              blocks_array, num_blocks, physical_files_array, num_physical_files,
                              output_files_array , output_files_idx, output_dirs_array , output_dirs_idx, dedup_type);

        }
    } else {//we have Reached the desired depth
        if (current_depth == (goal_depth - 1)){
            printf("------> Current dir depth %d = %d (goal depth) \n" , current_dir->depth , (goal_depth -1));
            //create new merged file and save it to output_files_array
            if(dedup_type == 'B'){
                current_dir->merged_file = file_create("sarit_hadad" , *output_files_idx , current_dir->dir_sn ,
                                                       0 , 0 , 0 , 0 , 'B' , 'F');
            } else {
                current_dir->merged_file = file_create("sarit_hadad_queen" , *output_files_idx , current_dir->dir_sn ,
                                                       0 , 0 , 0 , 0 , 'F' , 'L');
            }
            output_files_array[*output_files_idx] = current_dir->merged_file;
            (*output_files_idx)++;


        } else { //current_depth > (goal_depth - 1)
            printf("------> Current dir depth %d > %d (goal depth) \n" , current_dir->depth , (goal_depth -1));
            //update the merged file pointer of the directory to be the one of the father
            current_dir->merged_file = (dirs_array[current_dir->dir_sn])->merged_file;
        }

        //merge all child file blocks to the merged file of the parent directory
        assert(current_dir->merged_file);
        for(int f = 0 ; f < current_dir->num_of_files  ; f++){
            //merge all blocks of files_array[(current_dir->files_array)[f]] to current_dir->merged_file
            if(dedup_type == 'B'){
                merge_file_blocks(current_dir->merged_file , files_array[(current_dir->files_array)[f]]);
            } else {
                merge_file_physical_files( current_dir->merged_file , files_array[(current_dir->files_array)[f]]);
            }
            //Destroy the file that was merged
//            file_destroy(files_array[(current_dir->files_array)[f]]);
//            files_array[(current_dir->files_array)[f]] = NULL;
        }
        //update all directory depths
        //For each directory - call update_dir_values
        for(int j = 0 ; j < current_dir->num_of_subdirs ; j++){
            //update dir depth
            dir_set_depth(dirs_array[(current_dir->dirs_array)[j]] ,(current_dir->depth + 1));
            (dirs_array[(current_dir->dirs_array)[j]])->merged_file = (dirs_array[current_dir->dir_sn])->merged_file;
            //TODO - free this directory from the array ????

            //For each directory - call update_dir_values
            if((current_dir->dirs_array)[j] == current_dir->dir_sn){
                continue;
            }
            update_dir_values(dirs_array[(current_dir->dirs_array)[j]] ,goal_depth,
                              dirs_array, num_dirs, files_array, num_files,
                              blocks_array, num_blocks, physical_files_array, num_physical_files,
                              output_files_array , output_files_idx, output_dirs_array , output_dirs_idx, dedup_type);

        }
    }
};

/*
 *  calculateDepthAndMergeFiles - ...
 *
 *
 * @roots_array          - ...
 * @num_roots           - ...
 * @dirs_array           - ...
 * @num_dirs             - ...
 * @files_array          - ...
 * @num_files            - ...
 * @blocks_array         - ...
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
void calculateDepthAndMergeFiles(Dir* roots_array, int num_roots,
                                 Dir* dirs_array, unsigned long num_dirs,
                                 File* files_array,  unsigned long num_files,
                                 Block* blocks_array, unsigned long num_blocks,
                                 File* physical_files_array, unsigned long num_physical_files,
                                 char dedup_type , int goal_depth ,
                                 File* output_files_array , unsigned long* output_files_idx ,
                                 Dir* output_dirs_array , unsigned long* output_dirs_idx ){

    printf("Let's Start Building the tree ..... \n");
    for(int r = 0 ; r < num_roots ; r++){
        //Set each roots depth to be 0
        dir_set_depth(roots_array[r] , 0);

        //add root to output_dirs_array
        output_dirs_array[*output_dirs_idx] = roots_array[r];
        (*output_dirs_idx)++;

        printf("-->Updating values for directory : %lu \n" , roots_array[r]->dir_sn);
        update_dir_values(roots_array[r] , goal_depth, dirs_array, num_dirs,
                          files_array, num_files, blocks_array, num_blocks,
                          physical_files_array, num_physical_files, output_files_array , output_files_idx,
                          output_dirs_array , output_dirs_idx, dedup_type);
    }
}


void print_output_csv_header(FILE* results_file , char dedup_type , char* input_files_list, int goal_depth,
                             double run_time , unsigned long num_files_input , unsigned long num_files_output,
                             unsigned long num_dirs_input , unsigned long num_dirs_output){
    fprintf(results_file ,"# Heuristic output\n");

    if(dedup_type == 'B'){
        fprintf(results_file ,"# Output type: block-level\n");
    } else if(dedup_type == 'F'){
        fprintf(results_file ,"# Output type: file-level\n");
    }

    fprintf(results_file , "%s" , input_files_list);
    fprintf(results_file ,"# Target Level: %d\n" , goal_depth);
    fprintf(results_file ,"# Run Time: %f\n",run_time);
    fprintf(results_file ,"# Memory Usage: ... \n"); //TODO find a way
    fprintf(results_file ,"# Num files in input: %lu\n",num_files_input);
    fprintf(results_file ,"# Num files in output: %lu\n",num_files_output);
    fprintf(results_file ,"# Num directories in input: %lu\n",num_dirs_input);
    fprintf(results_file ,"# Num directories in output: %lu\n",num_dirs_output);
}



#endif //DEDUPLICATIONPROJ_HEURISTIC_TEXTPARSINGUTILITIES_H
