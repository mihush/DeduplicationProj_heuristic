//
// Created by mihush on 17/03/2018.
//

#include "TextParsingUtilities.h"

/*****************************************~ Implementation ~*****************************************/

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

unsigned int determine_Merged_File_Base_Object_HT_Size(unsigned long num_base_objects , char dedup_type , int goal_depth){
    unsigned int ht_size;

    //unsigned int magic_number = 1;

    /*
    if((goal_depth > 4) && (goal_depth < 8)){
        magic_number = 2.5;
    }
    if(dedup_type == 'B'){ //Block Level Deduplication
        if(num_base_objects <= 1000000){ //Less than 1,000,000
            ht_size = num_base_objects/(goal_depth * 2 * magic_number);
        } else if (1000000 < num_base_objects  && num_base_objects <= 2000000){ // 1,000,000 < x <= 2,000,000
            ht_size = num_base_objects/(goal_depth * 2.5 * magic_number);
        } else if (2000000 < num_base_objects  && num_base_objects <= 3000000){ // 2,000,000 < x <= 3,000,000
            ht_size = num_base_objects/(goal_depth * 3 * magic_number);
        } else if (3000000 < num_base_objects  && num_base_objects <= 4000000){ // 3,000,000 < x <= 4,000,000
            ht_size = num_base_objects/(goal_depth * 3.5 * magic_number);
        } else if (4000000 < num_base_objects  && num_base_objects <= 5000000){ // 4,000,000 < x <= 5,000,000
            ht_size = num_base_objects/(goal_depth * 4 * magic_number);
        } else if (5000000 < num_base_objects  && num_base_objects <= 6000000){ // 5,000,000 < x <= 6,000,000
            ht_size = num_base_objects/(goal_depth * 4.5 * magic_number);
        } else if (6000000 < num_base_objects  && num_base_objects <= 7000000){ // 6,000,000 < x <= 7,000,000
            ht_size = num_base_objects/(goal_depth * 5 * magic_number);
        } else if (7000000 < num_base_objects  && num_base_objects <= 8000000){ // 7,000,000 < x <= 8,000,000
            ht_size = num_base_objects/(goal_depth * 5.5 * magic_number);
        } else if (8000000 < num_base_objects  && num_base_objects <= 9000000){ // 8,000,000 < x <= 9,000,000
            ht_size = num_base_objects/(goal_depth * 6 * magic_number);
        } else if (9000000 < num_base_objects  && num_base_objects <= 10000000){ // 9,000,000 < x <= 10,000,000
            ht_size = num_base_objects/(goal_depth * 6.5 * magic_number);
        } else if (10000000 < num_base_objects  && num_base_objects <= 12000000){ // 10,000,000 < x <= 12,000,000
            ht_size = num_base_objects/(goal_depth * 7 * magic_number);
        } else if (12000000 < num_base_objects  && num_base_objects <= 14000000){ // 12,000,000 < x <= 14,000,000
            ht_size = num_base_objects/(goal_depth * 7.5 * magic_number); //Based on 13,000,000 - Division by 100 way fine
        } else if (14000000 < num_base_objects  && num_base_objects <= 16000000){ // 14,000,000 < x <= 16,000,000
            ht_size = num_base_objects/(goal_depth * 8 * magic_number);
        } else if (16000000 < num_base_objects  && num_base_objects <= 18000000){ // 16,000,000 < x <= 18,000,000
            ht_size = num_base_objects/(goal_depth * 10 * magic_number);
        } else if (18000000 < num_base_objects  && num_base_objects <= 20000000){ // 18,000,000 < x <= 20,000,000
            ht_size = num_base_objects/(goal_depth * 11 * magic_number);
        } else if (20000000 < num_base_objects  && num_base_objects <= 22000000){ // 20,000,000 < x <= 22,000,000
            ht_size = num_base_objects/(goal_depth * 13 * magic_number);
        } else if (22000000 < num_base_objects  && num_base_objects <= 24000000){ // 22,000,000 < x <= 24,000,000
            ht_size = num_base_objects/(goal_depth * 15 * magic_number);
        } else if (24000000 < num_base_objects  && num_base_objects <= 26000000){ // 22,000,000 < x <= 24,000,000
            ht_size = num_base_objects/(goal_depth * 17);
        } else if (26000000 < num_base_objects  && num_base_objects <= 28000000){ // 26,000,000 < x <= 28,000,000
            ht_size = num_base_objects/(goal_depth * 25 * magic_number);
        } else if (28000000 < num_base_objects  && num_base_objects <= 30000000){ // 28,000,000 < x <= 30,000,000
            ht_size = num_base_objects/(goal_depth * 35 * magic_number);
        } else if (30000000 < num_base_objects  && num_base_objects <= 35000000){ // 30,000,000 < x <= 35,000,000
            ht_size = num_base_objects/(goal_depth * 45 * magic_number);
        } else if (35000000 < num_base_objects  && num_base_objects <= 40000000){ // 35,000,000 < x <= 40,000,000
            ht_size = num_base_objects/(goal_depth * 55 * magic_number);
        } else {
            ht_size = num_base_objects/(goal_depth * 85 * magic_number);
        }
    } else { //File Level Deduplication
        if(num_base_objects <= 100000){ //Less than 100,000
            ht_size = num_base_objects/(goal_depth * 2 * magic_number);
        } else if (100000 < num_base_objects  && num_base_objects <= 200000){ // 100,000 < x <= 200,000
            ht_size = num_base_objects/(goal_depth * 2.5 * magic_number);
        } else if (200000 < num_base_objects  && num_base_objects <= 300000){ // 200,000 < x <= 300,000
            ht_size = num_base_objects/(goal_depth * 3 * magic_number);
        } else if (300000 < num_base_objects  && num_base_objects <= 400000){ // 300,000 < x <= 400,000
            ht_size = num_base_objects/(goal_depth * 3.5 * magic_number);
        } else if (400000 < num_base_objects  && num_base_objects <= 500000){ // 400,000 < x <= 500,000
            ht_size = num_base_objects/(goal_depth * 4 * magic_number);
        } else if (500000 < num_base_objects  && num_base_objects <= 600000){ // 500,000 < x <= 600,000
            ht_size = num_base_objects/(goal_depth * 4.5 * magic_number);
        } else if (600000 < num_base_objects  && num_base_objects <= 700000){ // 600,000 < x <= 700,000
            ht_size = num_base_objects/(goal_depth * 5 * magic_number);
        } else if (700000 < num_base_objects  && num_base_objects <= 800000){ // 700,000 < x <= 800,000
            ht_size = num_base_objects/(goal_depth * 5.5 * magic_number);
        } else if (800000 < num_base_objects  && num_base_objects <= 900000){ // 800,000 < x <= 900,000
            ht_size = num_base_objects/(goal_depth * 6 * magic_number);
        } else if (900000 < num_base_objects  && num_base_objects <= 1000000){ // 900,000 < x <= 1,000,000
            ht_size = num_base_objects/(goal_depth * 6.5 * magic_number);
        } else if (1000000 < num_base_objects  && num_base_objects <= 1200000){ // 1,000,000 < x <= 1,200,000
            ht_size = num_base_objects/(goal_depth * 7 * magic_number);
        } else if (1200000 < num_base_objects  && num_base_objects <= 1400000){ // 1,200,000 < x <= 1,400,000
            ht_size = num_base_objects/(goal_depth * 7.5 * magic_number);
        } else if (1400000 < num_base_objects  && num_base_objects <= 1600000){ // 1,400,000 < x <= 1,600,000
            ht_size = num_base_objects/(goal_depth * 8 * magic_number);
        } else if (1600000 < num_base_objects  && num_base_objects <= 1800000){ // 1,600,000 < x <= 1,800,000
            ht_size = num_base_objects/(goal_depth * 8.5 * magic_number);
        } else if (1800000 < num_base_objects  && num_base_objects <= 2000000){ // 1,800,000 < x <= 2,000,000
            ht_size = num_base_objects/(goal_depth * 9 * magic_number);
        } else if (2000000 < num_base_objects  && num_base_objects <= 2200000){ // 2,000,000 < x <= 2,200,000
            ht_size = num_base_objects/(goal_depth * 11 * magic_number);
        } else if (2200000 < num_base_objects  && num_base_objects <= 2400000){ // 2,200,000 < x <= 2,400,000
            ht_size = num_base_objects/(goal_depth * 13 * magic_number);
        } else if (2400000 < num_base_objects  && num_base_objects <= 2600000){ // 2,400,000 < x <= 2,600,000
            ht_size = num_base_objects/(goal_depth * 15 * magic_number);
        } else if (2600000 < num_base_objects  && num_base_objects <= 2800000){ // 2,600,000 < x <= 2,800,000
            ht_size = num_base_objects/(goal_depth * 21 * magic_number);
        } else if (2800000 < num_base_objects  && num_base_objects <= 3000000){ // 2,800,000 < x <= 3,000,000
            ht_size = num_base_objects/(goal_depth * 25 * magic_number);
        } else if (3000000 < num_base_objects  && num_base_objects <= 3500000){ // 3,000,000 < x <= 3,500,000
            ht_size = num_base_objects/(goal_depth * 30 * magic_number);
        } else if (3500000 < num_base_objects  && num_base_objects <= 4000000){ // 3,500,000 < x <= 4,000,000
            ht_size = num_base_objects/(goal_depth * 40 * magic_number);
        } else {
            ht_size = num_base_objects/(goal_depth * 70 * magic_number);
        }
    }

    if(ht_size < 5){ // Just in case
        if(num_base_objects > 100){
            ht_size = 100;
        } else {
            ht_size = num_base_objects;
        }

    }*/

    if(dedup_type == 'B'){
        if(num_base_objects < 100000){ //Block Level
            ht_size = num_base_objects;
        } else {
            ht_size = 100000;
        }
    } else{ //File Level
        if(num_base_objects < 10000){
            ht_size = num_base_objects;
        } else {
            ht_size = 10000;
        }
    }

    return ht_size;
}



