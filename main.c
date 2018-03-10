#include "TextParsingUtilities.h"
#include <sys/time.h>


#define  OUTPUT_TYPE_CHAR_LOC 15


#define NUM_INPUT_FILES 1

/**********************************************************/
/*void print_ht_to_CSV(char dedup_type , char** files_to_read){
    Entry pair = NULL;
    FILE *results_file = NULL;
    char* fileName = calloc(777 , sizeof(char));
    strcpy(fileName , "Parsing_Results_");

    for(int i = 0 ; i < NUM_INPUT_FILES ; i++){
        char file_proc[5];
        strncpy(file_proc , files_to_read[i] , 4);
        file_proc[5] = '\0';
        strcat(fileName, file_proc);
        if(i < NUM_INPUT_FILES -1){
            strcat(fileName , "_");
        }
    }
    if( dedup_type == 'B'){
        strcat(fileName , "_B.csv");
    } else {
        strcat(fileName , "_F.csv");
    }
    results_file = fopen(fileName , "w+");

    fprintf(results_file ,"# Output type: heuristic\n");
    // I think she doesn't mention we need to print it
    /*
    if(dedup_type == 'B'){
        fprintf(results_file ,"# Output type: block-level\n");
    } else {
        fprintf(results_file ,"# Output type: file-level\n");
    }
     */
//    fprintf(results_file ,"# Input files: ");
//    for(int i =0 ; i < NUM_INPUT_FILES ; i++){
//        fprintf(results_file ,"%s" , files_to_read[i]);
//    }
//    fprintf(results_file ,"\n");
//    Printing the results header:
//    fprintf(results_file ,"# Target level: %d\n" , goal); //TODO - insert the givven goal
//    fprintf(results_file ,"# Run time (seconds): %f\n" , time); //TODO - calculate the time
//    fprintf(results_file ,"# Memory usage (GB): %f\n" , memory); //TODO - memory usage
//    fprintf(results_file ,"# Num files in input: %d\n" , num_of_input_files); //TODO - num of input files
//    fprintf(results_file ,"# Num files in output: %d\n" , num_of_output_files); //TODO - num of output files
//    fprintf(results_file ,"# Num directories in input: %d\n" , num_of_input_directories); //TODO - num of input directories
//    fprintf(results_file ,"# Num directories in output: %d\n" , num_of_output_directories); //TODO - num of output directories
    /*
    if(dedup_type == 'B'){
        fprintf(results_file ,"# Num Blocks: %lu\n", (blocks_sn));
    } else {
        fprintf(results_file ,"# Num physical files: %lu\n", (physical_files_sn));
    }
     */

