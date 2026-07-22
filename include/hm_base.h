/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */

/*********************************************************************
 * Latest API Description see https://github.com/Flmpx/hmfocx/blob/main/README.md
 * 
 ********************************************************************/

#ifndef HM_BASE_H
#define HM_BASE_H

#define HM_FOCX_VERSION_MAJOR 0
#define HM_FOCX_VERSION_MINOR 9
#define HM_FOCX_VERSION_PATCH 0
#define HM_FOCX_VERSION "0.9.0"

#include <stdio.h>


typedef void (*hm_free)(void* ptr);

typedef int (*hm_cmp)(const void* ptr1, const void* ptr2);

typedef size_t (*hm_hash)(const void* ptr);



#endif