/* **************************************************** INCLUDES **************************************************** */
#include "ObjectStructures.h"
/* **************************************************** INCLUDES **************************************************** */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
/* ******************* START ******************* Block STRUCT Functions ******************* START ******************* */

Block block_create(char* block_id , unsigned long block_sn , unsigned int block_size ,
                   unsigned short shared_by_num_files){
    Block block = malloc(sizeof(*block)); //create a block
    if(block == NULL){ //Check memory allocation was successful
        return NULL;
    }

    block->block_id = calloc((BLOCK_ID_LEN + 1), sizeof(char)); //allocate string for block_id
    if(block->block_id == NULL){ //check successful allocation
        free(block);
        return NULL;
    }
    block->block_id = strcpy(block->block_id , block_id);

    block->block_sn = block_sn;
    block->shared_by_num_files = 0;
    block->block_size = block_size;

//    block->files_array = calloc(shared_by_num_files , sizeof(unsigned long));
    block->files_array_updated = calloc(shared_by_num_files , sizeof(unsigned long));
//    if(block->files_array == NULL){
//        free(block->block_id);
//        free(block);
//        return NULL;
//    }
    if(block->files_array_updated == NULL){
        free(block->files_array);
        free(block->block_id);
        free(block);
        return NULL;
    }
    block->output_updated_idx = 0;
    return block;
}

void block_destroy(Block block){
    assert(block);
    free(block->block_id);
    free(block->files_array);
    free(block);
}

long block_get_SN(Block block){
    assert(block);
    return block->block_sn;
}

char* block_get_ID(Block block){
    assert(block);
    return block->block_id;
}

//ErrorCode block_add_file(Block block , unsigned long file_sn){
//    if(block == NULL){ //Check input is valid
//        return INVALID_INPUT;
//    }
//
//    (block->files_array)[block->shared_by_num_files] = file_sn;
//    (block->shared_by_num_files)++;
//
//    return SUCCESS;
//}

ErrorCode add_blockptr_to_files(Block block , File* files_array , unsigned long file_sn){
    if(block == NULL || files_array == NULL){ //Check input is valid
        return INVALID_INPUT;
    }
//    assert(files_array[file_sn]->ind_blocks<= files_array[file_sn]->num_base_objects );
    files_array[file_sn]->blocks_array[(files_array[file_sn]->ind_blocks)] = block;
    (files_array[file_sn]->ind_blocks)++;


    return SUCCESS;
}

void print_block(Block block){
    assert(block);
    printf("# ------- Block : %lu ------- #\n" , block->block_sn);
    printf("        id : %s\n" , block->block_id);
    printf("      size : %d\n" , block->block_size);
    printf(" num_files : %d\n" , block->shared_by_num_files);
    for(int i = 0 ; i < block->shared_by_num_files ; i++){
        if( i == ((block->shared_by_num_files)-1)){
            printf("%lu\n", block->files_array[i]);
        } else {
            printf("%lu - ", block->files_array[i]);
        }
    }
    printf("# ------------------------- #\n");
}

void print_block_to_csv(Block block , char* output_line){
    assert(block);
    char temp[100];
    sprintf(output_line , "B,%lu,%s,%d,", block->block_sn , block->block_id, block->shared_by_num_files);
    //Print all file serial numbers the block belongs to
    for(int i = 0 ; i < block->output_updated_idx ;  i++){
        sprintf(temp , "%lu," , (block->files_array_updated)[i]);
        strcat(output_line , temp);
    }
    strcat(output_line , "\n");
}

/* ******************** END ******************** Block STRUCT Functions ******************** END ******************** */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
/* ******************* START ******************* File STRUCT Functions ******************* START ******************** */

