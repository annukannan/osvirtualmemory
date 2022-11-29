//
// Created by SUND9 on 11/24/2022.
//
#include <stdio.h>
#include "oslabs.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
/*
struct PTE {
  int is_valid;
  int frame_number;
  int arrival_timestamp;
  int last_access_timestamp;
  int reference_count;
};
*/
typedef struct PTE PTE;

int process_page_access_fifo(struct PTE page_table[TABLEMAX],int *table_cnt, int page_number,
        int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp){
    if (page_table[page_number].is_valid)
    {
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count++;
        return page_table[page_number].frame_number;
    }

    if (*frame_cnt > 0)
    {
        page_table[page_number].is_valid = 1;
        page_table[page_number].frame_number = frame_pool[--(*frame_cnt)];
        page_table[page_number].arrival_timestamp = current_timestamp;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        return page_table[page_number].frame_number;
    }

    int index = -1;
    int arrival_timestamp = 2147483647; //INT_MAX
    for (int i = 0; i < *table_cnt; i++)
    {
        if (page_table[i].is_valid &&
            page_table[i].arrival_timestamp < arrival_timestamp)
        {
            arrival_timestamp = page_table[i].arrival_timestamp;
            index = i;
        }
    }
    page_table[page_number].frame_number = page_table[index].frame_number;
    page_table[page_number].is_valid = 1;
    page_table[page_number].arrival_timestamp = current_timestamp;
    page_table[page_number].last_access_timestamp = current_timestamp;
    page_table[page_number].reference_count = 1;

    page_table[index] = (PTE){0, -1, -1, -1, -1};

    return page_table[page_number].frame_number;
}

int count_page_faults_fifo(struct PTE page_table[TABLEMAX],int table_cnt, int reference_string[REFERENCEMAX],
        int reference_cnt,int frame_pool[POOLMAX],int frame_cnt) {
    int faults = 0;
    int timestamp = 1000;
    for (int i = 0; i < reference_cnt; i++)
    {
        int page_number = reference_string[i];

        if (page_table[page_number].is_valid)
        {
            page_table[page_number].last_access_timestamp = timestamp + i;
            page_table[page_number].reference_count++;
            continue;
        }

        faults++;

        if (frame_cnt > 0)
        {
            page_table[page_number].is_valid = 1;
            page_table[page_number].frame_number = frame_pool[--frame_cnt];
            page_table[page_number].arrival_timestamp = timestamp + i;
            page_table[page_number].last_access_timestamp = timestamp + i;
            page_table[page_number].reference_count = 1;
            continue;
        }

        int index = -1;
        int arrival_timestamp = 2147483647; //INT_MAX;
        for (int j = 0; j < table_cnt; j++)
        {
            if (page_table[j].is_valid &&
                page_table[j].arrival_timestamp < arrival_timestamp)
            {
                arrival_timestamp = page_table[j].arrival_timestamp;
                index = j;
            }
        }
        page_table[page_number].frame_number = page_table[index].frame_number;
        page_table[page_number].is_valid = 1;
        page_table[page_number].arrival_timestamp = timestamp + i;
        page_table[page_number].last_access_timestamp = timestamp + i;
        page_table[page_number].reference_count = 1;

        page_table[index] = (PTE){0, -1, -1, -1, -1};
    }
    return faults;
}

int process_page_access_lru(struct PTE page_table[TABLEMAX],int *table_cnt, int
page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp){
    if (page_table[page_number].is_valid)
    {
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count++;
        return page_table[page_number].frame_number;
    }

    if (*frame_cnt > 0)
    {
        page_table[page_number].is_valid = 1;
        page_table[page_number].frame_number = frame_pool[--(*frame_cnt)];
        page_table[page_number].arrival_timestamp = current_timestamp;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        return page_table[page_number].frame_number;
    }

    int index = -1;
    int last_access_timestamp = 2147483647; //INT_MAX;
    for (int i = 0; i < *table_cnt; i++)
    {
        if (page_table[i].is_valid &&
            page_table[i].last_access_timestamp < last_access_timestamp)
        {
            last_access_timestamp = page_table[i].last_access_timestamp;
            index = i;
        }
    }
    page_table[page_number].frame_number = page_table[index].frame_number;
    page_table[page_number].is_valid = 1;
    page_table[page_number].arrival_timestamp = current_timestamp;
    page_table[page_number].last_access_timestamp = current_timestamp;
    page_table[page_number].reference_count = 1;

    page_table[index] = (PTE){0, -1, -1, -1, -1};

    return page_table[page_number].frame_number;
}

