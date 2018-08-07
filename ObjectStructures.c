/* **************************************************** INCLUDES **************************************************** */
#include "ObjectStructures.h"
/* **************************************************** INCLUDES **************************************************** */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
/* ******************* START ******************* Block STRUCT Functions ******************* START ******************* */

Base_Object base_object_create(unsigned long base_object_sn, unsigned int base_object_size, PMemory_pool memory_pool){
    Base_Object base_object = memory_pool_alloc(memory_pool , sizeof(*base_object));
    if(base_object == NULL){ //Check memory allocation was successful
        return NULL;
    }

    base_object->sn = base_object_sn;
    base_object->shared_by_num_files = 0;
    base_object->size = base_object_size;
    base_object->output_updated_idx = 0;
    return base_object;
}

Base_Object base_object_update(Base_Object base_object, char *base_object_id,
                               unsigned short shared_by_num_files, PMemory_pool memory_pool){
    if(base_object == NULL){ //Check memory allocation was successful
        return NULL;
    }

    base_object->id = memory_pool_alloc(memory_pool, sizeof(char)*(strlen(base_object_id) + 1)); //allocate string for base_object_id
    if(base_object->id == NULL){ //check successful allocation
        return NULL;
    }
    base_object->id = strcpy(base_object->id , base_object_id);
    base_object->shared_by_num_files = shared_by_num_files;
    base_object->files_array_updated = memory_pool_alloc(memory_pool, sizeof(unsigned long)*shared_by_num_files);
    if(base_object->files_array_updated == NULL){
        return NULL;
    }

    return base_object;
}

void print_base_object(Base_Object base_object){
    printf("# ------- Block : %lu ------- #\n" , base_object->sn);
    printf("        id : %s\n" , base_object->id);
    printf("      size : %d\n" , base_object->size);
    printf(" num_files : %d\n" , base_object->shared_by_num_files);
    for(int i = 0 ; i < base_object->shared_by_num_files ; i++){
        if( i == ((base_object->shared_by_num_files)-1)){
            printf("%lu\n", base_object->files_array_updated[i]);
        } else {
            printf("%lu - ", base_object->files_array_updated[i]);
        }
    }
    printf("# ------------------------- #\n");
}

void print_base_object_to_csv(Base_Object base_object, char* output_line, char object_type){
    char temp[100];
    sprintf(output_line , "%c,%lu,%s,%d,", object_type, base_object->sn, base_object->id , base_object->shared_by_num_files);

    //Print all file serial numbers the block belongs to
    for(int i = 0 ; i < base_object->output_updated_idx ;  i++){
        sprintf(temp , "%lu," , (base_object->files_array_updated)[i]);
        strcat(output_line , temp);
    }
    strcat(output_line , "\n");
}

/* ******************** END ******************** Block STRUCT Functions ******************** END ******************** */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
/* ******************* START ******************* File STRUCT Functions ******************* START ******************** */

File file_create(unsigned long sn ,char* id , unsigned long parent_dir_sn,
                 unsigned int num_base_object, unsigned int size ,
                 bool isMerged , PMemory_pool memory_pool){
    File file = memory_pool_alloc(memory_pool , (sizeof(*file)));
    if(file == NULL){
        return NULL;
    }
    file->depth = -1;
    file->isMergedF = isMerged;
    file->id = memory_pool_alloc(memory_pool, (sizeof(char)*(strlen(id) + 1)));
    if(file->id == NULL){
        return NULL;
    }
    file->id = strcpy(file->id, id);
    file->sn = sn;
    file->dir_sn = parent_dir_sn;
    file->size = size;
    file->num_base_objects = num_base_object;
    file->base_object_arr_idx = 0;
    file->base_objects_arr = memory_pool_alloc(memory_pool , ((sizeof(Base_Object))*num_base_object));
    if(file->base_objects_arr == NULL){
        return NULL;
    }
    if(isMerged){
        file->objects_bin_array = memory_pool_alloc(memory_pool, sizeof(bool)*num_base_object);
        if(file->objects_bin_array == NULL){
            return NULL;
        }
        for(int i = 0 ; i < num_base_object ; i++){
            file->objects_bin_array[i] = false;
        }
    }
    return file;
}

void add_base_object_to_merged_file(File file, Base_Object base_object, char *file_id){
    assert(file);
    bool object_exists = false, is_first_object = false;
    if(file->objects_bin_array[(base_object->sn)]){
        object_exists = true;
    }
    if(object_exists == false){ //Check if block exists already - do not increase counter
        // Update correspondingly file_sn at each block contain this file.
        (base_object->files_array_updated)[(base_object->output_updated_idx)] = file->sn;
        (base_object->output_updated_idx)++;

        (file->base_objects_arr)[file->base_object_arr_idx] = base_object;
        if(file->base_object_arr_idx == 0){
            is_first_object = true;
        }
        file->base_object_arr_idx = file->base_object_arr_idx + 1;
        file->objects_bin_array[base_object->sn] = true;
    }
    //Check if first physical file and changed id
    if(is_first_object){
        char new_file_id[FILE_ID_LEN];
        sprintf(new_file_id , "MF_");
        strcat(new_file_id , file_id);
        strcpy(file->id , new_file_id);
    }
    return;
}