File file_create(char* file_id , unsigned long file_sn ,unsigned long parent_dir_sn,
                 unsigned long num_of_blocks , unsigned long num_of_files,
                 unsigned int size , unsigned long physical_sn ,
                 char dedup_type , char file_type , bool isMerged){
    File file = malloc(sizeof(*file));
    if(file == NULL){
        return NULL;
    }
    file->depth = -1;
    file->isMergedF = isMerged;
    file->file_id = calloc((FILE_ID_LEN + 1) , sizeof(char));
    if(file->file_id == NULL){
        free(file);
        return NULL;
    }
    file->file_id = strcpy(file->file_id , file_id);
    file->file_sn = file_sn;

    if(dedup_type == 'B') { //Block level deduplication
        if(isMerged){
            file->objects_bin_array = calloc(num_of_blocks , sizeof(bool));;
            if(file->objects_bin_array == NULL){
                free(file->file_id);
                free(file);
                return NULL;
            }
        }
        file->dir_sn = parent_dir_sn;
        file->num_base_objects = num_of_blocks;
        file->flag = 'F';
        file->blocks_array = malloc(num_of_blocks*sizeof(Block));
        file->ind_blocks = 0;
        if(file->blocks_array == NULL){
            free(file->objects_bin_array);
            free(file->file_id);
            free(file);
            return NULL;
        }
    }else{
        file->files_array = calloc(num_of_files , sizeof(unsigned long));
        if(file->files_array == NULL){
            free(file->file_id);
            free(file);
            return NULL;
        }
        if(file_type == 'P') { //Physical File
            if(isMerged){
                file->objects_bin_array = calloc(num_of_files , sizeof(bool));
                if(file->objects_bin_array == NULL){
                    free(file->files_array);
                    free(file->file_id);
                    free(file);
                    return NULL;
                }
            }
            file->shared_by_num_files = num_of_files;
            file->flag = 'P';
        } else {
            file->dir_sn = parent_dir_sn;
            file->num_base_objects = num_of_blocks;
            file->physical_sn = physical_sn;
            file->file_size = size;
            file->flag = 'L';
        }
    }
    return file;
}

void file_destroy(File file){
    assert(file);
    free(file->file_id);
    if(file->flag == 'P'){
        free(file->files_array);
    }else if(file->flag == 'F'){
        free(file->blocks_array);
    }
    //TODO- free hashtable
//    if(file->isMergedF){
//        hashTableF_destroy(file->ht_base_objects,)
//    }
    free(file);
}

unsigned long file_get_SN(File file){
    assert(file);
    return file->file_sn;
}

//void file_set_SN(File file , char* new_sn){
//    assert(file);
//    if(!file){
//        return;
//    }
//    free(file->file_sn);
//
//    file->file_id = malloc(sizeof(char)* (FILE_ID_LEN + 3));
//    if(file->file_id == NULL){
//        free(file);
//        return;
//    }
//    file->file_id = strcpy(file->file_id , new_sn);
//}

char* file_get_ID(File file){
    assert(file);
    return file->file_id;
}

int file_get_depth(File file){
    assert(file);
    return file->depth;
}

void file_set_depth(File file, int depth){
    assert(file);
    file->depth = depth;
}

int file_get_num_base_objects(File file){
    assert(file);
    return file->num_base_objects;
}

ErrorCode file_add_block(File file , unsigned long block_sn , int block_size/* int idx*/){
    if(file == NULL || block_size < 0){
        printf("!!1\n");
        return INVALID_INPUT;
    }
    Block bi = malloc(sizeof(*bi));
    if(bi == NULL){
        printf("!!2\n");
        return OUT_OF_MEMORY;
    }
    bi->block_sn =  block_sn;
    bi->block_size = block_size;
    bi->output_updated_idx = 0;
    bi->files_array_updated = NULL;
    bi->files_array = NULL;
    bi->shared_by_num_files = 0;
    //(file->blocks_array)[file->ind_blocks] = bi;
    //(file->ind_blocks)++;

    return SUCCESS;
}