//    if(dedup_type == 'B'){ //Block level deduplication
//        //Print Files - Logical
//        for(int i = 0 ; i < (ht_files->size_table) ;i++){
//            pair = ht_files->table[i];
//            while( pair != NULL && pair->key != NULL) {
//                File temp_file = ((File)(pair->data));
//                fprintf(results_file , "F,%lu,%s,%lu,%d,",
//                        temp_file->file_sn, temp_file->file_id , temp_file->dir_sn,
//                        temp_file->num_blocks);
//                //Object_Info temp_oi;
//                LIST_FOREACH(Block_Info , iter ,temp_file->blocks_list){
//                    unsigned long block_sn = ((Block)(ht_get(ht_blocks , iter->id)))->block_sn;
//                    fprintf(results_file ,"%lu,%d," , block_sn , iter->size);
//                }
//                //temp_oi = listGetFirst(temp_file->blocks_list);
//                fprintf(results_file ,"\n");
//                pair = pair->next;
//            }
//        }
//        //Print Blocks
//        for(int i = 0 ; i < (ht_blocks->size_table) ;i++){
//            pair = ht_blocks->table[i];
//            while( pair != NULL && pair->key != NULL) {
//                Block temp_block = ((Block)(pair->data));
//                fprintf(results_file , "B,%lu,%s,%d,",
//                        temp_block->block_sn , temp_block->block_id,
//                        temp_block->shared_by_num_files);
//                printf("%s\n", temp_block->block_id);
//                for(int j = 0 ; j < (temp_block->files_ht->size_table) ; j++){
//                    EntryF pair_file_id = temp_block->files_ht->table[j];
//                    while( pair_file_id != NULL && pair_file_id->key != NULL) {
//                        unsigned long file_sn = ((File)(ht_get(ht_files , pair_file_id->key)))->file_sn;
//                        fprintf(results_file ,"%lu," , file_sn);
//                        pair_file_id = pair_file_id->next;
//                    }
//                }
//                fprintf(results_file ,"\n");
//                pair = pair->next;
//            }
//        }
//    }else{
//        printf("File Level Dedup\n");
//
//        //Print logical files
//        for(int i = 0 ; i < (ht_files->size_table) ;i++){
//            pair = ht_files->table[i];
//            while( pair != NULL && pair->key != NULL) {
//                File temp_file = ((File)(pair->data));
//                fprintf(results_file , "F,%lu,%s,%lu,%d,%lu,%d\n",
//                        temp_file->file_sn, temp_file->file_id , temp_file->dir_sn,
//                        1, temp_file->physical_sn, temp_file->file_size);
//                pair = pair->next;
//            }
//        }
//        //Print physical files
//        for(int i = 0 ; i < (ht_physical_files->size_table) ;i++){
//            pair = ht_physical_files->table[i];
//            while( pair != NULL && pair->key != NULL) {
//                File temp_file = ((File)(pair->data));
//                fprintf(results_file , "P,%lu,%s,%d,",
//                        temp_file->physical_sn, temp_file->file_id ,
//                        temp_file->num_files);
//                for(int j = 0 ; j < (temp_file->files_ht->size_table) ; j++){
//                    EntryF pair_file_id = temp_file->files_ht->table[j];
//                    while( pair_file_id != NULL && pair_file_id->key != NULL) {
//                        unsigned long file_sn = ((File)(ht_get(ht_files , pair_file_id->key)))->file_sn;
//                        fprintf(results_file ,"%lu," , file_sn);
//                        pair_file_id = pair_file_id->next;
//                    }
//                }
//                fprintf(results_file ,"\n");
//                pair = pair->next;
//            }
//        }
//
//    }
//
//    //Print Directories
//    for(int i = 0 ; i < (ht_dirs->size_table) ;i++){
//        pair = ht_dirs->table[i];
//        while( pair != NULL && pair->key != NULL) {
//            Dir temp_dir = ((Dir)(pair->data));
//
//            if(temp_dir->dir_depth == -1){
//                fprintf(results_file , "R,");
//            }else {
//                fprintf(results_file , "D,");
//            }
//            fprintf(results_file , "%lu,%s,%lu,%d,%d," ,
//                    temp_dir->dir_sn, temp_dir->dir_id, temp_dir->parent_dir_sn,
//                    temp_dir->num_of_subdirs, temp_dir->num_of_files);
//            LIST_FOREACH(unsigned long* , iter , temp_dir->dirs_list){
//                fprintf(results_file ,"%lu," , *(iter));
//            }
//            LIST_FOREACH(unsigned long* , iter , temp_dir->files_list){
//                fprintf(results_file ,"%lu," , *(iter));
//            }
//            fprintf(results_file , "\n");
//            pair = pair->next;
//        }
//    }
//    fclose(results_file);
//    free(fileName);
//}

/*
 *
 *# Output type: heuristic
 *# Input files: <file1>, <file2>
 *# Target level: <int>
 *# Run time (seconds):
 *# Memory usage (GB):
 *# Num files in input
 *# Num files in output
 *# Num directories in input
 *# Num directories in output
 *
 */
