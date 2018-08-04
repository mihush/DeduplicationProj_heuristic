#include "TextParsingUtilities.h"
#include <sys/time.h>


#define  OUTPUT_TYPE_CHAR_LOC 15

/**********************************************************/

int main(int argc , char** argv){
    /* ------------------------------------------ Define Variables ----------------------------------------- */
    PMemory_pool mem_pool = calloc(1 , sizeof(Memory_pool));
    memory_pool_init(mem_pool);

    /* Root Directory */
    Dir* roots = NULL;

    char dedup_type[2];
    dedup_type[1] = '\0';

    char* input_file_path;
    char line[MAX_LINE_LEN];
    int num_roots = 0;
    unsigned long num_file_objects = 0 , num_dir_objects = 0 , num_base_objects = 0;
    unsigned long file_objects_cnt = 0 , dir_objects_cnt = 0 , root_objects_cnt = 0, base_objects_cnt = 0;
    File* files_array = NULL;
    Dir* dirs_array = NULL;
    Dir* roots_array = NULL;
    Base_Object* base_objects_arr = NULL;
    char input_files_list[MAX_LINE_LEN] ;
    char* tok = NULL;
    char sep[2] = ":";
    int goal_depth = 2;

    /* ------------------------------------------ Define Variables ---------------------------------------- */
    /* ---------------------------------------------------------------------------------------------------- */
    /* -------------------------------------- Read Global Parameters -------------------------------------- */
    if(argc == 1){
        printf("No Extra Command Line Argument Passed Other Than Program Name\n");
        return 0;
    }

    input_file_path = (char*)memory_pool_alloc(mem_pool , (strlen(argv[1]) + 1)*sizeof(char));
    strcpy(input_file_path, argv[1]);

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

    //Get the number of File objects in the input file
    fgets(line , MAX_LINE_LEN , input_file);
    tok = strtok(line , sep);
    tok = strtok(NULL , sep); //get the number with leading space
    num_file_objects = atol(tok);

    //Get the number of Directory objects in the input file
    fgets(line , MAX_LINE_LEN , input_file);
    tok = strtok(line , sep);
    tok = strtok(NULL , sep); //get the number with leading space
    num_dir_objects = atol(tok);

    //Get the number of Blocks/Physical Files objects in the input file
    fgets(line , MAX_LINE_LEN , input_file);
    tok = strtok(line , sep);
    tok = strtok(NULL , sep);
    num_base_objects = atol(tok);

    //Allocate Arrays For Files, Base objects , Directories and Roots
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

    /* -------------------------------------- Read Global Parameters -------------------------------------- */
    /* ---------------------------------------------------------------------------------------------------- */
    /* ---------------------------------------------------------------------------------------------------- */
    /* ----------------------------------------- Read Data Objects ---------------------------------------- */
    Dir dir = NULL;
    File file = NULL;
    Base_Object base_object = NULL;
    fgets(line , MAX_LINE_LEN , input_file);
    while(!feof(input_file)) {
        switch(line[0]){
            case 'F':
                file = readFileLine(line, mem_pool, base_objects_arr);
                files_array[file->sn] = file;
                file_objects_cnt++;
                break;
            case 'B':
            case 'P':
                base_object = readBaseObjectLine(line, files_array, mem_pool, base_objects_arr);
                base_objects_arr[base_object->sn] = base_object;
                base_objects_cnt++;
                break;
            case 'R':
                dir = readDirLine(line, mem_pool);
                roots_array[root_objects_cnt] = dir;
                dirs_array[dir->sn] = dir;
                root_objects_cnt++;
                dir_objects_cnt++;
                break;
            case 'D':
                dir = readDirLine(line, mem_pool);
                dirs_array[dir->sn] = dir;
                dir_objects_cnt++;
                break;
            default:
                break;
        }
        fgets(line , MAX_LINE_LEN , input_file);
        //TODO - check if need to use the clearLine function
    }


    printf(" #-#-# The Files array #-#-# \n");
    for( int i = 0 ; i < num_file_objects ; i++){
        print_file((files_array[i]));
    }
    printf(" #-#-# The Blocks array #-#-# \n");
    for(int i = 0 ; i < num_base_objects; i++){
        print_base_object(base_objects_arr[i]);
    }
    printf(" #-#-# The Directories array #-#-# \n");
    for( int i = 0 ; i < num_dir_objects ; i++){
        print_dir(dirs_array[i]);
    }



    //Build the tree hierarchy of the file systems
    /* ----------------- Define Output Directories and Files arrays -----------------  */
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

    /* ----------------- Define Output Directories and Files arrays -----------------  */

    calculate_depth_and_merge_files(roots_array, num_roots, dirs_array, num_dir_objects, files_array,  num_file_objects,
                                    base_objects_arr, num_base_objects, goal_depth,
                                    output_files_array, &output_files_idx, output_dirs_array, &output_dirs_idx , mem_pool);

    //TODO - correct the output file name by using the input file name (adding hurisctic or some other shit)!
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
    print_output_csv_header(results_file ,dedup_type[0] , input_files_list , goal_depth ,
                            num_file_objects , output_files_idx , num_dir_objects , output_dirs_idx);

    //Print Files to Output CSV
    printf(" #-#-# The OUTPUT Files array #-#-# \n");
    for( int i = 0 ; i < output_files_idx ; i++){
        print_file((output_files_array[i]));
        print_file_to_csv(output_files_array[i] , temp_output_line);
    }

    //Print Base_object (physhcal_file or block) output CSV
    printf(" #-#-# The OUTPUT Blocks array #-#-# \n");
    for(int i = 0 ; i < num_base_objects; i++){
        print_base_object(base_objects_arr[i]);
        print_base_object_to_csv(base_objects_arr[i] , temp_output_line, dedup_type[0]);
    }

    //queen Sarit-Hadad
    //Print Directories to output CSV
    printf(" #-#-# The OUTPUT Directories array #-#-# \n");
    for( int i = 0 ; i < output_dirs_idx ; i++){
        print_dir(output_dirs_array[i]);
        print_dir_to_csv(output_dirs_array[i], temp_output_line);
    }

    /* ----------------------------------------- Read Data Objects ---------------------------------------- */
    /* ---------------------------------------------------------------------------------------------------- */
    /* ---------------------------------------------------------------------------------------------------- */
    /* -------------------------------------- Free all allocated Data ------------------------------------- */
    fclose(input_file);
    fclose(results_file);

    //freeing all allocations
    memory_pool_destroy(mem_pool);
    free(mem_pool);

    /* ------------------------------------- Free all allocated Data ------------------------------------ */
    return 0;
}
