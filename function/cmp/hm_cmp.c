/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */
#include "hm_cmp.h"
#include <string.h>

int cmp_int_up(void* ptr1, void* ptr2) {
    int a = *(int*)ptr1, b = *(int*)ptr2;
    return (a > b) - (a < b);
}

int cmp_int_down(void* ptr1, void* ptr2) {
    int a = *(int*)ptr1, b = *(int*)ptr2;
    return (a < b) - (a > b);
}

int cmp_str_up(void* ptr1, void* ptr2) {
    return strcmp((const char*)ptr1, (const char*)ptr2);
    
}

int cmp_str_down(void* ptr1, void* ptr2) {
    return -strcmp((const char*)ptr2, (const char*)ptr1);
    
}

