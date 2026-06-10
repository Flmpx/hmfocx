#ifndef HM_BASE_H
#define HM_BASE_H
#include <stdio.h>


typedef void (*hm_free)(void* ptr);

typedef int (*hm_cmp)(void* ptr1, void* ptr2);

typedef size_t (*hm_hash)(void* ptr);



#endif