File readFileLine(FILE* input_file, char* line, PMemory_pool memory_pool,
                  Base_Object* base_objects_arr , unsigned int ht_size){
    File file = NULL;
    char* file_id;
    unsigned long file_sn = 0;
    unsigned long parent_dir_sn = 0;
    unsigned long num_base_objects = 0;
    int input_line_len = strlen(line);

    char* tok = NULL;
    char sep[2] = ",";

    tok = strtok(line , sep); //reading the flag ("F")
    tok = strtok(NULL , sep); // reading the file_sn
    file_sn = atol(tok);
    file_id = strtok(NULL , sep); // reading the file_id

    // Block Level: F - file_sn - file_id - dir_sn - num_blocks - block1_sn - block1_size - ....
    // File Level : F - file_sn - file_id - dir_sn - num_blocks - physical1_sn - physical1_size

    // reading parent_sir_sn
    tok = strtok(NULL , sep);
    parent_dir_sn = atol(tok);
    // reading num_base_objects
    tok = strtok(NULL , sep);
    num_base_objects = atol(tok);

    file = file_create(file_sn, file_id, parent_dir_sn, num_base_objects, 0, false, ht_size , memory_pool);

    // Check if we have an extremely big line which need to be parse fragmented
    // TODO - check this function - CAREFULLY !!!!!
    if(((input_line_len + 1) == MAX_LINE_LEN) && (line[input_line_len-1] != '\n')){ //Enter This condition only if we haven't read the entire line
        read_fragmented_line_File(input_file, line,input_line_len, memory_pool, base_objects_arr , file);
    }else{ //The Line is not fragmented proceed as regular
        unsigned long base_object_sn = 0;
        unsigned int base_object_size = 0;
        for(int i = 0 ; i < num_base_objects ; i++){
            tok = strtok(NULL , sep);
            base_object_sn = atol(tok);
            tok = strtok(NULL , sep);
            base_object_size = atoi(tok);

            // In case we have files which share the same base_object don't need to create him again
            if(base_objects_arr[base_object_sn] == NULL){
                base_objects_arr[base_object_sn] = base_object_create(base_object_sn, base_object_size, memory_pool);
            }
            file->base_objects_arr[i] = base_objects_arr[base_object_sn];
            file->size += base_object_size;
            (file->base_object_arr_idx)++;
        }
    }
    return file;
}

