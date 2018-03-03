
/********************** ~Includes~ **********************/

#include "ObjectStructures.h"

///* ****************** START ****************** Block STRUCT Functions ****************** START ****************** */

/*
 *  blockCreate - Creates a new Block with:
 *           - a given serial number
 *           - a hashed id
 *           - creates an empty files list
 *           - zeros the counter that contains the amount of files sharing this block
 */
Block block_create(char* block_id , unsigned long block_sn , unsigned int block_size){
    assert(block_sn && block_size!=0 && block_id);
    Block block = malloc(sizeof(*block)); //create a block
    if(block == NULL){ //Check memory allocation was successful
        printf("(Block)--> Adding block to file - Allocation Error (1) \n");
        return NULL;
    }

    block->block_id = malloc(sizeof(char)*(BLOCK_ID_LEN + 1)); //allocate string for block_id
    if(block->block_id == NULL){ //check successful allocation
        free(block);
        return NULL;
    }
    block->block_id = strcpy(block->block_id , block_id);
    block->block_sn = block_sn;
    block->shared_by_num_files = 0;
    block->block_size = block_size;

    //I think it's a redundant fild - I can't see why we need the files
    // contain this block in a hash or whatever...
//    block->files_ht = ht_createF('N');
//    if(block->files_ht == NULL){
//        free(block->block_id);
//        free(block);
//        return NULL;
//    }
    return block;
}

/*
 *  block_destroy - Destroys and frees space of a block structure
 */
void block_destroy(Block block){
    assert(block);
    free(block->block_id);
    free(block);
}

/*
 *  block_get_SN - returns the SN of the block
 */
long block_get_SN(Block block){
    assert(block);
    return block->block_sn;
}

/*
 *  block_get_ID - Returns the hashed id of the block
 */
char* block_get_ID(Block block){
    assert(block);
    return block->block_id;
}

/*
 *  block_add_file - adds the file containing the block to the files list saved in the block
 */
/**** Maybe it's redundant ****/
//ErrorCode block_add_file(Block block , File file){
//    if(file == NULL || block == NULL){ //Check input is valid
//        return INVALID_INPUT;
//    }
//
//    EntryF result = ht_setF(block->files_ht, file->file_id);
//    if(result == NULL){ //Check for memory allocation
//        return OUT_OF_MEMORY;
//    }
//
//    (block->shared_by_num_files)++;
//
//    return SUCCESS;
//}

///* *************** START ************** File STRUCT Functions *************** START **************** */
/*
 *  file_create - Creates a new file object with:
 *                      - file id - a hashed id as appears in the input file
 *                      - depth
 *                      -file sn - running index on all files in the filesystem
 *                      - dir sn
 *
 */
File file_create(char* file_id , unsigned int depth , unsigned long file_sn , unsigned int size ,
                 unsigned long physical_sn){
    File file = malloc(sizeof(*file));
    if(file == NULL){
        printf("(File)--> Creating file - Allocation Error (1) \n");
        return NULL;
    }

    file->file_id = malloc(sizeof(char)* (FILE_ID_LEN + 1));
    if(file->file_id == NULL){
        printf("(File)--> Creating file - Allocation Error (2) \n");
        free(file);
        return NULL;
    }

    file->file_id = strcpy(file->file_id , file_id);
    file->file_sn = file_sn;
    file->dir_sn = 0; //not known in the time of creation
    file->num_blocks = 0;
    file->file_depth = depth;
    file->file_size = size;
    file->num_files = 1;
    file->flag = 'P';
    file->physical_sn = physical_sn; // will be updated from file_compare

    file->blocks_list = listCreate(copy_block_info , free_block_info);
    if(file->blocks_list == NULL){
        printf("(File)--> Adding block to file - Allocation Error (3) \n");
        free(file->file_id);
        free(file);
        return NULL;
    }

//    file->files_ht = ht_createF('N');
//    if(file->files_ht == NULL){
//        free(file->file_id);
//        listDestroy(file->blocks_list);
//        free(file);
//        return NULL;
//    }
//
//    ht_setF(file->files_ht, file_id);
    return file;
}

/*
 *  file_destroy - Destroys and frees space of a file structure
 */
void file_destroy(File file){
    assert(file);
    free(file->file_id);
    listDestroy(file->blocks_list);
    free(file);
}

/*
 *  file_get_SN - returns the SN of the file
 */
unsigned long file_get_SN(File file){
    assert(file);
    return file->file_sn;
}

/*
 * file_get_ID - returns the hashed ID of the file
 */
char* file_get_ID(File file){
    assert(file);
    return file->file_id;
}

/*
 *  file_get_depth - returns the depth of the file in the hierarchy
 */
unsigned int file_get_depth(File file){
    assert(file);
    return file->file_depth;
}

/*
 *  file_get_num_blocks - returns the number of blocks the file contains
 */
int file_get_num_blocks(File file){
    assert(file);
    return file->num_blocks;
}

/*
 *
 */
ErrorCode file_set_parent_dir_sn(File file , unsigned long dir_sn){
    assert(file && dir_sn!=0);
    file->dir_sn = dir_sn;
    return SUCCESS;
}
/*
 *
 */
ErrorCode file_set_physical_sn(File file , unsigned long physical_file_sn){
    assert(file && physical_file_sn!=0);
    file->physical_sn = physical_file_sn;
    return SUCCESS;
}

/*
 *
 */
ErrorCode file_set_logical_flag(File file){
    assert(file);
    file->flag = 'L';
    return SUCCESS;
}


/*
 *
 */
