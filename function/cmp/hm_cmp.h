/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */
#ifndef HM_CMP_H
#define HM_CMP_H
#include "../../include/hm_base.h"

extern int cmp_int_up(const void* ptr1, const void* ptr2);
extern int cmp_int_down(const void* ptr1, const void* ptr2);

extern int cmp_str_up(const void* ptr1, const void* ptr2);
extern int cmp_str_down(const void* ptr1, const void* ptr2);
#endif