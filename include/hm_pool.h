/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */
#ifndef HM_POOL_H
#define HM_POOL_H
#include "hm_base.h"

/**
 * A listnode of recording every page
 * This element is located in the head of every page
 */
typedef struct hm_pool_page_node {
    struct hm_pool_page_node* next;
} hm_pool_page_node;


/**
 * A listnode of recording every free block
 */
typedef struct hm_pool_block_node {
    struct hm_pool_block_node* next;
} hm_pool_block_node;


/**
 * Memory pool with only fixed block size
 */
typedef struct hm_pool {
    hm_pool_page_node* head_page;
    hm_pool_block_node* head_block;
    size_t block_size;
    size_t blocks_per_page;
} hm_pool;


/**
 * Basic function
 */

extern void hm_pool_init(hm_pool* pool, size_t block_size, size_t blocks_per_page);
extern void* hm_pool_block_allocate(hm_pool* pool);
extern void hm_pool_block_free(hm_pool* pool, void* block);
extern void hm_pool_free(hm_pool* pool);

/**
 * Simple function
 */
extern size_t hm_pool_get_pages(hm_pool* pool);
extern size_t hm_pool_get_bytes_per_page(hm_pool* pool);

#endif