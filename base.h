#ifndef BASE_H
#define BASE_H
#include <stdio.h>

/// @brief result of compare
typedef enum hm_cmp_res {
    hm_less = -1,
    hm_same,
    hm_more
} hm_cmp_res;



/// @brief return information of functions
typedef enum hm_info {
    hm_error = 2,
    hm_warning,
    hm_none,
    hm_success
} hm_info;



typedef void (*hm_free)(void* ptr);

typedef hm_cmp_res (*hm_cmp)(void* ptr1, void* ptr2);

typedef size_t (*hm_hash)(void* ptr);



#endif