Base_Object readBaseObjectLine(char *line, PMemory_pool memory_pool , Base_Object* base_objects_arr){
    char* base_object_id = NULL;
    unsigned long base_object_sn = 0;
    unsigned int shared_by_num_files = 0;
    char* tok = NULL;
    char sep[2] = ",";

    tok = strtok(line , sep); //reading the flag ("B/P")
    tok = strtok(NULL , sep); //reading base_object_sn
    base_object_sn = (unsigned long)atol(tok);
    base_object_id = strtok(NULL , sep); //reading Block_id

    tok = strtok(NULL , sep); //reading num_of_files
    shared_by_num_files = atoi(tok);
    Base_Object base_object_to_update = base_objects_arr[base_object_sn];
    base_object_update(base_object_to_update, base_object_id, shared_by_num_files, memory_pool);
    return base_object_to_update;
}

Dir readDirLine(FILE* input_file, char* line , PMemory_pool memory_pool){
    char* dir_id = NULL;
    unsigned long dir_sn = 0;
    unsigned long parent_dir_sn = 0;
    unsigned long num_of_sub_dirs = 0;
    unsigned long num_of_files = 0;
    int input_line_len = (int)strlen(line);

    char* tok = NULL;
    char sep[2] = ",";

    tok = strtok(line , sep); //reading the flag ("D/R")
    tok = strtok(NULL , sep); //reading dir_sn
    dir_sn = (unsigned long)atol(tok);

    dir_id = strtok(NULL , sep); //reading dir_id

    tok = strtok(NULL , sep); //reading parent_dir_sn
    parent_dir_sn = (unsigned long)atol(tok);

    tok = strtok(NULL , sep); //reading num_of_sub_dirs
    num_of_sub_dirs = (unsigned long)atol(tok);
    tok = strtok(NULL , sep); //reading num_of_files
    num_of_files = (unsigned long)atol(tok);

    Dir directory = dir_create(dir_id , dir_sn , parent_dir_sn , num_of_files , num_of_sub_dirs , memory_pool);

    // Need to get the directory using fragments
    if(((input_line_len + 1) == MAX_LINE_LEN) && (line[input_line_len-1] != '\n')){ //Enter This condition only if we haven't read the entire line
        read_fragmented_line_Dir(input_file, line, input_line_len, memory_pool, directory, num_of_sub_dirs, num_of_files);
    }else { //The Line is not fragmented proceed as regular
        //reading dir_sn
        unsigned long sub_dir_sn = 0;
        for (int i = 0; i < num_of_sub_dirs; i++) {
            tok = strtok(NULL, sep);
            sub_dir_sn = (unsigned long)atol(tok);
            add_sub_dir_sn_to_dir(directory, sub_dir_sn, i); //add sub_dir to directory
        }

        //reading file_sn
        unsigned long sub_file_sn = 0;
        for (int i = 0; i < num_of_files; i++) {
            tok = strtok(NULL, sep);
            sub_file_sn = (unsigned long)atol(tok);
            add_file_sn_to_dir(directory, sub_file_sn, i); //add file to directory
        }
    }
    return directory;
}

