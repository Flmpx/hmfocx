/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */
#ifndef HM_TEST_TOOLS_H
#define HM_TEST_TOOLS_H
#include <time.h>
#include <stdbool.h>


/**
 * Print tools
 * 
 */

extern void check_res(bool res, const char* info, int* fail_cnt, int tag);
extern void print_run(const char* info);
extern void print_end(const char* info, int fail_cnt);
extern void print_run_time(const char* info, clock_t start, clock_t end, size_t scale, size_t oper_cnt);
extern void print_speed_vs(const char* info_a, clock_t start_a, clock_t end_a,
                           const char* info_b, clock_t start_b, clock_t end_b,
                           size_t scale, size_t oper_cnt);




/**
 * Cmp tools
 * 
 */

extern int cmp_int_up(const void* ptr1, const void* ptr2);
extern int cmp_int_down(const void* ptr1, const void* ptr2);

extern int cmp_str_up(const void* ptr1, const void* ptr2);
extern int cmp_str_down(const void* ptr1, const void* ptr2);


/**
 * Hash tools
 */

extern size_t hash_int_1(const void* ptr);

extern size_t hash_str_1(const void* ptr);

#endif