ErrorCode file_add_logical_file(File file , unsigned long logical_files_sn , int idx){
    if(file == NULL){
        return INVALID_INPUT;
    }

    (file->files_array)[idx] = logical_files_sn;
    (file->shared_by_num_files)++;

    return SUCCESS;
}

void file_add_merged_block(File file , Block bi , char* file_id){
    assert(file);
    bool object_exists = false;
    if(file->objects_bin_array[(bi->block_sn)]){
        object_exists = true;
    }
    if(object_exists == false){ //Check if block exists already - do not increase counter
        // Update correspondingly file_sn at each block contain this file.
        bi->files_array_updated[(bi->output_updated_idx)] = file->file_sn;
        (bi->output_updated_idx)++;
        (file->num_base_objects)++;
        file->objects_bin_array[(bi->block_sn)] = true;
    }
    //Check if first physical file and changed id
    //TODO - treat merged file differently
    if(file->num_base_objects == 1){
        char new_file_id[FILE_ID_LEN];
        sprintf(new_file_id , "MF_");
        strcat(new_file_id , file_id);
        strcpy(file->file_id , new_file_id);
    }
    return;
}

void file_add_merged_physical(File file , File physical_file , char* file_id){
    assert(file);
    bool object_exists = false;
    if(file->objects_bin_array[physical_file->file_sn]){
        object_exists = true;
    }
    if(object_exists == false){ //Check if block exists already - do not increase counter
        file->files_array[(file->num_base_objects)] = physical_file->file_sn;
        (file->num_base_objects)++;
        file->objects_bin_array[(physical_file->file_sn)] = true;
    }
    //Check if first physical file and changed id
    if(file->num_base_objects == 1){
        char new_file_id[FILE_ID_LEN];
        sprintf(new_file_id , "MF_");
        strcat(new_file_id , file_id);
        strcpy(file->file_id , new_file_id);
    }
    return;
}

void print_file(File file){
    assert(file);
    if(file->flag == 'F'){
        printf("# ------- File : %lu ------- #\n" , file->file_sn);
    } else if (file->flag == 'P'){
        printf("# ------- Physical File : %lu ------- #\n", file->file_sn);
    } else if(file->flag == 'L'){
        printf("# ------- Logical File : %lu ------- #\n", file->file_sn);
    }
    printf("         id : %s\n" , file->file_id);
    printf(" num_blocks : %d\n" , file->num_base_objects);
    printf(" parent_dir : %lu\n" , file->dir_sn);
    printf("# ------------------------- #\n");
}

void print_file_to_csv(File file, char* output_line){
    assert(file);
    char temp[100];
    char file_type = file->flag;

    if(file_type == 'F'){
        sprintf(output_line , "F,%lu,%s,%lu,%d,",file->file_sn,file->file_id,file->dir_sn,file->num_base_objects);
        //TODO differntiate between merged file and original
//        for(int i = 0 ; i < file->num_base_objects ; i++){
//            sprintf(temp , "%lu,%d" , ((file->blocks_array)[i])->block_sn , ((file->blocks_array)[i])->size);
//            strcat(output_line , temp);
//        }
    } else if(file_type == 'P'){
        sprintf(output_line , "P,%lu,%s,%d,", file->physical_sn,file->file_id ,file->shared_by_num_files);
        for(int i = 0 ; i < file->shared_by_num_files ; i++){
            sprintf(temp , "%lu," , (file->files_array)[i]);
            strcat(output_line , temp);
        }
    } else if(file_type == 'L'){
        sprintf(output_line , "F,%lu,%s,%lu,%d,%lu,%d,\n",file->file_sn, file->file_id , file->dir_sn, 1, file->physical_sn, file->file_size);
        //TODO Take care of merged files
    }
    strcat(output_line , "\n");
}
/* ******************** END ******************** File STRUCT Functions ******************** END ********************* */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
/* ****************** START ****************** Directory STRUCT Functions ****************** START ****************** */