void add_base_object_to_merge_file(File merged_file, File file_to_insert, PMemory_pool memory_pool, Base_Object* base_object_array ){
    Base_Object base_object = NULL;
    unsigned long merged_file_sn = merged_file->sn;

    for(int i = 0 ; i < file_to_insert->num_base_objects ; i++){ //Go over all base objects that belong the file to be merged
        bool object_exists = false, is_first_object = false;
        unsigned long base_object_sn = ((file_to_insert->base_objects_arr))[i]->sn; //the sn of the base object to be added
        char base_object_sn_str[FILE_ID_LEN]; // Get the sn of the base object in string format
        sprintf(base_object_sn_str , "%lu" , base_object_sn);

        base_object = base_object_array[base_object_sn]; //Get The pointer to the base object element from the general array
        //Try adding the object to the merged file hashtable
        ht_set(merged_file->base_objects_hash_merged , base_object_sn_str , &object_exists , base_object->size , memory_pool);
        //If the block doesn't exist it is added to the hashtable ,  otherwise it is already there

        if(object_exists == false){ //Check if block exists already - do not increase counter
            bool file_exists = false;
            if(merged_file->base_object_arr_idx == 0){
                is_first_object = true;
            }
            (merged_file->base_object_arr_idx)++;
            //Check if first physical file and changed id
            if(is_first_object){
                char new_file_id[FILE_ID_LEN];
                if(!((file_to_insert->id)[0] == 'M' && (file_to_insert->id)[1] == 'F')){
                    sprintf(new_file_id , "MF_");
                } else {
                    new_file_id[0] = '\0';
                }
                strcat(new_file_id , file_to_insert->id);
                strcpy(merged_file->id , new_file_id);
            }


            //TODO RESET Hash Size HERE
            if(base_object->output_files_ht->size_table != (base_object->shared_by_num_files)){
                printf("Weird Bug Strikes Again =[ \n");
                base_object->output_files_ht->size_table = base_object->shared_by_num_files;
            }

            // Update the base object to contain the id of the merged file
            ht_set(base_object->output_files_ht, merged_file->id, &file_exists , merged_file->sn , memory_pool);
            if(file_exists == false){ //Check for memory allocation
                (base_object->files_array_updated)[base_object->output_updated_idx] = merged_file_sn;
                (base_object->output_updated_idx)++;
            }
        }
    }
}

