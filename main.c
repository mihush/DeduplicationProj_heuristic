#include "TextParsingUtilities.h"
#include <time.h>


#define  OUTPUT_TYPE_CHAR_LOC 15

//BOYS  : 3 containers /Users/mihushamsh/CLionProjects/DeduplicationProj_heuristic/NB_inputs/P_dedup_002_002_4194304_D0_P0.csv 1
//GIRLS : 3 chunks /Users/mihushamsh/CLionProjects/DeduplicationProj_heuristic/inputs/P_dedup_002_002.csv

int main(int argc , char** argv){
    /* ---------------------------------------------- Define Variables ---------------------------------------------- */
    PMemory_pool mem_pool = calloc(1 , sizeof(Memory_pool));
    memory_pool_init(mem_pool);

    char dedup_type[2];
    dedup_type[1] = '\0';
    int goal_depth = 0;

    //Debugging parameters
    int original_depth = 0;
    int max_mf_mempool_cnt = 0;


    char* input_file_path;
    char* line = (char*)memory_pool_alloc(mem_pool , (MAX_LINE_LEN*sizeof(char)));
    int num_roots = 0;
    unsigned long num_file_objects = 0 , num_dir_objects = 0 , num_base_objects = 0;
    unsigned long file_objects_cnt = 0 , dir_objects_cnt = 0 , root_objects_cnt = 0, base_objects_cnt = 0;

    File* files_array = NULL;
    Dir* dirs_array = NULL;
    Dir* roots_array = NULL;
    Base_Object* base_objects_arr = NULL;
    char input_files_list[MAX_LINE_LEN];
    char* tok = NULL;
    char* input_type;
    char sep[2] = ":";

    //We Will Determine the hash table size of the merged file blocks based on the total amount of blocks in the system
    int merged_file_ht_size = 0;
    bool is_input_file_heuristic = false;

    //Variables in order to calculate time execution
    time_t load_time_start , load_time_end , process_time_start , process_time_end;
    /* ---------------------------------------------- Define Variables ---------------------------------------------- */
    /* -------------------------------------------------------------------------------------------------------------- */
    /* ------------------------------------------- Read Global Parameters ------------------------------------------- */
    if(argc == 1){
        printf("No Extra Command Line Argument Passed Other Than Program Name\n");
        return 0;
    }
    // Getting params from command line
    goal_depth = atoi(argv[1]);

    // Run over ours files (girls) or nadav_benny files (boys)
    int input_type_len = (int)strlen(argv[2]) + 1;
    input_type = (char*)memory_pool_alloc(mem_pool , (input_type_len * sizeof(char)));
    strcpy(input_type, argv[2]);

    // Get input file path
    int input_file_path_len = (int)strlen(argv[3]) + 1;
    input_file_path = (char*)memory_pool_alloc(mem_pool , (input_file_path_len * sizeof(char)));
    strcpy(input_file_path, argv[3]);

    //Open the Input File
    time(&load_time_start);
    FILE* input_file = fopen(input_file_path , "r");
    if(input_file == NULL){
        printf("-----> Can't open input file/s =[ \n");
        return 0;
    }
    /* ------------------------------------------- Read Global Parameters ------------------------------------------- */
    /* -------------------------------------------------------------------------------------------------------------- */
    /* -------------------------------- Read Parameter Values from Input file header -------------------------------- */
    if(strcmp(input_type, "containers") == 0){ // Case of Boys input file (Nadav & Benny)
        //Get the number of files that were read - use the input file names line
        int k = 4;
        //Read three first rows uneeded from header
        while (k){
            fgets(line , MAX_LINE_LEN , input_file);
            k--;
        }
        // Getting number of roots from the command line
        num_roots = atoi(argv[4]);

        tok = strtok(line , sep);
        tok = strtok(NULL , sep); //get the number with leading space
        num_file_objects = (unsigned long)atol(tok);

        //Get the number of Directory objects in the input file
        fgets(line , MAX_LINE_LEN , input_file);
        tok = strtok(line , sep);
        tok = strtok(NULL , sep); //get the number with leading space
        num_dir_objects = (unsigned long)atol(tok);

        //Get the number of Blocks/Physical Files objects in the input file
        fgets(line , MAX_LINE_LEN , input_file);
        tok = strtok(line , sep);
        tok = strtok(NULL , sep);
        num_base_objects = (unsigned long)atol(tok);

        // Need to pass three more line from header
        k = 3;
        while (k){
            fgets(line , MAX_LINE_LEN , input_file);
            k--;
        }

    } else if(strcmp(input_type, "chunks") == 0){ // Case of OUR input file (Michal & Polina)
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

        //Get the number of File objects in the input file
        fgets(line , MAX_LINE_LEN , input_file);
        // Check if  this is a heuristic Output File:
        //                - It its heuristic it will be :      # Target depth: 5
        //                - Otherwise it will be :             # Num files: 36
        if(line[2] == 'T'){
            is_input_file_heuristic = true;
            printf("---> For your INFORMATION : You are executing the heuristic code with heuristic input .....  \n" );
            fgets(line , MAX_LINE_LEN , input_file); //read next relevant line
        }

        tok = strtok(line , sep);
        tok = strtok(NULL , sep); //get the number with leading space
        num_file_objects = (unsigned long)atol(tok);

        //Get the number of Directory objects in the input file
        fgets(line , MAX_LINE_LEN , input_file);
        tok = strtok(line , sep);
        tok = strtok(NULL , sep); //get the number with leading space
        num_dir_objects = (unsigned long)atol(tok);

        //Get the number of Blocks/Physical Files objects in the input file
        fgets(line , MAX_LINE_LEN , input_file);
        tok = strtok(line , sep);
        tok = strtok(NULL , sep);
        num_base_objects = (unsigned long)atol(tok);

    } else {
        //Should not get here !
        printf("There is no input type given =[\n");
    }

    //Allocate Object Arrays
    files_array = memory_pool_alloc(mem_pool , (num_file_objects * sizeof(*files_array)));
    dirs_array = memory_pool_alloc(mem_pool , (num_dir_objects * sizeof(*dirs_array)));
    roots_array = memory_pool_alloc(mem_pool , (num_roots * sizeof(*roots_array)));
    base_objects_arr = memory_pool_alloc(mem_pool , (num_base_objects * sizeof(*base_objects_arr)));
    if(files_array == NULL || dirs_array == NULL || roots_array == NULL || base_objects_arr == NULL){
        printf("----------> Fail to allocate arrays for objects from input file\n");
        memory_pool_destroy(mem_pool);
        free(mem_pool);
        return 0;
    }
    for(int i = 0; i < num_base_objects ; i++){
        base_objects_arr[i] = NULL;
    }

    //Determine Merged File HashTable Size
    merged_file_ht_size = determine_Merged_File_Base_Object_HT_Size(num_base_objects , dedup_type[0]);
    printf("---> Merged File Hashtable size is: %d \n" , merged_file_ht_size);
    printf("---> Number of Base Objects  is: %lu \n" , num_base_objects);
    /* -------------------------------- Read Parameter Values from Input file header -------------------------------- */
    /* -------------------------------------------------------------------------------------------------------------- */
    /* --------------------------------------------- Read Data Objects ---------------------------------------------- */
    Dir dir = NULL;
    File file = NULL;
    Base_Object base_object = NULL;
    fgets(line , MAX_LINE_LEN , input_file);
    while(!feof(input_file)) {
        switch(line[0]){
            case 'F': //This Lines can be too long for the buffer
                file = readFileLine(input_file, line, mem_pool, base_objects_arr , merged_file_ht_size);
                files_array[file->sn] = file;
                file_objects_cnt++;
                break;
            case 'C':
            case 'B':
            case 'P': //This Lines Shouldn't be extremely long
                base_object = readBaseObjectLine(input_file, line, mem_pool, base_objects_arr);
                base_objects_arr[base_object->sn] = base_object;
                base_objects_cnt++;
                break;
            case 'R':
                dir = readDirLine(input_file, line, mem_pool);
                roots_array[root_objects_cnt] = dir;
                dirs_array[dir->sn] = dir;
                root_objects_cnt++;
                dir_objects_cnt++;
                break;
            case 'D':
                dir = readDirLine(input_file, line, mem_pool);
                dirs_array[dir->sn] = dir;
                dir_objects_cnt++;
                break;
            case 'M':
                break;
            default:
                break;
        }
        fgets(line , MAX_LINE_LEN , input_file);
    }
    time(&load_time_end);
    /* --------------------------------------------- Read Data Objects ---------------------------------------------- */
    /* -------------------------------------------------------------------------------------------------------------- */
    /* --------------------------------------- Create Output File Name String --------------------------------------- */
    //The format of the File Name will be : P_heuristic_depth3_118_120.csv
//    char* input_file_name = (strrchr(input_file_path , '\\') + 1);
    char* input_file_name = (strrchr(input_file_path , '/') + 1);
    char sep_file_name[2] = "_";
    char* output_file_name = calloc(275 , sizeof(char));
    char depth_to_output[10];
    sprintf(depth_to_output, "_depth%d", goal_depth);

    strncpy(output_file_name , input_file_name , 2);
    strcat(output_file_name , "heuristic");
    strcat(output_file_name , depth_to_output);

    tok = strtok(input_file_name , sep_file_name); // Read B/P
    // Case of input from Nadav&Benny need to relate as "file-level"
    if(strcmp(tok, "P") == 0 && strcmp(input_type, "boys") == 0) {
        dedup_type[0] = 'F';
    }

    //Read the Output type0
    fgets(line , MAX_LINE_LEN , input_file);
    tok = strtok(NULL , sep_file_name); // Read heuristic/dedup
    if(is_input_file_heuristic == true){ //The input was heuristic output
        tok = strtok(NULL , sep_file_name); // Read depth
    }

    tok = strtok(NULL , sep_file_name); // Read first srv index
    strcat(output_file_name , sep_file_name);
    strcat(output_file_name , tok);
    tok = strtok(NULL , sep_file_name); // Read second srv index
    strcat(output_file_name , sep_file_name);
    strcat(output_file_name , tok);
    if(strcmp(input_type, "boys") == 0) {
        tok = strtok(NULL , sep_file_name); // Read file size
        strcat(output_file_name , sep_file_name);
        strcat(output_file_name , tok);
        tok = strtok(NULL , sep_file_name); // Read "D0"
        strcat(output_file_name , sep_file_name);
        strcat(output_file_name , tok);
        tok = strtok(NULL , sep_file_name); // Read "P0"
        strcat(output_file_name , sep_file_name);
        strcat(output_file_name , tok);
    }

    if(strcmp(input_type, "boys") == 0){
        //Determine the Output type
        if(output_file_name[0] == 'B'){
            dedup_type[0] = 'B';
        } else if (output_file_name[0] == 'P'){
            dedup_type[0] = 'F';
        }
    }

    //At this point output_file_name contains the output file that will initially contain the header only
    //Directories Output File Name
    char* output_dirs_file_name = calloc(275 , sizeof(char));
    strcpy(output_dirs_file_name , output_file_name);
    output_dirs_file_name[strlen(output_dirs_file_name) - 4] = '\0';
    strcat(output_dirs_file_name , "_DIR.csv");

    //Base Objects Output File Name
    char* output_base_objects_file_name = calloc(275 , sizeof(char));
    strcpy(output_base_objects_file_name , output_file_name);
    output_base_objects_file_name[strlen(output_base_objects_file_name) - 4] = '\0';
    strcat(output_base_objects_file_name , "_BO.csv");

    //Logical Files Output File Name
    char* output_logical_files_file_name = calloc(275 , sizeof(char));
    strcpy(output_logical_files_file_name , output_file_name);
    output_logical_files_file_name[strlen(output_logical_files_file_name) - 4] = '\0';
    strcat(output_logical_files_file_name , "_LF.csv");

    //Merged  Files Output File Name
    char* output_merged_files_file_name = calloc(275 , sizeof(char));
    strcpy(output_merged_files_file_name , output_file_name);
    output_merged_files_file_name[strlen(output_merged_files_file_name) - 4] = '\0';
    strcat(output_merged_files_file_name , "_MF.csv");

    /* --------------------------------------- Create Output File Name String --------------------------------------- */
    /* -------------------------------------------------------------------------------------------------------------- */
    /* --------------------------------- Define Output Directories and Files arrays --------------------------------- */

    File* output_files_array = memory_pool_alloc(mem_pool , (num_file_objects * sizeof(*output_files_array)));
    if(output_files_array == NULL){
        return 0;
    }
    Dir* output_dirs_array = memory_pool_alloc(mem_pool , (num_dir_objects * sizeof(*output_dirs_array)));
    if(output_dirs_array == NULL){
        free(output_files_array);
        return 0;
    }

    unsigned long output_files_idx = 0 , output_dirs_idx = 0;

    /* --------------------------------- Define Output Directories and Files arrays --------------------------------- */
    /* -------------------------------------------------------------------------------------------------------------- */
    /* ------------------------------------- Implement Heuristic on Input Data -------------------------------------- */
    // Create empty file to print temporally the output files into a csv file
    FILE* output_merged_files_file = fopen(output_merged_files_file_name , "w+");
    if(output_merged_files_file == NULL){
        printf("-----> Can't open MERGED FILES output file =[ \n");
        return 0;
    }
    free(output_merged_files_file_name);
    //Build the tree hierarchy of the file systems
    time(&process_time_start);
    calculate_depth_and_merge_files(output_merged_files_file , roots_array, num_roots, dirs_array, num_dir_objects,
                                    files_array, num_file_objects, base_objects_arr, num_base_objects, goal_depth,
                                    output_files_array, &output_files_idx, output_dirs_array, &output_dirs_idx ,
                                    merged_file_ht_size , &original_depth , &max_mf_mempool_cnt , mem_pool);
    time(&process_time_end);

    /* ------------------------------------- Implement Heuristic on Input Data -------------------------------------- */
    /* -------------------------------------------------------------------------------------------------------------- */
    /* -------------------------------------------------------------------------------------------------------------- */
    /* ---------------------------------------- Print Objects to Output File ---------------------------------------- */
    char* temp_output_line = (char*)memory_pool_alloc(mem_pool , (MAX_LINE_LEN*sizeof(char)));
    // Open the output file
    FILE* results_file = NULL; //Will Contain initially the header - later the other outputs will be merged into it by a bash script
    results_file = fopen(output_file_name , "w+");
    FILE* output_dirs_file = fopen(output_dirs_file_name , "w+");
    if(output_dirs_file == NULL){
        printf("-----> Can't open DIRECTORIES output file =[ \n");
        return 0;
    }
    FILE* output_base_objects_file = fopen(output_base_objects_file_name , "w+");
    if(output_base_objects_file == NULL){
        printf("-----> Can't open BASE OBJECTS output file =[ \n");
        return 0;
    }
    FILE* output_logical_files_file = fopen(output_logical_files_file_name , "w+");
    if(output_logical_files_file == NULL){
        printf("-----> Can't open LOGICAL FILES output file =[ \n");
        return 0;
    }

    print_output_csv_header(results_file ,dedup_type[0] , input_files_list , goal_depth , output_files_idx ,
                            output_dirs_idx , num_base_objects , input_type);
    printf(" #-#-# The OUTPUT Files array #-#-# \n");
    for( int i = 0 ; i < output_files_idx ; i++){ //Print Files to Output CSV
        if(output_files_array[i] == NULL){ //Skip over empty cells of Merged File
            continue;
        }
        print_file_to_csv(output_files_array[i] , temp_output_line , output_logical_files_file);
    }
    printf(" #-#-# The OUTPUT Blocks array #-#-# \n");
    for(int i = 0 ; i < num_base_objects; i++){ //Print Base_object (physichal_file or block) output CSV
        if(dedup_type[0] == 'B'){
            print_base_object_to_csv(base_objects_arr[i] , temp_output_line, 'B' , output_base_objects_file);
        } else{
            print_base_object_to_csv(base_objects_arr[i] , temp_output_line, 'P' , output_base_objects_file);

        }
    }
    printf(" #-#-# The OUTPUT Directories array #-#-# \n");
    for( int i = 0 ; i < output_dirs_idx ; i++){ //Print Directories to output CSV
        print_dir_to_csv(output_dirs_array[i], temp_output_line , output_dirs_file);
    }
    /* ---------------------------------------- Print Objects to Output File ---------------------------------------- */
    /* -------------------------------------------------------------------------------------------------------------- */
    double diff_load = difftime(load_time_end , load_time_start);
    double diff_process = difftime(process_time_end , process_time_start);

    FILE* debugging_params_file = fopen("debugging_params.csv" , "w+");
    fprintf(debugging_params_file ,"# InputDepth :%d\n",original_depth);
    fprintf(debugging_params_file ,"# InputLogicalFiles :%lu\n",num_file_objects);
    fprintf(debugging_params_file ,"# InputDirectories :%lu\n",num_dir_objects);
    fprintf(debugging_params_file ,"# FilesBelowTargetDepth :%lu\n",(num_file_objects - output_files_idx));
    fprintf(debugging_params_file ,"# TimeToLoad :%f\n",diff_load);
    fprintf(debugging_params_file ,"# TimeToProcess :%f\n",diff_process);
    fprintf(debugging_params_file ,"# RegularMempoolCount :%d\n",mem_pool->mempool_cnt);
    fprintf(debugging_params_file ,"# MaxMFMempoolCount :%d\n",max_mf_mempool_cnt);
    fclose(debugging_params_file);


    /* -------------------------------------------------------------------------------------------------------------- */
    /* ------------------------------------------- Free all allocated Data ------------------------------------------ */
    fclose(input_file);
    fclose(results_file);
    fclose(output_dirs_file);
    fclose(output_base_objects_file);
    fclose(output_logical_files_file);
    fclose(output_merged_files_file);

    //free output file names strings
    free(output_file_name);
    free(output_dirs_file_name);
    free(output_base_objects_file_name);
    free(output_logical_files_file_name);

    //freeing all allocations
    memory_pool_destroy(mem_pool);
    free(mem_pool);
    /* ------------------------------------------- Free all allocated Data ------------------------------------------ */
    return 0;
}
