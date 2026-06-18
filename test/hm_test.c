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