void move_files_to_output_array(Dir current_dir , File* files_array , File* output_files_array ,
                                unsigned long* output_files_idx, PMemory_pool memory_pool){
    // add all of the files to output_files_array
    for(int f = 0 ; f < current_dir->num_of_files  ; f++){
        //Update the sn of the inserted file
        File curr_file = files_array[(current_dir->files_array)[f]]; // Get file ptr from files array
        curr_file->dir_sn = current_dir->sn;

        // Update file sn with the global output index
        curr_file->sn = *output_files_idx;
        output_files_array[*output_files_idx] = curr_file;
        (*output_files_idx)++;

        //update parent directory with new file sn
        (current_dir->upd_files_array)[current_dir->upd_files_array_idx] = curr_file->sn;
        (current_dir->upd_files_array_idx)++;

        // Update file_sn at each base_object containing this file
        for(int j = 0 ; j < curr_file->num_base_objects ; j++){
            bool file_exists = false;
            Base_Object curr_object = (curr_file->base_objects_arr)[j];

            ht_set(curr_object->output_files_ht, curr_file->id, &file_exists, curr_file->sn , memory_pool);
            if(file_exists == false){
                (curr_object->files_array_updated)[(curr_object->output_updated_idx)] = curr_file->sn;
                (curr_object->output_updated_idx)++;
            }
        }
    }
}

