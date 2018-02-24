////
//// Created by Polina on 22-Feb-18.
////
//#include "ObjectStructures.h"
///* ****************** START ****************** Block STRUCT Functions ****************** START ****************** */
//Block block_create(char* block_id , unsigned long block_sn , unsigned int block_size){
//    Block block = malloc(sizeof(*block)); //create a block
//    if(block == NULL){ //Check memory allocation was successful
//        printf("(Block)--> Adding block to file - Allocation Error (1) \n");
//        return NULL;
//    }
//
//    block->block_id = malloc(sizeof(char)*(BLOCK_ID_LEN + 1)); //allocate string for block_id
//    if(block->block_id == NULL){ //check successful allocation
//        free(block);
//        return NULL;
//    }
//    block->block_id = strcpy(block->block_id , block_id);
//    block->block_sn = block_sn;
//    block->shared_by_num_files = 0;
//    block->block_size = block_size;
//
//    block->files_ht = ht_createF('N');
//    if(block->files_ht == NULL){
//        free(block->block_id);
//        free(block);
//        return NULL;
//    }
//    return block;
//}
//
//void block_destroy(Block block){
//    assert(block);
//    free(block->block_id);
//    hashTableF_destroy(block->files_ht);
//    free(block);
//}
//
//long block_get_SN(Block block){
//    assert(block);
//    return block->block_sn;
//}
//
//char* block_get_ID(Block block){
//    assert(block);
//    return block->block_id;
//}
//
//ErrorCode block_add_file(Block block , File* file_obj){
//    if(file_obj == NULL || block == NULL){ //Check input is valid
//        return INVALID_INPUT;
//    }
//
//    EntryF result = ht_setF(block->files_ht, file_obj);
//    if(result == NULL){ //Check for memory allocation
//        return OUT_OF_MEMORY;
//    }
//
//    (block->shared_by_num_files)++;
////    printf("(Block)--> Containing file was added to block Successfully:\n");
////    printf("            - Block SN   : %lu \n" , block->block_sn);
////    printf("            - File  ID   : %s \n" , file_id);
//    return SUCCESS;
//}
///* ******************* END ******************* Block STRUCT Functions ******************* END ******************* */
///* ************************************************************************************************************** */
///* ************************************************************************************************************** */
///* ************************************************************************************************************** */
///* *************** START ************** File STRUCT Functions *************** START **************** */
//
//File file_create(char* file_id , unsigned int depth , unsigned long file_sn , unsigned int size ,
//                 unsigned long physical_sn){
//    File file = malloc(sizeof(*file));
//    if(file == NULL){
//        printf("(File)--> Creating file - Allocation Error (1) \n");
//        return NULL;
//    }
//
//    file->file_id = malloc(sizeof(char)* (FILE_ID_LEN + 1));
//    if(file->file_id == NULL){
//        printf("(File)--> Creating file - Allocation Error (2) \n");
//        free(file);
//        return NULL;
//    }
//
//    file->file_id = strcpy(file->file_id , file_id);
//    file->file_sn = file_sn;
//    file->dir_sn = 0; //not known in the time of creation
//    file->num_blocks = 0;
//    file->file_depth = depth;
//    file->file_size = size;
//    file->num_files = 1;
//    file->flag = 'P';
//    file->physical_sn = physical_sn; // will be updated from file_compare
//
//    file->blocks_list = listCreate(copy_block_info , free_block_info);
//    if(file->blocks_list == NULL){
//        printf("(File)--> Adding block to file - Allocation Error (3) \n");
//        free(file->file_id);
//        free(file);
//        return NULL;
//    }
//
//    file->files_ht = ht_createF('N');
//    if(file->files_ht == NULL){
//        free(file->file_id);
//        listDestroy(file->blocks_list);
//        free(file);
//        return NULL;
//    }
//
//    ht_setF(file->files_ht, file_id);
//    return file;
//}
//
//void file_destroy(File file){
//    assert(file);
//    free(file->file_id);
//    listDestroy(file->blocks_list);
//    free(file);
//}
//
//unsigned long file_get_SN(File file){
//    assert(file);
//    return file->file_sn;
//}
//
//char* file_get_ID(File file){
//    assert(file);
//    return file->file_id;
//}
//
//unsigned int file_get_depth(File file){
//    assert(file);
//    return file->file_depth;
//}
//
//int file_get_num_blocks(File file){
//    assert(file);
//    return file->num_blocks;
//}
//
//ErrorCode file_set_parent_dir_sn(File file , unsigned long dir_sn){
//    assert(file);
//    file->dir_sn = dir_sn;
//    return SUCCESS;
//}
//
//ErrorCode file_set_physical_sn(File file , unsigned long physical_file_sn){
//    assert(file);
//    file->physical_sn = physical_file_sn;
//    return SUCCESS;
//}
//
//ErrorCode file_set_logical_flag(File file){
//    file->flag = 'L';
//    return SUCCESS;
//}
//
//ErrorCode file_add_block(File file , char* block_id , int block_size){
//    if(file == NULL || block_id == NULL || block_size < 0){
//        return INVALID_INPUT;
//    }
//
//    Block_Info bi = malloc(sizeof(*bi));
//    if(bi == NULL){
//        printf("(File)--> Adding block to file - Allocation Error (1) \n");
//        return OUT_OF_MEMORY;
//    }
//    bi->id =  malloc(sizeof(char)*(strlen(block_id) +1));
//    if(bi->id == NULL){
//        printf("(File)--> Adding block to file - Allocation Error (2) \n");
//        free(bi);
//        return OUT_OF_MEMORY;
//    }
//    strcpy(bi->id , block_id);
//    bi->size = block_size;
//    ListResult res = listInsertLast(file->blocks_list , bi);
//
//    if(res != LIST_SUCCESS){
//        printf("(File)--> Adding block to file - List of files containing block allocation Error (3) \n");
//        free(bi->id);
//        free(bi);
//        return OUT_OF_MEMORY;
//    }
//
//    (file->num_blocks)++;
//    free(bi->id);
//    free(bi);
//    return SUCCESS;
//}
//
//
///* **************** END *************** File STRUCT Functions **************** END ***************** */
///* *************** START *************** Directory STRUCT Functions *************** START *************** */
//
//Dir dir_create(char* dir_id , unsigned int depth , unsigned long dir_sn){
//    Dir dir = malloc(sizeof(*dir));
//    if(dir == NULL){
//        printf("(Directory)--> Creating Directory - Allocation Error (1) \n");
//        return NULL;
//    }
//    dir->dir_id = malloc((sizeof(char)*DIR_NAME_LEN));
//    if(!(dir->dir_id)){
//        printf("(Directory)--> Creating Directory - Allocation Error (2) \n");
//        free(dir);
//        return NULL;
//    }
//    dir->dir_id = strcpy(dir->dir_id , dir_id);
//    dir->dir_depth = depth;
//    dir->dir_sn = dir_sn;
//    dir->num_of_files = 0;
//    dir->num_of_subdirs = 0;
//    dir->parent_dir_sn = 0; //  not known in the time of creation
//    dir->dirs_list = listCreate(copy_directory_info , free_dir_info);
//    dir->files_list = listCreate(copy_directory_info , free_dir_info);
//
//    if((!dir->files_list) || (!dir->dirs_list)){
//        printf("(Directory)--> Creating Directory - Allocation Error (3) \n");
//        free(dir->dir_id);
//        free(dir);
//        return NULL;
//    }
//
////    printf("(Directory)--> Created Directory Successfully:\n");
////    printf("              - SN    : %lu \n" , dir->dir_sn);
////    printf("              - ID    : %s \n" , dir->dir_id);
////    printf("              - Depth : %d \n" , dir->dir_depth);
//    return dir;
//}
//
//ErrorCode dir_set_parent_dir_sn(Dir dir , unsigned long sn){
//    assert(dir);
//    dir->parent_dir_sn = sn;
//    return SUCCESS;
//}
//
//void dir_destroy(Dir dir){
//    assert(dir);
//    free(dir->dir_id);
//    listDestroy(dir->dirs_list);
//    listDestroy(dir->files_list);
//    free(dir);
//}
//
//unsigned long dir_get_SN(Dir dir){
//    assert(dir);
//    return dir->dir_sn;
//}
//
//char* dir_get_ID(Dir dir){
//    assert(dir);
//    return dir->dir_id;
//}
//
//unsigned int dir_get_depth(Dir dir){
//    assert(dir);
//    return dir->dir_depth;
//}
//
//ErrorCode dir_add_file(Dir dir , unsigned long file_sn){
//    if(dir == NULL || file_sn < 0){
//        return INVALID_INPUT;
//    }
//    unsigned long* temp = malloc(sizeof(*temp));
//    if(temp == NULL){
//        printf("(Directory)--> Adding file to Directory - Allocation Error (1) \n");
//        return OUT_OF_MEMORY;
//    }
//    *temp = file_sn;
//    ListResult res = listInsertFirst(dir->files_list , temp);
//    if(res != LIST_SUCCESS){
//        free(temp);
//        printf("(Directory)--> Adding file to Directory - Allocation Error (2) \n");
//        return OUT_OF_MEMORY;
//
//    }
//    (dir->num_of_files)++;
////    printf("(Directory)--> File was added to Directory Successfully:\n");
////    printf("            - File  SN     : %lu \n" , file_sn);
////    printf("            - Directory SN : %lu \n" , dir->dir_sn);
//    return SUCCESS;
//}
//
//ErrorCode dir_add_sub_dir(Dir dir , unsigned long dir_sn){
//    if(dir == NULL || dir_sn < 0){
//        return INVALID_INPUT;
//    }
//    unsigned long* temp = malloc(sizeof(*temp));
//    if(temp == NULL){
//        printf("(Directory)--> Adding sub directory to Directory - Allocation Error (1) \n");
//        return OUT_OF_MEMORY;
//    }
//    *temp = dir_sn;
//    ListResult res = listInsertFirst(dir->dirs_list, temp);
//    if(res != LIST_SUCCESS){
//        free(temp);
//        printf("(Directory)--> Adding sub directory to Directory - Allocation Error (2) \n");
//        return OUT_OF_MEMORY;
//    }
//    (dir->num_of_subdirs)++;
////    printf("(Directory)--> Directory was added to Parent Directory Successfully:\n");
////    printf("            - Directory  SN : %lu \n" , dir_sn);
////    printf("            - Parent Dir SN : %lu \n" , dir->dir_sn);
//    return SUCCESS;
//}
///* **************** END **************** Directory STRUCT Functions **************** END **************** */
