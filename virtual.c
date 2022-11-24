//
// Created by SUND9 on 11/24/2022.
//
#include <stdio.h>
#include "oslabs.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int process_page_access_fifo(struct PTE page_table[TABLEMAX],int *table_cnt, int
page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp){
    if(page_table[page_number].is_valid == true){
        page_table[page_number].reference_count += 1;
        page_table[page_number].last_access_timestamp = current_timestamp;
        return page_table[page_number].frame_number;
    } else if ((page_table[page_number].is_valid == false) && (*frame_cnt > 0)){
        page_table[page_number].frame_number = frame_pool[*frame_cnt-1];
        *frame_cnt -= 1;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        page_table[page_number].is_valid = true;
        return page_table[page_number].frame_number;
    } else {
        bool flag_first = false;
        int temp_index = 0;
        int temp_frame = 0;
        int temp_arrival_stemp = 0;
        for (int index = 0; index < *table_cnt ; index++){
            if((page_table[index].is_valid == true) && (flag_first==
                                                        false)){
                temp_index = index;
                temp_frame = page_table[index].frame_number;
                temp_arrival_stemp =
                        page_table[index].arrival_timestamp;
                flag_first = true;
            }
            else if ((page_table[index].is_valid == true) &&
                     (temp_arrival_stemp > page_table[index].arrival_timestamp) && (flag_first==
                                                                                    true)){
                temp_arrival_stemp =
                        page_table[index].arrival_timestamp;
                temp_index = index;
                temp_frame = page_table[index].frame_number;
            }
        } if (flag_first == true){
            page_table[temp_index].arrival_timestamp = 0;
            page_table[temp_index].reference_count = 0;
            page_table[temp_index].last_access_timestamp = 0;
            page_table[temp_index].frame_number = -1;
            page_table[temp_index].is_valid = false;
            page_table[page_number].frame_number = temp_frame;
            page_table[page_number].arrival_timestamp = current_timestamp;
            page_table[page_number].is_valid = true;
            page_table[page_number].last_access_timestamp =
                    current_timestamp;
            page_table[page_number].reference_count = 1;
        }
        return temp_frame;
    }
}