//void print_ht_to_CSV(char* dedup_type , char** files_to_read, unsigned long num_file_objects,
//                     unsigned long num_dir_objects, unsigned long num_block_objects, unsigned long num_phys_file_objects,
//                     unsigned long file_objects_cnt, unsigned long dir_objects_cnt, unsigned long root_objects_cnt,
//                     unsigned long block_objects_cnt, unsigned long phys_file_objects_cnt, int goal_depth){
//    FILE *results_file = NULL;
//    char* fileName = calloc(777 , sizeof(char));
//    strcpy(fileName , "Parsing_Results_");
//
//    for(int i = 0 ; i < NUM_INPUT_FILES ; i++){
//        char file_proc[5];
//        strncpy(file_proc , files_to_read[i] , 4);
//        file_proc[5] = '\0';
//        strcat(fileName, file_proc);
//        if(i < NUM_INPUT_FILES -1){
//            strcat(fileName , "_");
//        }
//    }
//    if( dedup_type[0] == 'B'){
//        strcat(fileName , "_B.csv");
//    } else {
//        strcat(fileName , "_F.csv");
//    }
//    results_file = fopen(fileName , "w+");
//
//    fprintf(results_file ,"# Output type: heuristic\n");
//// I think she doesn't mention we need to print it
////    if(dedup_type == 'B'){
////        fprintf(results_file ,"# Output type: block-level\n");
////    } else {
////        fprintf(results_file ,"# Output type: file-level\n");
////    }
////
//    fprintf(results_file ,"# Input files: ");
//    for(int i =0 ; i < NUM_INPUT_FILES ; i++){
//        if(i == NUM_INPUT_FILES - 1){
//
//
//        }
//        fprintf(results_file ,"%s, " , files_to_read[i]);
//    }
//    fprintf(results_file ,"\n");
//    // Printing the results header:
//    fprintf(results_file ,"# Target level: %d\n" , goal_de); //TODO - insert the givven goal
//    fprintf(results_file ,"# Run time (seconds): %f\n" , time); //TODO - calculate the time
//    fprintf(results_file ,"# Memory usage (GB): %f\n" , memory); //TODO - memory usage
//    fprintf(results_file ,"# Num files in input: %d\n" , num_of_input_files); //TODO - num of input files
//    fprintf(results_file ,"# Num files in output: %d\n" , num_of_output_files); //TODO - num of output files
//    fprintf(results_file ,"# Num directories in input: %d\n" , num_of_input_directories); //TODO - num of input directories
//    fprintf(results_file ,"# Num directories in output: %d\n" , num_of_output_directories); //TODO - num of output directories
//
//}


