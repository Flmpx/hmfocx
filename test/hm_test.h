#ifndef HM_TEST_H
#define HM_TEST_H

extern void check_res(bool res, const char* info, int* fail_cnt);
extern void print_run(const char* info);
extern void print_end(const char* info, int fail_cnt);

#endif