void update_dir_values(Dir current_dir , int goal_depth, Dir* dirs_array, unsigned long num_dirs,
                       File* files_array,  unsigned long num_files,
                       Base_Object* base_object_array, unsigned long num_base_object,
                       File* output_files_array , unsigned long* output_files_idx,
                       Dir* output_dirs_array , unsigned long* output_dirs_idx, int parent_depth,
                       unsigned int merged_file_ht_size , PMemory_pool memory_pool){
    int current_depth = 0;
    unsigned long new_sub_dir_sn = 0;
    if(current_dir == NULL){
        return;
    }
    // Check if current dir is a root directory or not
    if(current_dir->sn == current_dir->parent_dir_sn){
        current_depth = current_dir->depth;
    } else {
        current_dir->depth = (parent_depth + 1);
        current_depth = current_dir->depth;
    }

    // STOP CONDITIONS - stop if you have reached the leaves meaning a folder with no subdirs or files
    if(current_dir->num_of_subdirs == 0){
        if(current_dir->num_of_files == 0){
            return;
        } else { //There are still some files in the directory
            if(current_depth <= (goal_depth - 1)){
                move_files_to_output_array(current_dir, files_array, output_files_array, output_files_idx, memory_pool);
            }
            else { // current_depth > (goal_depth -1)
                // Add all the file blocks to the merged file - meaning current_dir->mergedFile
                assert(current_dir->merged_file);
                for(int f = 0 ; f < current_dir->num_of_files  ; f++) {
                    //merge all blocks of files_array[(current_dir->files_array)[f]] to current_dir->merged_file
                    add_base_object_to_merge_file(current_dir->merged_file, files_array[(current_dir->files_array)[f]], memory_pool , base_object_array);
                }
            }
            return;
        }
    }

    // CALCULATE RECURSION - Get Here ONLY if we have dirs to process !!!!!!!!!
    if(current_depth < (goal_depth - 1)){ //we HAVEN'T Reached the desired depth
        move_files_to_output_array(current_dir, files_array, output_files_array, output_files_idx, memory_pool);
        for(int d = 0 ; d < current_dir->num_of_subdirs ; d++){
            // root dir contains its own sn in the subdirs array
            if((dirs_array[(current_dir->dirs_array)[d]])->sn == (dirs_array[(current_dir->dirs_array)[d]])->parent_dir_sn){
                continue;
            }
            // Add current subdir to output_dirs_array
            output_dirs_array[*output_dirs_idx] = dirs_array[(current_dir->dirs_array)[d]];
            output_dirs_array[*output_dirs_idx]->sn = *output_dirs_idx;
            new_sub_dir_sn = *output_dirs_idx;

            // Update the sn of the subdir in the parent subdir array
            (current_dir->dirs_array)[d] = *output_dirs_idx;
            output_dirs_array[*output_dirs_idx]->parent_dir_sn = current_dir->sn;
            (*output_dirs_idx)++;

            //Update parent directory with the subdirectories new sn
            (current_dir->upd_subdirs_array)[current_dir->upd_subdirs_array_idx] = new_sub_dir_sn;
            (current_dir->upd_subdirs_array_idx)++;

            //For each directory - call update_dir_values
            update_dir_values(output_dirs_array[(current_dir->dirs_array)[d]] ,goal_depth, dirs_array, num_dirs,
                              files_array, num_files, base_object_array, num_base_object,
                              output_files_array, output_files_idx, output_dirs_array, output_dirs_idx,
                              current_depth, merged_file_ht_size , memory_pool);
        }
    } else {//current_depth >= (goal_depth - 1) : we have Reached the desired depth
        if (current_depth == (goal_depth - 1)){
            //create new merged file and save it to output_files_array
            current_dir->merged_file = file_create(*output_files_idx , "Sarit_Hadad_12345678912345678123456789", current_dir->sn ,
                                                    num_base_object , 0 , true , merged_file_ht_size , memory_pool);
            if(current_dir->merged_file == NULL){
                printf("Empty Merged.....\n");
            }

            output_files_array[*output_files_idx] = current_dir->merged_file;
            (*output_files_idx)++;

            unsigned long sn_to_save = (current_dir->merged_file)->sn;
            //update parent directory with new file sn
            (current_dir->upd_files_array)[current_dir->upd_files_array_idx] = sn_to_save;
            (current_dir->upd_files_array_idx)++;
        }

        //merge all child file blocks to the merged file of the parent directory
        assert(current_dir->merged_file);
        for(int f = 0 ; f < current_dir->num_of_files  ; f++){
            //merge all blocks of files_array[(current_dir->files_array)[f]] to current_dir->merged_file
            add_base_object_to_merge_file(current_dir->merged_file, files_array[(current_dir->files_array)[f]], memory_pool, base_object_array);
        }

        //Update all directory depths : for each directory - call update_dir_values
        for(int j = 0 ; j < current_dir->num_of_subdirs ; j++){
            // In case this is a root directory - don't need to update
            if((dirs_array[(current_dir->dirs_array)[j]])->sn == (dirs_array[(current_dir->dirs_array)[j]])->parent_dir_sn){
                continue;
            }

            dirs_array[(current_dir->dirs_array)[j]]->depth = current_dir->depth + 1; //update dir depth
            dirs_array[(current_dir->dirs_array)[j]]->merged_file = current_dir->merged_file; //update dir merged file - taken from parent
            dirs_array[(current_dir->dirs_array)[j]]->parent_dir_sn = current_dir->sn; //update the directory's parent dir sn

            //For each directory - call update_dir_values
            update_dir_values(dirs_array[(current_dir->dirs_array)[j]] ,goal_depth,
                              dirs_array, num_dirs, files_array, num_files, base_object_array, num_base_object,
                              output_files_array, output_files_idx, output_dirs_array , output_dirs_idx,
                              current_depth, merged_file_ht_size , memory_pool);
        }
    }
};

