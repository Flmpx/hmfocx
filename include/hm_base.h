/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */
#ifndef HM_BASE_H
#define HM_BASE_H

#define HM_FOCX_VERSION_MAJOR 0
#define HM_FOCX_VERSION_MINOR 4
#define HM_FOCX_VERSION_PATCH 0
#define HM_FOCX_VERSION "0.4.0"

#include <stdio.h>


typedef void (*hm_free)(void* ptr);

typedef int (*hm_cmp)(const void* ptr1, const void* ptr2);

typedef size_t (*hm_hash)(const void* ptr);



#endif