/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */

#include "../include/hm_pool.h"
#include <stdlib.h>
#include <stdint.h>

/**
 * Align the memory to the variable `align`
 */
static size_t align_up(size_t n, size_t align) {
    return (n + (align - 1) & ~(align - 1));
}



/**
 * Initialize the memory pool 
 * @note - `hm_pool_block_allocate` will return `NULL` if `blocks_per_page` is `zero` or `block_size` is `zero`
 */
void hm_pool_init(hm_pool* pool, size_t block_size, size_t blocks_per_page) {
    pool->head_page = NULL;
    pool->head_block = NULL;

    pool->blocks_per_page = blocks_per_page;

    // let pool.block_size == 0 when pass-in block_size == 0
    // This action can let allocator handle this sitution easily
    if (block_size == 0) {
        pool->block_size = 0;
        return;
    }

    block_size = block_size > sizeof(hm_pool_block_node) ? block_size : sizeof(hm_pool_block_node);
    pool->block_size = align_up(block_size, sizeof(void*));

}


/**
 * Get pointer of to block
 * @note - It will return `NULL` when allocation fails
 * @note - `hm_pool_block_allocate` will return `NULL` if `blocks_per_page` is `zero` or `block_size` is `zero`
 */
void* hm_pool_block_allocate(hm_pool* pool) {

    // refactor: allocator reurn NULL when `block_size` or `blocks_per_page` == 0
    if (pool->block_size == 0 || pool->blocks_per_page == 0) {
        return NULL;

    }
    if (pool->head_block) {
        hm_pool_block_node* node = pool->head_block;
        pool->head_block = pool->head_block->next;
        return node;
    }

    // product check
    if (pool->blocks_per_page > SIZE_MAX / pool->block_size) {
        return NULL;
    }
    size_t product = pool->block_size * pool->blocks_per_page;

    // add check
    if (product > SIZE_MAX - sizeof(hm_pool_page_node)) {
        return NULL;
    }
    size_t total = product + sizeof(hm_pool_page_node);

    hm_pool_page_node* new_page = (hm_pool_page_node*)malloc(total);

    if (new_page == NULL) {
        return NULL;
    }

    new_page->next = pool->head_page;
    pool->head_page = new_page;

    for (size_t i = 0; i < pool->blocks_per_page * pool->block_size; i += pool->block_size) {
        hm_pool_block_node* node = (hm_pool_block_node*)((char*)(new_page + 1) + i);
        node->next = pool->head_block;
        pool->head_block = node;
    }

    hm_pool_block_node* node = pool->head_block;

    /**
     * the next action will generate error if the `blocks_per_page` is zero
     * So, let it return NULL for user
     */
    if (pool->head_block) {
        pool->head_block = pool->head_block->next;
    }

    return node;
}


/**
 * Free a block
 * @note - The pointer of block must match the memory pool
 */
void hm_pool_block_free(hm_pool* pool, void* block) {
    if (block == NULL) return;
    hm_pool_block_node* node = (hm_pool_block_node*)block;
    node->next = pool->head_block;
    pool->head_block = node;
}


/**
 * Free all content of the memory pool
 * @note Do not use blocks from a freed memory pool
 */
void hm_pool_free(hm_pool* pool) {
    hm_pool_page_node* node = pool->head_page;
    while (node) {
        hm_pool_page_node* cur = node;
        node = node->next;
        free(cur);
    }
    hm_pool_init(pool, pool->block_size, pool->blocks_per_page);
}


/**
 * Get the number of pages in the given memory pool
 * 
 */
size_t hm_pool_get_pages(hm_pool* pool) {
    size_t cnt = 0;
    hm_pool_page_node* cur = pool->head_page;
    while (cur) {
        cur = cur->next;
        cnt++;
    }
    return cnt;
}


/**
 * Get the theoretical number of bytes per page in the memory pool
 * 
 */
size_t hm_pool_get_bytes_per_page(hm_pool* pool) {
    return sizeof(hm_pool_page_node) + pool->block_size * pool->blocks_per_page;
}