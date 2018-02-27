#include <stdio.h>

#include "TextParsingUtilities.h"
#define  OUTPUT_TYPE_CHAR_LOC 15
#define  OUTPUT_NUM_FILE_OBJECTS_LOC 13
#define  OUTPUT_NUM_DIR_OBJECTS_LOC 19
#define  OUTPUT_NUM_BLOC_OBJECTS_LOC 14

int main() {
    /* ------------------------------------------ Define Variables ----------------------------------------- */
    char dedup_type = NULL;
    char* input_file_path;
    char line[1024];
    int num_roots = 0;
    unsigned long num_file_objects = 0 , num_dir_objects = 0 , num_block_objects = 0, num_phys_file_objects = 0;
    File files_array = NULL;
    File physical_files_array = NULL;
    Dir dirs_array = NULL;
    Block blocks_array = NULL;
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
    fgets(line , 1024 , input_file);
    if(line[OUTPUT_TYPE_CHAR_LOC] == 'b'){
        dedup_type = 'B';
    } else if (line[OUTPUT_TYPE_CHAR_LOC] == 'f'){
        dedup_type = 'F';
    }

    //Get the number of files that were read - use the input file names line
    fgets(line , 1024 , input_file);
    num_roots = countRootsInInput(line);
    printf("%d\n" , num_roots);

    //Get the number of File objects in the input file
    fgets(line , 1024 , input_file);
    num_file_objects = line[OUTPUT_NUM_FILE_OBJECTS_LOC] - '0';

    //Get the number of Directory objects in the input file
    fgets(line , 1024 , input_file);
    num_dir_objects = line[OUTPUT_NUM_DIR_OBJECTS_LOC] - '0';

    //Get the number of Blocks/Physical Files objects in the input file
    fgets(line , 1024 , input_file);
    if(dedup_type == 'B'){
        num_block_objects = line[OUTPUT_NUM_BLOC_OBJECTS_LOC] - '0';
    }else{
        num_phys_file_objects = line[OUTPUT_NUM_BLOC_OBJECTS_LOC] - '0';
    }

    //Allocate Arrays For Files, Block/Physical Files , Directories and roots
    files_array = malloc(num_file_objects * sizeof(*files_array));
    dirs_array = malloc(num_dir_objects * sizeof(*dirs_array));
    blocks_array = malloc(num_block_objects * sizeof(*blocks_array));

    if(dedup_type == 'F'){
        physical_files_array = malloc(num_phys_file_objects * sizeof(*physical_files_array));
    }
    /* -------------------------------------- Read Global Parameters -------------------------------------- */
    /* ---------------------------------------------------------------------------------------------------- */
    /* ---------------------------------------------------------------------------------------------------- */
    /* ----------------------------------------- Read Data Objects ---------------------------------------- */
    //TODO Read the objects from the file
    while(!feof(input_file)) {
        fgets(line , 1024 , input_file);
        switch(line[0]){
            case 'F':

                break;
            case 'B':

                break;
            case 'P':

                break;
            case 'R':

                break;
            case 'D':

                break;
            default:
                break;
        }
    }

    //TODO Do The Heuristic Part


    //TODO Save Output to File
    /* ----------------------------------------- Read Data Objects ---------------------------------------- */
    /* ---------------------------------------------------------------------------------------------------- */
    /* ---------------------------------------------------------------------------------------------------- */
    /* -------------------------------------- Free all allocated Data ------------------------------------- */
    free(input_file_path);
    fclose(input_file);
    err = freeStructuresArrays(&files_array , &physical_files_array , &dirs_array , &blocks_array,
                               num_file_objects , num_dir_objects , num_block_objects,
                               num_phys_file_objects ,dedup_type);
    if(err != SUCCESS){
        return 0;
    }
    //TODO free the blocks/physical files array
    free(blocks_array);
    if(dedup_type == 'F'){
        free(physical_files_array);
    }
    //TODO free the Files array
    free(files_array);
    //TODO free the Directories array
    free(dirs_array);
    /* ------------------------------------- Free all allocated Data ------------------------------------ */
    return 0;
}