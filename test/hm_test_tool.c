/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */
#include <stdbool.h>
#include <stdio.h>
#include "hm_test_tool.h"
#include <string.h>
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"



/**
 * Print tools
 * 
 */

void check_res(bool res, const char* info, int* fail_cnt, int tag) {
    if (!res) {
        printf(COLOR_RED "[       FAIL]" COLOR_YELLOW "[tag: %d]" COLOR_RED ":%s " COLOR_RESET "\n", tag, info);
        (*fail_cnt)++;
    }
}

void print_run(const char* info) {
    printf(COLOR_YELLOW "[RUN        ]:%s" COLOR_RESET "\n", info);
}

void print_end(const char* info, int fail_cnt) {
    if (fail_cnt) {
        printf(COLOR_RED "[       FAIL]:%s: " COLOR_YELLOW "%d failed" COLOR_RESET "\n", info, fail_cnt);
    } else {
        printf(COLOR_GREEN "[OK         ]:%s" COLOR_RESET "\n", info);
    }
}

void print_run_time(const char* info, clock_t start, clock_t end, size_t scale, size_t oper_cnt) {
    printf(COLOR_YELLOW "| (SCALE):" COLOR_RESET "[%zu]\n", scale);
    printf(COLOR_YELLOW "| (OPERATION CNT):" COLOR_RESET "[%zu]\n", oper_cnt);
    printf(COLOR_YELLOW "| (%s COST TIME):" COLOR_RESET "[%lfs]\n", info, (double)(end - start) / CLOCKS_PER_SEC);

    printf(COLOR_YELLOW "| (OPERATION PER SEC):" COLOR_RESET);
    if (end <= start) {
        printf("[OPERATION CNT TOO SMALL]\n\n");
    } else {
        printf("[%zu]\n\n", (size_t)(oper_cnt * CLOCKS_PER_SEC/ (end - start)));
    }
}


void print_speed_vs(const char* info_a, clock_t start_a, clock_t end_a,
                    const char* info_b, clock_t start_b, clock_t end_b,
                    size_t scale, size_t oper_cnt) {
    printf(COLOR_YELLOW "| (SCALE):" COLOR_RESET "[%zu]\n", scale);
    printf(COLOR_YELLOW "| (OPERATION CNT):" COLOR_RESET "[%zu]\n", oper_cnt);
    // compare cost time
    printf(COLOR_YELLOW "| (COST TIME):");
    printf(COLOR_YELLOW "%s" COLOR_RESET ": [%lfs] | ", info_a, (double)(end_a - start_a) / CLOCKS_PER_SEC); 
    printf(COLOR_YELLOW "%s" COLOR_RESET ": [%lfs]\n", info_b, (double)(end_b - start_b) / CLOCKS_PER_SEC); 


    printf(COLOR_YELLOW "| (OPERATION PER SEC):" COLOR_RESET);
    if (end_a <= start_a) {
        printf("[OPERATION CNT TOO SMALL]\n");
    } else {
        printf(COLOR_YELLOW "%s" COLOR_RESET ": [%zu] | ", info_a, (size_t)(oper_cnt * CLOCKS_PER_SEC/ (end_a - start_a)));
    }


    if (end_b <= start_b) {
        printf("[OPERATION CNT TOO SMALL]\n");
    } else {
        printf(COLOR_YELLOW "%s" COLOR_RESET ": [%zu]\n", info_b, (size_t)(oper_cnt * CLOCKS_PER_SEC/ (end_b - start_b)));
    }
    
    
    printf(COLOR_YELLOW "| (%s / %s * 100%%):" COLOR_RESET, info_a, info_b);
    if (end_b <= start_b) {
        printf("[OPERATION CNT TOO SMALL]\n\n");
    } else {
        printf("[%lf%%]\n\n", (double)(end_b - start_b) / (end_a - start_a) * 100);
    }
    

}




/**
 * Cmp tools
 * 
 */

int cmp_int_up(const void* ptr1, const void* ptr2) {
    int a = *(int*)ptr1, b = *(int*)ptr2;
    return (a > b) - (a < b);
}

int cmp_int_down(const void* ptr1, const void* ptr2) {
    int a = *(int*)ptr1, b = *(int*)ptr2;
    return (a < b) - (a > b);
}

int cmp_str_up(const void* ptr1, const void* ptr2) {
    return strcmp((const char*)ptr1, (const char*)ptr2);
    
}

int cmp_str_down(const void* ptr1, const void* ptr2) {
    return -strcmp((const char*)ptr2, (const char*)ptr1);
    
}


/**
 * Hash tools
 */

size_t hash_int_1(const void* ptr) {
    unsigned int key = (unsigned int)(*(int*)ptr);
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = (key >> 16) ^ key;
    return (size_t)key;
}

static const size_t mod = 1000000009;
static const size_t base = 131;


size_t hash_str_1(const void* ptr) {
    char* key = (char*)ptr;
    size_t hash_res = 0;
    for (size_t i = 0; key[i]; i++) {
        hash_res = (hash_res * base + key[i] + 199) % mod;
    }
    return hash_res;
}