void calculate_depth_and_merge_files(Dir* roots_array, int num_roots, Dir* dirs_array, unsigned long num_dirs,
                                     File* files_array,  unsigned long num_files,
                                     Base_Object * base_object_array, unsigned long num_base_object, int goal_depth,
                                     File* output_files_array , unsigned long* output_files_idx ,
                                     Dir* output_dirs_array , unsigned long* output_dirs_idx ,
                                     unsigned int merged_file_ht_size , PMemory_pool memory_pool){

    for(int r = 0 ; r < num_roots ; r++){
        //Set each roots depth to be 0
        (roots_array[r])->depth = 0;

        // Add root to output_dirs_array - update the dir_sn from a global param
        output_dirs_array[*output_dirs_idx] = roots_array[r];
        (roots_array[r])->sn = *output_dirs_idx;
        (roots_array[r])->parent_dir_sn = *output_dirs_idx;
        (*output_dirs_idx)++;

        //add the root to be his own subdirectory
        (roots_array[r]->upd_subdirs_array)[roots_array[r]->upd_subdirs_array_idx] = roots_array[r]->sn;
        (roots_array[r]->upd_subdirs_array_idx)++;

        update_dir_values(roots_array[r] , goal_depth, dirs_array, num_dirs, files_array, num_files,
                          base_object_array, num_base_object, output_files_array , output_files_idx,
                          output_dirs_array , output_dirs_idx, 0, merged_file_ht_size , memory_pool);
    }
}

void print_output_csv_header(FILE *results_file, char dedup_type, char *input_files_list, int goal_depth,
                             unsigned long num_files_output, unsigned long num_dirs_output, unsigned long num_base_object){
    if(dedup_type == 'B'){
        fprintf(results_file ,"# Output type: block-level\n");
    } else if(dedup_type == 'F'){
        fprintf(results_file ,"# Output type: file-level\n");
    }

    fprintf(results_file ,"%s" , input_files_list);
    fprintf(results_file ,"# Target depth: %d\n" , goal_depth);
    fprintf(results_file ,"# Num files: %lu\n",num_files_output);
    fprintf(results_file ,"# Num directories: %lu\n",num_dirs_output);
    if(dedup_type == 'B'){
        fprintf(results_file ,"# Num Blocks: %lu\n", num_base_object);
    } else {
        fprintf(results_file ,"# Num physical files: %lu\n", num_base_object);
    }
}

unsigned int pow_aux(int x, int y){
    if (y == 0){
        return 1;
    }
    unsigned int res = x;
    int i;
    for (i = 1; i < y; i++){
        res *= x;
    }

    return res;
}

