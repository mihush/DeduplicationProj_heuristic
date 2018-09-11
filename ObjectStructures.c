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
    base_object->size = base_object_size;

    //This Params Will be Filled Later by base_object_update
    base_object->output_files_ht = NULL;
    base_object->shared_by_num_files = 0;
    base_object->output_updated_idx = 0;
    return base_object;
}

Base_Object base_object_update(Base_Object base_object, char *base_object_id,
                               unsigned short shared_by_num_files, PMemory_pool memory_pool){
    if(base_object == NULL){ //Check memory allocation was successful
        return NULL;
    }

    base_object->shared_by_num_files = shared_by_num_files;
    base_object->files_array_updated = memory_pool_alloc(memory_pool, (sizeof(unsigned long)*(shared_by_num_files + 1)));

    base_object->id = memory_pool_alloc(memory_pool, sizeof(char)*(strlen(base_object_id)+1)); //allocate string for base_object_id
    if(base_object->id == NULL ){ //check successful allocation
        return NULL;
    }
    base_object->id = strcpy(base_object->id , base_object_id);

    base_object->output_files_ht = ht_create(shared_by_num_files, memory_pool);
    if(base_object->output_files_ht == NULL){ //check successful allocation
        return NULL;
    }
    return base_object;
}

void print_base_object_to_csv(Base_Object base_object, char* output_line, char object_type , FILE* csv_output_file){
    char temp[100];
    sprintf(output_line , "%c,%lu,%s,%lu,", object_type, base_object->sn, base_object->id , base_object->output_updated_idx);

    //Print all file serial numbers the block belongs to
    for(int i = 0 ; i < base_object->output_updated_idx ;  i++){
        sprintf(temp , "%lu," , (base_object->files_array_updated)[i]);
        strcat(output_line , temp);
    }
    strcat(output_line , "\n");
    fprintf(csv_output_file , "%s" ,output_line);
}

/* ******************** END ******************** Block STRUCT Functions ******************** END ******************** */
/* ****************************************************************************************************************** */
/* ****************************************************************************************************************** */
/* ******************* START ******************* File STRUCT Functions ******************* START ******************** */

File file_create(unsigned long sn ,char* id , unsigned long parent_dir_sn,
                 unsigned int num_base_object, unsigned int size , bool isMerged , PMemory_pool memory_pool){
    File file = memory_pool_alloc(memory_pool , (sizeof(*file)));
    if(file == NULL){
        return NULL;
    }

    int id_length = 0;
    if(isMerged){
        id_length = MERGED_FILE_ID;
    } else {
        id_length = (int)(strlen(id) + 1);
    }
    file->id = memory_pool_alloc(memory_pool, (sizeof(char)*id_length));
    if(file->id == NULL){
        return NULL;
    }
    file->id = strcpy(file->id, id);
    file->sn = sn;
    file->dir_sn = parent_dir_sn;
    file->size = size;
    file->num_base_objects = num_base_object;
    file->base_object_arr_idx = 0;


    if(isMerged){ //Allocate Hash Table For Merged Files
        unsigned long ht_size = num_base_object/3;
        file->base_objects_hash_merged = ht_create(ht_size , memory_pool);
        if(file->base_objects_hash_merged == NULL){
            return NULL;
        }
    } else { //Allocate Arrays For Regular Files
        file->base_objects_arr = memory_pool_alloc(memory_pool , ((sizeof(Base_Object))*num_base_object));
        if(file->base_objects_arr == NULL){
            return NULL;
        }
    }

//    if(isMerged){
//        file->objects_bin_array = memory_pool_alloc(memory_pool, sizeof(bool)*num_base_object);
//        if(file->objects_bin_array == NULL){
//            return NULL;
//        }
//        for(int i = 0 ; i < num_base_object ; i++){
//            file->objects_bin_array[i] = false;
//        }
//    }
    return file;
}