int count_page_faults_lru(struct PTE page_table[TABLEMAX],int table_cnt, int
reference_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int
                          frame_cnt){
    int faults = 0;
    int timestamp = 1000;
    for (int i = 0; i < reference_cnt; i++)
    {
        int page_number = reference_string[i];

        if (page_table[page_number].is_valid)
        {
            page_table[page_number].last_access_timestamp = timestamp + i;
            page_table[page_number].reference_count++;
            continue;
        }

        faults++;

        if (frame_cnt > 0)
        {
            page_table[page_number].is_valid = 1;
            page_table[page_number].frame_number = frame_pool[--frame_cnt];
            page_table[page_number].arrival_timestamp = timestamp + i;
            page_table[page_number].last_access_timestamp = timestamp + i;
            page_table[page_number].reference_count = 1;
            continue;
        }

        int index = -1;
        int last_access_timestamp = 2147483647; //INT_MAX;
        for (int i = 0; i < table_cnt; i++)
        {
            if (page_table[i].is_valid &&
                page_table[i].last_access_timestamp < last_access_timestamp)
            {
                last_access_timestamp = page_table[i].last_access_timestamp;
                index = i;
            }
        }
        page_table[page_number].frame_number = page_table[index].frame_number;
        page_table[page_number].is_valid = 1;
        page_table[page_number].arrival_timestamp = timestamp + i;
        page_table[page_number].last_access_timestamp = timestamp + i;
        page_table[page_number].reference_count = 1;

        page_table[index] = (PTE){0, -1, -1, -1, -1};
    }

    return faults;
}

int process_page_access_lfu(struct PTE page_table[TABLEMAX],int *table_cnt, int
page_number, int frame_pool[POOLMAX],int *frame_cnt, int current_timestamp){
    int temp_frame = 0;
    return temp_frame;
}

int count_page_faults_lfu(struct PTE page_table[TABLEMAX],int table_cnt, int
refrence_string[REFERENCEMAX],int reference_cnt,int frame_pool[POOLMAX],int
                          frame_cnt){
    int page_fault = 0;
    return page_fault;
}

/*
int main(int argc, char **argv)
{
    PTE page_table[TABLEMAX] = {
            {0, -1, -1, -1, -1},
            {0, -1, -1, -1, -1},
            {1, 10, 3, 3, 1},
            {0, -1, -1, -1, -1},
            {0, -1, -1, -1, -1},
            {1, 20, 2, 4, 2},
            {0, -1, -1, -1, -1},
            {1, 30, 1, 1, 1},
    };
    int table_cnt = 8;
    int page_number = 0;
    int frame_pool[POOLMAX] = {0};
    int frame_cnt = 0;
    int current_timestamp = 12;

    int frame_number = process_page_access_fifo(
            page_table,
            &table_cnt,
            page_number,
            frame_pool,
            &frame_cnt,
            current_timestamp);
    printf("frame_number: %d; VALID: %d\n", frame_number, frame_number == 30);

    PTE page_table1[TABLEMAX] = {
            {0, -1, -1, -1, -1},
            {0, -1, -1, -1, -1},
            {0, -1, -1, -1, -1},
            {0, -1, -1, -1, -1},
            {0, -1, -1, -1, -1},
            {0, -1, -1, -1, -1},
            {0, -1, -1, -1, -1},
            {0, -1, -1, -1, -1},
    };
    int table_cnt1 = 8;
    int reference_string[REFERENCEMAX] = {0, 3, 2, 6, 3, 4, 5, 2, 4, 5, 6};
    int reference_cnt = 11;
    int frame_pool1[POOLMAX] = {0, 1, 2};
    int frame_cnt1 = 3;
    int faults = count_page_faults_fifo(page_table1, table_cnt1, reference_string, reference_cnt, frame_pool1, frame_cnt1);
    printf("faults: %d; VALID: %d\n", faults, faults == 8);

    PTE page_table2[TABLEMAX] = {
            {0, -1, -1, -1, -1},
            {0, -1, -1, -1, -1},
            {0, -1, -1, -1, -1},
            {0, -1, -1, -1, -1},
            {0, -1, -1, -1, -1},
            {0, -1, -1, -1, -1},
            {0, -1, -1, -1, -1},
            {0, -1, -1, -1, -1},
    };
    int table_cnt2 = 8;
    int reference_string2[REFERENCEMAX] = {0, 3, 2, 6, 3, 4, 5, 2, 6, 4, 5};
    int reference_cnt2 = 11;
    int frame_pool2[POOLMAX] = {0, 1, 2};
    int frame_cnt2 = 3;
    int faults2 = count_page_faults_lfu(page_table2, table_cnt2, reference_string2, reference_cnt2, frame_pool2, frame_cnt2);
    printf("faults: %d; VALID: %d\n", faults2, faults2 == 10);

    return 0;
}
*/