Dir dir_create(char* dir_id , unsigned long dir_sn, unsigned long parent_dir_sn ,
               unsigned long num_of_files , unsigned long num_of_sub_dirs){
    Dir dir = malloc(sizeof(*dir));
    if(dir == NULL){
        return NULL;
    }
    dir->depth = -1;

    dir->dir_id = malloc((sizeof(char)*(strlen(dir_id) + 1)));
    if(!(dir->dir_id)){
        free(dir);
        return NULL;
    }
    dir->dir_id = strcpy(dir->dir_id , dir_id);

    dir->dir_sn = dir_sn;
    dir->num_of_files = num_of_files;
    dir->num_of_subdirs = num_of_sub_dirs;
    dir->parent_dir_sn = parent_dir_sn;
    dir->files_array = calloc(num_of_files , sizeof(unsigned long));
    if(dir->files_array== NULL){
        free(dir->dir_id);
        free(dir);
        return NULL;
    }
    dir->dirs_array = calloc(num_of_sub_dirs , sizeof(unsigned long));
    if(dir->dirs_array == NULL){
        free(dir->files_array);
        free(dir->dir_id);
        free(dir);
        return NULL;
    }
    dir->merged_file = NULL;
    return dir;
}

void dir_destroy(Dir dir){
    assert(dir);
    free(dir->dir_id);
    free(dir->dirs_array);
    free(dir->files_array);
    free(dir);
}

unsigned long dir_get_SN(Dir dir){
    assert(dir);
    return dir->dir_sn;
}

char* dir_get_ID(Dir dir){
    assert(dir);
    return dir->dir_id;
}

unsigned int dir_get_depth(Dir dir){
    assert(dir);
    return dir->depth;
}

void dir_set_depth(Dir dir , int depth){
    assert(dir);
    dir->depth = depth;
}

/* Adding file into the directory */
ErrorCode dir_add_file(Dir dir , unsigned long file_sn , int idx){
    if(dir == NULL || file_sn < 0){
        return INVALID_INPUT;
    }
    (dir->files_array)[idx] = file_sn;

    return SUCCESS;
}

/* Adding sub_dir into the directory */
ErrorCode dir_add_sub_dir(Dir dir , unsigned long dir_sn ,int idx){
    if(dir == NULL || dir_sn < 0){
        return INVALID_INPUT;
    }
    (dir->dirs_array)[idx] = dir_sn;

    return SUCCESS;
}

void print_dir(Dir dir){
    assert(dir);
    printf("# ------- : %lu \n ------- #" , dir->dir_sn);
    printf("- num_of_files : %lu \n" , dir->num_of_files);
    for(int i = 0 ; i < dir->num_of_files ; i++){
        printf("%lu - ", (dir->files_array)[i]);
    }
    printf("\n");
    printf("- num_of_subdirs : %lu \n" , dir->num_of_subdirs);
    for(int i = 0 ; i < dir->num_of_subdirs ; i++){
        printf("%lu - ", (dir->dirs_array)[i]);
    }
    printf("\n");
    printf("# ------------------------- #\n");
}

void print_dir_to_cvs(Dir dir , char* output_line){
    assert(dir);
    char temp[100];
    //Determine if root or not
    char dir_type = 'Z';
    if(dir->dir_sn == dir->parent_dir_sn){ //It's root directory
        dir_type = 'R';
    } else { // It's a regular directory
        dir_type = 'D';
    }

    sprintf(output_line , "%c,%lu,%s,%lu,%d,%d,\n" ,dir_type,
            dir->dir_sn, dir->dir_id, dir->parent_dir_sn,
            dir->num_of_subdirs, dir->num_of_files);
    //TODO - add printing hirarchy of dirs printing

}
/* ******************* END ******************* Directory STRUCT Functions ******************* END ******************* */

//#endif //DEDUPLICATIONPROJ_HEURISTIC_OBJECTSTRUCTURES_H