ErrorCode file_add_block(File file , char* block_id , int block_size){
    if(file == NULL || block_id == NULL || block_size < 0){
        return INVALID_INPUT;
    }

    Block_Info bi = malloc(sizeof(*bi));
    if(bi == NULL){
        printf("(File)--> Adding block to file - Allocation Error (1) \n");
        return OUT_OF_MEMORY;
    }
    bi->id =  malloc(sizeof(char)*(strlen(block_id) +1));
    if(bi->id == NULL){
        printf("(File)--> Adding block to file - Allocation Error (2) \n");
        free(bi);
        return OUT_OF_MEMORY;
    }
    strcpy(bi->id , block_id);
    bi->size = block_size;
    ListResult res = listInsertLast(file->blocks_list , bi);

    if(res != LIST_SUCCESS){
        printf("(File)--> Adding block to file - List of files containing block allocation Error (3) \n");
        free(bi->id);
        free(bi);
        return OUT_OF_MEMORY;
    }

    (file->num_blocks)++;
    free(bi->id);
    free(bi);
    return SUCCESS;
}

///* *************** START *************** Directory STRUCT Functions *************** START *************** */
/*
 * dir_create - Creates a new Directory object with:
 *                      - dir_id
 *                      - dir_sn
 *                      - depth
 */
Dir dir_create(char* dir_id , unsigned int depth , unsigned long dir_sn){
    Dir dir = malloc(sizeof(*dir));
    if(dir == NULL){
        printf("(Directory)--> Creating Directory - Allocation Error (1) \n");
        return NULL;
    }
    dir->dir_id = malloc((sizeof(char)*DIR_NAME_LEN));
    if(!(dir->dir_id)){
        printf("(Directory)--> Creating Directory - Allocation Error (2) \n");
        free(dir);
        return NULL;
    }
    dir->dir_id = strcpy(dir->dir_id , dir_id);
    dir->dir_depth = depth;
    dir->dir_sn = dir_sn;
    dir->num_of_files = 0;
    dir->num_of_subdirs = 0;
    dir->parent_dir_sn = 0; //  not known in the time of creation
    dir->dirs_list = listCreate(copy_directory_info , free_dir_info);
    dir->files_list = listCreate(copy_directory_info , free_dir_info);

    if((!dir->files_list) || (!dir->dirs_list)){
        printf("(Directory)--> Creating Directory - Allocation Error (3) \n");
        free(dir->dir_id);
        free(dir);
        return NULL;
    }

//    printf("(Directory)--> Created Directory Successfully:\n");
//    printf("              - SN    : %lu \n" , dir->dir_sn);
//    printf("              - ID    : %s \n" , dir->dir_id);
//    printf("              - Depth : %d \n" , dir->dir_depth);
    return dir;
}

/*
 *
 */
ErrorCode dir_set_parent_dir_sn(Dir dir , unsigned long sn){
    assert(dir);
    dir->parent_dir_sn = sn;
    return SUCCESS;
}

/*
 * dir_destroy - Destroy struct of Directory
 */
void dir_destroy(Dir dir){
    assert(dir);
    free(dir->dir_id);
    listDestroy(dir->dirs_list);
    listDestroy(dir->files_list);
    free(dir);
}

/*
 * dir_get_SN - Return the sn of directory
 */
unsigned long dir_get_SN(Dir dir){
    assert(dir);
    return dir->dir_sn;
}

/*
 * dir_get_ID - Return the ID of directory
 */
char* dir_get_ID(Dir dir){
    assert(dir);
    return dir->dir_id;
}

/*
 * dir_get_depth - Return the depth of the directory
 */
unsigned int dir_get_depth(Dir dir){
    assert(dir);
    return dir->dir_depth;
}

/* Adding file into the directory */
ErrorCode dir_add_file(Dir dir , unsigned long file_sn){
    if(dir == NULL || file_sn < 0){
        return INVALID_INPUT;
    }
    unsigned long* temp = malloc(sizeof(*temp));
    if(temp == NULL){
        printf("(Directory)--> Adding file to Directory - Allocation Error (1) \n");
        return OUT_OF_MEMORY;
    }
    *temp = file_sn;
    ListResult res = listInsertFirst(dir->files_list , temp);
    if(res != LIST_SUCCESS){
        free(temp);
        printf("(Directory)--> Adding file to Directory - Allocation Error (2) \n");
        return OUT_OF_MEMORY;

    }
    (dir->num_of_files)++;

    return SUCCESS;
}

/* Adding sub_dir into the directory */
ErrorCode dir_add_sub_dir(Dir dir , unsigned long dir_sn){
    if(dir == NULL || dir_sn < 0){
        return INVALID_INPUT;
    }
    unsigned long* temp = malloc(sizeof(*temp));
    if(temp == NULL){
        printf("(Directory)--> Adding sub directory to Directory - Allocation Error (1) \n");
        return OUT_OF_MEMORY;
    }
    *temp = dir_sn;
    ListResult res = listInsertFirst(dir->dirs_list, temp);
    if(res != LIST_SUCCESS){
        free(temp);
        printf("(Directory)--> Adding sub directory to Directory - Allocation Error (2) \n");
        return OUT_OF_MEMORY;
    }
    (dir->num_of_subdirs)++;

    return SUCCESS;
}

/* **************** END **************** Directory STRUCT Functions **************** END **************** */
/* ****************************************** Function Declarations ******************************************** */

//#endif //DEDUPLICATIONPROJ_HEURISTIC_OBJECTSTRUCTURES_H
