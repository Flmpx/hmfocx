#include "hm_cmp.h"
#include <string.h>

hm_cmp_res cmp_int_up(void* ptr1, void* ptr2) {
    int a = *(int*)ptr1, b = *(int*)ptr2;
    if (a > b) {
        return hm_more;
    } else if (a < b) {
        return hm_less;
    } else {
        return hm_same;
    }
}

hm_cmp_res cmp_int_down(void* ptr1, void* ptr2) {
    int a = *(int*)ptr1, b = *(int*)ptr2;
    if (a > b) {
        return hm_less;
    } else if (a < b) {
        return hm_more;
    } else {
        return hm_same;
    }
}

hm_cmp_res cmp_str_up(void* ptr1, void* ptr2) {
    int res = strcmp((const char*)ptr1, (const char*)ptr2);
    if (res < 0) return hm_less;
    else if (res > 0) return hm_more;
    else return hm_same;
}

hm_cmp_res cmp_str_down(void* ptr1, void* ptr2) {
    int res = strcmp((const char*)ptr2, (const char*)ptr1);
    if (res < 0) return hm_less;
    else if (res > 0) return hm_more;
    else return hm_same;
}

