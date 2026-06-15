#include <stdbool.h>
#include <stdio.h>
#include "hm_test.h"
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[31m"
#define COLOR_GREEN   "\033[32m"
#define COLOR_YELLOW  "\033[33m"

void check_res(bool res, const char* info, int* fail_cnt) {
    if (!res) {
        printf(COLOR_RED "[       FAIL]:%s " COLOR_RESET "\n", info);
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

void print_run_time(const char* info, double time, size_t size) {
    printf(COLOR_YELLOW "(SIZE: %16zu ) | (%s COST TIME):" COLOR_RESET "[%lfs]" COLOR_YELLOW " | (OPERATION PER SEC):" COLOR_RESET "[%zu]\n", size, info, time, (size_t)(size / time));
}