void print_file_to_csv(File file, char* output_line , FILE* csv_output_file){
    assert(file);
    char temp[100];
    int output_line_len = 0;
    Entry pair = NULL;

    sprintf(output_line , "F,%lu,%s,%lu,%lu,",file->sn,file->id,file->dir_sn,file->base_object_arr_idx);
    output_line_len = (int)strlen(output_line);

    if((file->id[0] =='M')&&(file->id[1] =='F')&&(file->id[2] =='_')){ //In case this is a merged file - print from hashtable
        for(int i = 0 ; i < (file->base_objects_hash_merged->size_table) ;i++){
            pair = file->base_objects_hash_merged->table[i];
            while( pair != NULL && pair->key != NULL) {
                sprintf(temp , "%s,%u," , pair->key , pair->data);
                if((strlen(temp) + output_line_len + 1) > MAX_LINE_LEN){ //Check if line is too long for the buffer and print it to the file
                    fprintf(csv_output_file , "%s" ,output_line);//Print the Current line to file
                    memset(output_line,0,strlen(output_line)); //Clear Output Line Buffer
                }
                strcat(output_line , temp);
                output_line_len  = (int)strlen(output_line);

                pair = pair->next;
            }
        }
        strcat(output_line , "\n");
        fprintf(csv_output_file , "%s" ,output_line);
    } else { //In Case this isn't a merged file
        for(int i = 0 ; i < file->base_object_arr_idx ; i++){
            sprintf(temp , "%lu,%u," , ((file->base_objects_arr)[i])->sn , ((file->base_objects_arr)[i])->size);
            if((strlen(temp) + output_line_len + 1) > MAX_LINE_LEN){ //Check if line is too long for the buffer and print it to the file
                fprintf(csv_output_file , "%s" ,output_line);//Print the Current line to file
                memset(output_line,0,strlen(output_line)); //Clear Output Line Buffer
            }
            strcat(output_line , temp);
            output_line_len  = (int)strlen(output_line);
        }
        strcat(output_line , "\n");
        fprintf(csv_output_file , "%s" ,output_line);
    }
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

    dir->upd_files_array_idx = 0;
    dir->upd_subdirs_array_idx = 0;

    dir->parent_dir_sn = parent_dir_sn;

    dir->files_array = memory_pool_alloc(memory_pool , (sizeof(unsigned long)*num_of_files));
    if(dir->files_array== NULL){
        return NULL;
    }
    //Number of updated files can grow by at most 1 - addition of merged file
    dir->upd_files_array = memory_pool_alloc(memory_pool , (sizeof(unsigned long)*(num_of_files + 1)));
    if(dir->upd_files_array== NULL){
        return NULL;
    }

    //number of subdirectories can only stay the same or decrease
    dir->dirs_array = memory_pool_alloc(memory_pool , (sizeof(unsigned long)*num_of_sub_dirs));
    if(dir->dirs_array == NULL){
        return NULL;
    }
    dir->upd_subdirs_array = memory_pool_alloc(memory_pool , (sizeof(unsigned long)*num_of_sub_dirs));
    if(dir->upd_subdirs_array == NULL){
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

void print_dir_to_csv(Dir dir , char *output_line , FILE* csv_output_file){
    assert(dir);
    char temp[100];
    //Determine if root or not
    char dir_type;
    if(dir->sn == dir->parent_dir_sn){ //It's root directory
        dir_type = 'R';
    } else { // It's a regular directory
        dir_type = 'D';
    }

    sprintf(output_line , "%c,%lu,%s,%lu,%d,%d," ,dir_type, dir->sn, dir->id, dir->parent_dir_sn,
            dir->upd_subdirs_array_idx, dir->upd_files_array_idx);
    for(int i = 0 ; i < dir->upd_subdirs_array_idx ; i++){
        sprintf(temp , "%lu," , (dir->upd_subdirs_array)[i]);
        strcat(output_line , temp);
    }
    for(int i = 0 ; i < dir->upd_files_array_idx ; i++){
        sprintf(temp , "%lu," , (dir->upd_files_array)[i]);
        strcat(output_line , temp);
    }
    strcat(output_line , "\n");
    fprintf(csv_output_file , "%s" ,output_line);
}
/* ******************* END ******************* Directory STRUCT Functions ******************* END ******************* */

//#endif //DEDUPLICATIONPROJ_HEURISTIC_OBJECTSTRUCTURES_H
