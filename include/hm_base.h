/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */
#ifndef HM_BASE_H
#define HM_BASE_H

#define HM_FOCX_VERSION_MAJOR 0
#define HM_FOCX_VERSION_MINOR 3
#define HM_FOCX_VERSION_PATCH 0
#define HM_FOCX_VERSION "0.3.0"

#include <stdio.h>


typedef void (*hm_free)(void* ptr);

typedef int (*hm_cmp)(void* ptr1, void* ptr2);

typedef size_t (*hm_hash)(void* ptr);



#endif