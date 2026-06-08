#ifndef HM_CMP_H
#define HM_CMP_H
#include "../../base.h"

extern hm_cmp_res cmp_int_up(void* ptr1, void* ptr2);
extern hm_cmp_res cmp_int_down(void* ptr1, void* ptr2);

extern hm_cmp_res cmp_str_up(void* ptr1, void* ptr2);
extern hm_cmp_res cmp_str_down(void* ptr1, void* ptr2);
#endif