void print_file(File file){
    assert(file);
    printf("# ------- Logical File : %lu ------- #\n", file->sn);
    printf("         id : %s\n" , file->id);
    if(file->isMergedF){
        printf(" num_blocks : %d\n" , file->base_object_arr_idx);
    }else{
        printf(" num_blocks : %d\n" , file->num_base_objects);
    }
    printf(" parent_dir : %lu\n" , file->dir_sn);
    printf(" base obj sn's:");
    int num_itr = 0;
    if(file->isMergedF){
        num_itr = file->base_object_arr_idx;
    }else{
        num_itr = file->num_base_objects;
    }
    for(int i = 0 ; i < num_itr ; i++){
        printf("%d," , (file->base_objects_arr)[i]->sn);
    }
    printf("\n");
    printf("# ------------------------- #\n");
}

void print_file_to_csv(File file, char* output_line){
    assert(file);
    char temp[100];

    sprintf(output_line , "F,%lu,%s,%lu,%d,",file->sn,file->id,file->dir_sn,file->num_base_objects);
    int num_itr;
    if(file->isMergedF){
        num_itr = file->base_object_arr_idx;
    }else {
        num_itr = file->num_base_objects;
    }
    for(int i = 0 ; i < num_itr ; i++){
        sprintf(temp , "%lu,%u" , ((file->base_objects_arr)[i])->sn , ((file->base_objects_arr)[i])->size);
        strcat(output_line , temp);
    }
    strcat(output_line , "\n");
}
/* ******************** END ******************** File STRUCT Functions ******************** END ********************* */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
/* ****************** START ****************** Directory STRUCT Functions ****************** START ****************** */

Dir dir_create(char* dir_id , unsigned long dir_sn, unsigned long parent_dir_sn ,
               unsigned long num_of_files , unsigned long num_of_sub_dirs , PMemory_pool memory_pool){
    Dir dir = memory_pool_alloc(memory_pool , (sizeof(*dir)));
    if(dir == NULL){
        return NULL;
    }
    dir->depth = 0;

    dir->id = memory_pool_alloc(memory_pool , (sizeof(char)*(strlen(dir_id) + 1)));
    if(!(dir->id)){
        return NULL;
    }
    dir->id = strcpy(dir->id , dir_id);

    dir->sn = dir_sn;
    dir->num_of_files = num_of_files;
    dir->num_of_subdirs = num_of_sub_dirs;
    dir->parent_dir_sn = parent_dir_sn;
    dir->files_array = memory_pool_alloc(memory_pool , (sizeof(unsigned long)*num_of_files));
    if(dir->files_array== NULL){
        return NULL;
    }
    dir->dirs_array = memory_pool_alloc(memory_pool , (sizeof(unsigned long)*num_of_sub_dirs));
    if(dir->dirs_array == NULL){
        return NULL;
    }
    dir->merged_file = NULL;
    return dir;
}

/* Adding file into the directory */
ErrorCode add_file_sn_to_dir(Dir dir, unsigned long file_sn, int idx){
    if(dir == NULL){
        return INVALID_INPUT;
    }
    (dir->files_array)[idx] = file_sn;

    return SUCCESS;
}

/* Adding sub_dir into the directory */
ErrorCode add_sub_dir_sn_to_dir(Dir dir, unsigned long dir_sn, int idx){
    if(dir == NULL){
        return INVALID_INPUT;
    }
    (dir->dirs_array)[idx] = dir_sn;

    return SUCCESS;
}

void print_dir(Dir dir){
    assert(dir);
    printf("# ------- : %lu ------- #\n" , dir->sn);
    printf("- num_of_files : %hu \n" , dir->num_of_files);
    for(int i = 0 ; i < dir->num_of_files ; i++){
        printf("%lu - ", (dir->files_array)[i]);
    }
    printf("\n");
    printf("- num_of_subdirs : %hu \n" , dir->num_of_subdirs);
    for(int i = 0 ; i < dir->num_of_subdirs ; i++){
        printf("%lu - ", (dir->dirs_array)[i]);
    }
    printf("\n");
    printf("# ------------------------- #\n");
}

void print_dir_to_csv(Dir dir, char *output_line){
    assert(dir);
    char temp[100];
    //Determine if root or not
    char dir_type = 'Z';
    if(dir->sn == dir->parent_dir_sn){ //It's root directory
        dir_type = 'R';
    } else { // It's a regular directory
        dir_type = 'D';
    }

    sprintf(output_line , "%c,%lu,%s,%lu,%d,%d," ,dir_type, dir->sn, dir->id, dir->parent_dir_sn,
            dir->num_of_subdirs, dir->num_of_files);
    for(int i = 0 ; i < dir->num_of_subdirs ; i++){
        sprintf(temp , "%lu," , (dir->dirs_array)[i]);
        strcat(output_line , temp);
    }
    for(int i = 0 ; i < dir->num_of_files ; i++){
        sprintf(temp , "%lu," , (dir->files_array)[i]);
        strcat(output_line , temp);
    }
    strcat(output_line , "\n");
}
/* ******************* END ******************* Directory STRUCT Functions ******************* END ******************* */

//#endif //DEDUPLICATIONPROJ_HEURISTIC_OBJECTSTRUCTURES_H