int main() {
    /* ------------------------------------------ Define Variables ----------------------------------------- */
    char dedup_type[2];
    dedup_type[1] = '\0';
    char* input_file_path;
    char line[MAX_LINE_LEN];
    int num_roots = 0;
    unsigned long num_file_objects = 0 , num_dir_objects = 0 , num_block_objects = 0, num_phys_file_objects = 0;
    unsigned long file_objects_cnt = 0 , dir_objects_cnt = 0 , root_objects_cnt = 0, block_objects_cnt = 0, phys_file_objects_cnt = 0;
    File* files_array = NULL;
    File* physical_files_array = NULL;
    Dir* dirs_array = NULL;
    Dir* roots_array = NULL;
    Block* blocks_array = NULL;
    char input_files_list[MAX_LINE_LEN] ;
    char* tok = NULL;
    char sep[2] = ":";

    int goal_depth = 2;

    struct timeval tv_start,tv_end;
    double heurisitc_runtime = 0;
    /* ------------------------------------------ Define Variables ---------------------------------------- */
    /* ---------------------------------------------------------------------------------------------------- */
    /* -------------------------------------- Read Global Parameters -------------------------------------- */
    ErrorCode err = readInputParams(&input_file_path);
    if(err != SUCCESS){
        return 0;
    }

    //Open the Input File
    FILE* input_file = fopen(input_file_path , "r");
    if(input_file == NULL){
        printf("-----> Can't open input file/s =[ \n");
        return 0;
    }

    //Read the Output type
    fgets(line , MAX_LINE_LEN , input_file);
    if(line[OUTPUT_TYPE_CHAR_LOC] == 'b'){
        dedup_type[0] = 'B';
    } else if (line[OUTPUT_TYPE_CHAR_LOC] == 'f'){
        dedup_type[0] = 'F';
    }

    //Get the number of files that were read - use the input file names line
    fgets(input_files_list , MAX_LINE_LEN , input_file);
    num_roots = countRootsInInput(input_files_list);
    printf("%d\n" , num_roots);

    //Get the number of File objects in the input file
    fgets(line , MAX_LINE_LEN , input_file);
    tok = strtok(line , sep);
    tok = strtok(NULL , sep); //get the number with leading space
    num_file_objects = atol(tok);
    printf("%lu \n" , num_file_objects);

    //Get the number of Directory objects in the input file
    fgets(line , MAX_LINE_LEN , input_file);
    tok = strtok(line , sep);
    tok = strtok(NULL , sep); //get the number with leading space
    num_dir_objects = atol(tok);
    printf("%lu \n" , num_dir_objects);

    //Get the number of Blocks/Physical Files objects in the input file
    fgets(line , MAX_LINE_LEN , input_file);
    tok = strtok(line , sep);
    tok = strtok(NULL , sep);
    if(dedup_type[0] == 'B'){
        num_block_objects = atol(tok);
        printf("%lu \n" , num_block_objects);
    }else{
        num_phys_file_objects = atol(tok);
        printf("%lu \n" , num_phys_file_objects);
    }

    //Allocate Arrays For Files, Block/Physical Files , Directories and roots
    files_array = malloc(num_file_objects * sizeof(*files_array));
    dirs_array = malloc(num_dir_objects * sizeof(*dirs_array));
    roots_array = malloc(num_roots * sizeof(*roots_array));
    blocks_array = malloc(num_block_objects * sizeof(*blocks_array));
    if(files_array == NULL || dirs_array == NULL || roots_array == NULL || blocks_array == NULL){
        return 0;
    }

    if(dedup_type[0] == 'F'){
        physical_files_array = malloc(num_phys_file_objects * sizeof(*physical_files_array));
        if(physical_files_array == NULL){
            free(files_array);
            free(dirs_array);
            free(roots_array);
            free(blocks_array);
            return 0;
        }
    }

    /* -------------------------------------- Read Global Parameters -------------------------------------- */
    /* ---------------------------------------------------------------------------------------------------- */
    /* ---------------------------------------------------------------------------------------------------- */
    /* ----------------------------------------- Read Data Objects ---------------------------------------- */
    Dir res_dir = NULL;
    File res_file = NULL;
    Block res_block = NULL;
    fgets(line , MAX_LINE_LEN , input_file);
    while(!feof(input_file)) {
        printf("%s" , line);
        switch(line[0]){
            case 'F':
                res_file = readFileLine(line , dedup_type);
                printf("SN : %lu , num_file_objects : %lu\n" , res_file->file_sn , num_file_objects);
                files_array[res_file->file_sn] = res_file;
                file_objects_cnt++;
                break;
            case 'B':
                res_block = readBlockLine(line , files_array);
                blocks_array[res_block->block_sn] = res_block;
                block_objects_cnt++;
                break;
            case 'P':
                res_file = readFileLine(line , dedup_type);
                physical_files_array[res_file->file_sn] = res_file;
                phys_file_objects_cnt++;
                break;
            case 'R':
                res_dir = readDirLine(line , 'R');
                roots_array[root_objects_cnt] = res_dir;
                dirs_array[res_dir->dir_sn] = res_dir;
                root_objects_cnt++;
                dir_objects_cnt++;
                break;
            case 'D':
                res_dir = readDirLine(line , 'D');
                dirs_array[res_dir->dir_sn] = res_dir;
                dir_objects_cnt++;
                break;
            default:
                break;
        }
        fgets(line , MAX_LINE_LEN , input_file);
    }

    //TODO Remove this prints later
    printf(" #-#-# The Files array #-#-# \n");
    for( int i=0 ; i<num_file_objects ; i++){
        print_file((files_array[i]));
    }

    printf(" #-#-# The Directories array #-#-# \n");
    for( int i=0 ; i<num_dir_objects ; i++){
        print_dir((dirs_array[i]));
    }

    printf(" #-#-# The Blocks array #-#-# \n");
    for( int i=0 ; i<num_block_objects ; i++){
        print_block((blocks_array[i]));
    }

    //Build the tree hierarchy of the file systems
    gettimeofday(&tv_start, NULL);
    /* ----------------- Define Output Directories and Files arrays -----------------  */
    File* output_files_array = malloc(num_file_objects * sizeof(*output_files_array));
    if(output_files_array == NULL){
        return 0;
    }
    Dir* output_dirs_array = malloc(num_dir_objects * sizeof(*output_dirs_array));
    if(output_dirs_array == NULL){
        free(output_files_array);
        return 0;
    }

    unsigned long output_files_idx = 0 , output_dirs_idx = 0;

    /* ----------------- Define Output Directories and Files arrays -----------------  */

    calculateDepthAndMergeFiles(roots_array, num_roots,
    dirs_array, num_dir_objects, files_array,  num_file_objects,
    blocks_array, num_block_objects, physical_files_array, num_phys_file_objects,
    dedup_type[0], goal_depth, output_files_array, &output_files_idx, output_dirs_array, &output_dirs_idx);

    gettimeofday(&tv_end, NULL);
    //Calculate Total runtime of the heuristic part in seconds
    heurisitc_runtime = ((((double)(tv_end.tv_usec - tv_start.tv_usec))/1000000) +
                                                                    ((double)(tv_end.tv_sec - tv_start.tv_sec)));

    //TODO Save Output to File
    char temp_output_line[MAX_LINE_LEN];
    //Open the Output File
    FILE *results_file = NULL;
    char* output_file_name = calloc(777 , sizeof(char));
    strcpy(output_file_name , "Heuristic_Results_");
    if( dedup_type[0] == 'B'){
        strcat(output_file_name , "_B.csv");
    } else {
        strcat(output_file_name , "_F.csv");
    }
    // Open the output file
    results_file = fopen(output_file_name , "w+");
    print_output_csv_header(results_file ,dedup_type[0] , input_files_list , goal_depth , heurisitc_runtime,
                            num_file_objects , output_files_idx , num_dir_objects , output_dirs_idx);

    //Print Files to Output CSV
    printf(" #-#-# The OUTPUT Files array #-#-# \n");
    for( int i = 0 ; i < output_files_idx ; i++){
        //print_file((output_files_array[i]));
        printf("%lu\n" ,output_files_array[i]->file_sn);
        print_file_to_csv(output_files_array[i] , temp_output_line);
        fprintf(results_file , temp_output_line);
        memset(temp_output_line , 0 , sizeof(temp_output_line));
    }

    if(dedup_type[0] == 'B'){
        //Print Blocks to output CSV
        for(int i = 0 ; i < num_block_objects ; i++){
            //print_block(blocks_array[i]);
            printf("%lu\n" , blocks_array[i]->block_sn);
            print_block_to_csv(blocks_array[i] , temp_output_line);
            fprintf(results_file , temp_output_line);
            memset(temp_output_line , 0 , sizeof(temp_output_line));
        }
    } else {
        //Print Physical files to output CSV
        for(int i = 0 ; i < num_phys_file_objects ; i++){
            //print_file(physical_files_array[i]);
            print_file_to_csv(physical_files_array[i] , temp_output_line);
            fprintf(results_file , temp_output_line);
            memset(temp_output_line , 0 , sizeof(temp_output_line));
        }
    }

    //Print Directories to output CSV
    printf(" #-#-# The OUTPUT Directories array #-#-# \n");
    for( int i = 0 ; i < output_dirs_idx ; i++){
        //print_dir(output_dirs_array[i]);
        print_dir_to_cvs(output_dirs_array[i] , temp_output_line );
        fprintf(results_file , temp_output_line);
        memset(temp_output_line , 0 , sizeof(temp_output_line));
    }

    /* ----------------------------------------- Read Data Objects ---------------------------------------- */
    /* ---------------------------------------------------------------------------------------------------- */
    /* ---------------------------------------------------------------------------------------------------- */
    /* -------------------------------------- Free all allocated Data ------------------------------------- */
    free(input_file_path);
    fclose(input_file);
    fclose(results_file);

    err = freeStructuresArrays(files_array , physical_files_array , dirs_array , blocks_array,
                               num_file_objects , num_dir_objects , num_block_objects,
                               num_phys_file_objects ,dedup_type);
    if(err != SUCCESS){
        return 0;
    }
    //TODO free the blocks/physical files array
//    free(blocks_array);
//    if(dedup_type[0] == 'F'){
//        free(physical_files_array);
//    }
//    //TODO free the Files array
//    free(files_array);
//    //TODO free the Directories array
//    free(dirs_array);
    /* ------------------------------------- Free all allocated Data ------------------------------------ */
    return 0;
}
