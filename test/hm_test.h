#ifndef HM_TEST_H
#define HM_TEST_H
#include <time.h>

extern void check_res(bool res, const char* info, int* fail_cnt);
extern void print_run(const char* info);
extern void print_end(const char* info, int fail_cnt);
extern void print_run_time(const char* info, clock_t start, clock_t end, size_t scale, size_t oper_cnt);
#endif