// Input file may contain extremely long lines -  over 22M chars
// The Solution is to read each line in fragments of buffer with const size
// For each line read the following conditions will be checked and treated accordingly:
//          - Buffer Contains the Entire Line.
//          - Line was cut in the middle : Either end with a comma or a sign or number or with a new line sign.
// fgets will read until the first new line, maximum bytes to read at once, or EOF, which ever is sent first
void read_fragmented_line_File(FILE* input_file, char* line,int input_line_len ,PMemory_pool memory_pool,
                               Base_Object* base_objects_arr , File file_obj){
    bool line_end_with_comma = false; //if the line ends with a comma it means we are in the middle of a line.
    char last_char = line[input_line_len - 1];

    if(last_char == ',') {
        line_end_with_comma = true;
    }

    // Variable to get all base_objects (Blocks) : their sn and size
    char* tok = NULL;
    unsigned long base_object_sn = 0;
    unsigned int base_object_size = 0;
    int counter_of_sn = 0; // Counter For object sns Read
    int counter_of_size = 0; // Counter For object sizes Read


    tok = strtok(NULL, ","); // Get the SN of the first Block
    while(strcmp(tok, "\n") != 0) {
        if (counter_of_size == counter_of_sn) {
            base_object_sn = (unsigned long)atol(tok);
            counter_of_sn++;
        } else { // (counter_of_size < counter_of_sn){
            base_object_size = (unsigned int)atoi(tok);
            counter_of_size++;
        }

        tok = strtok(NULL, ",");// Get Next Value

        //If we Reached the end of the Line - treat it differently
        if (!tok) {
            // There is a line over flow, we need to fix last block size
            bool last_line_ended_with_comma = line_end_with_comma;
            fgets(line, MAX_LINE_LEN, input_file);
            line_end_with_comma = false;
            if (line[strlen(line) - 1] == ',') {
                line_end_with_comma = true;
            }

            if (line[0] == ',') {
                last_line_ended_with_comma = true;
            }

            tok = strtok(line, ",");

            // Case the first char are a continued number from previous buffer
            if(!last_line_ended_with_comma){
                if (counter_of_sn > counter_of_size) { // Case we cut the row in the middle of base_object sn
                    base_object_sn *= pow_aux(10, strlen(tok));
                    base_object_sn += atoi(tok);
                    tok = strtok(NULL, ",");
                } else { // Case we cut the row in the middle of base_object size
                    base_object_size *= pow_aux(10, strlen(tok));
                    base_object_size += atoi(tok);
                    tok = strtok(NULL, ",");
                }
            }
        }

        if(counter_of_size == counter_of_sn){ //Finished Reading a Base Object
            if(base_objects_arr[base_object_sn] == NULL){ // create a New Block if it doesn't exist
                base_objects_arr[base_object_sn] = base_object_create(base_object_sn, base_object_size, memory_pool);
            }
            //Save pointer to the block in the File Object
            file_obj->base_objects_arr[file_obj->base_object_arr_idx] = base_objects_arr[base_object_sn];
            file_obj->size += base_object_size;
            (file_obj->base_object_arr_idx)++;
        }
    }
    return;
}

void read_fragmented_line_Dir(FILE* input_file, char* line, int input_line_len ,PMemory_pool memory_pool,
                              Dir dir_obj, unsigned long num_of_sub_dirs, unsigned long num_of_files){
    bool line_end_with_comma = false; //if the line ends with a comma it means we are in the middle of a line.
    char last_char = line[input_line_len - 1];
    unsigned int counter_files = 0, counter_sub_dirs = 0;

    if(last_char == ',') {
        line_end_with_comma = true;
    }

    // Variable to get all base_objects (Blocks) : their sn and size
    char* tok = NULL;
    unsigned long sn = 0;

    tok = strtok(NULL, ","); // Get the SN of the first Block
    while(strcmp(tok, "\n") != 0) { //Go On While We haven't finished reading the fragmented line
        sn = (unsigned long)atol(tok); //Convert sn to Unsigned Longs

        tok = strtok(NULL, ",");// Get Next Value - Take string until the first comma

        if (!tok) { //If we Reached the end of the Line - treat it differently
            // There is a line over flow, we need to fix last block size
            bool last_line_ended_with_comma = line_end_with_comma;
            fgets(line, MAX_LINE_LEN, input_file); //Get Another Line

            line_end_with_comma = false;
            if (line[strlen(line) - 1] == ',') {
                line_end_with_comma = true;
            }

            if (line[0] == ',') {
                last_line_ended_with_comma = true;
            }

            tok = strtok(line, ","); // Read the rest of the line

            // Case the first char are a continued number from previous buffer
            if(!last_line_ended_with_comma){
                sn *= pow_aux(10, strlen(tok));
                sn += atoi(tok);
                tok = strtok(NULL, ",");
            }
        }

        //Save sn of sub_dir or files od current directory
        //First appear directories , After Them there are files
        if (counter_sub_dirs == num_of_sub_dirs) {
            // Insert the dir files - in case all sub_dirs already inserted
            dir_obj->files_array[counter_files] = sn;
            counter_files++;
        } else { // (counter_sub_dirs < num_of_sub_dirs)
            assert(counter_files < num_of_files);
            dir_obj->dirs_array[counter_sub_dirs] = sn;
            counter_sub_dirs++;
        }

    }
    return;
}