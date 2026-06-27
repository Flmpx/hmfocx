/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */
#include "../../include/hm_pool.h"
#include <stdbool.h>
#include "../hm_test.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <stdint.h>

#define HM_TEST_COUNTER \
    all_failure_num += fail_cnt;


// This variable can record the all number of failure
int all_failure_num = 0;


void test_pool_integrity(hm_pool* pool, size_t used_block_num, int* fail_cnt) {
    size_t pages = 0;
    size_t free_block_num = 0;
    // get pages

    hm_pool_page_node* page_node = pool->head_page;
    while (page_node) {
        page_node = page_node->next;
        pages++;
    }
    hm_pool_block_node* block_node = pool->head_block;
    while (block_node) {
        block_node = block_node->next;
        free_block_num++;
    }
    check_res((free_block_num + used_block_num) == pages * pool->blocks_per_page , "TEST OF INTEGRITY: used + freed != pages * blocks_per_page ", fail_cnt);


} 


enum location_block {
    other_block_in_pool,
    freed_block_in_pool,
    used_block_in_pool,
};

/**
 * Judge the location of block that pass in
 * @return 
 */
int judge_memory_location(hm_pool* pool, void* block) {

    // Is freed block ?

    hm_pool_block_node* b_node = pool->head_block;
    while (b_node) {
        if (b_node == block) {
            return freed_block_in_pool;
        }
        b_node = b_node->next;
    }


    // Is used block ?

    hm_pool_page_node* p_node = pool->head_page;

    while (p_node) {
        void* min = p_node + 1;
        void* max = (char*)(p_node + 1) + pool->blocks_per_page * pool->block_size;
        if (block <= max && block >= min) {
            return used_block_in_pool;
        }
        p_node = p_node->next;
    }

    // Oh ! others
    return other_block_in_pool;

}


void test_pool_init() {

    print_run("POOL | FUNC | INIT | BLOCK SIZE: sizeof(int) BLOCKS: 1024");
    hm_pool pool;
    int fail_cnt = 0;
    int blocks_per_page = 1024;
    hm_pool_init(&pool, sizeof(int), blocks_per_page);
    test_pool_integrity(&pool, 0, &fail_cnt);
    check_res(pool.block_size == sizeof(void*), "the block size should is sizeof(void*)", &fail_cnt);
    check_res(pool.blocks_per_page == blocks_per_page, "the number of blocks per page should is 1024", &fail_cnt);
    check_res(pool.head_block == NULL, "head_block should be `NULL` after init", &fail_cnt);
    check_res(pool.head_page == NULL, "head_block should be `NULL` after init", &fail_cnt);

    print_end("POOL | FUNC | INIT | BLOCK SIZE: sizeof(int) BLOCKS: 1024", fail_cnt);
    
    HM_TEST_COUNTER

}

void test_pool_allocate() {

    hm_pool pool;
    int blocks_per_page = 2048;
    hm_pool_init(&pool, sizeof(int), blocks_per_page);
    int fail_cnt = 0;

    print_run("POOL | FUNC | ALLOCATE | BLOCK SIZE: sizeof(int) BLOCKS: 2048");

    int* val = hm_pool_block_allocate(&pool);
    test_pool_integrity(&pool, 1, &fail_cnt);
    check_res(judge_memory_location(&pool, val) == used_block_in_pool, "the val should be the used block in memory pool", &fail_cnt);

    check_res(val != NULL, "the allocated block shouldn't be NULL when allocate one block", &fail_cnt);
    // because this test exclude free test and `hm_pool_free` should destroy all the memory, so, cancel the free the block

    int nums = blocks_per_page;
    int* pointers[nums];
    int flag[nums];
    // allocate more than an page's blocks, **Tip: val still existed**
    for (int i = 0; i < nums; i++) {
        int* v = hm_pool_block_allocate(&pool);
        *v = flag[i] = i * 10;
        pointers[i] = v;
    }
    test_pool_integrity(&pool, nums + 1, &fail_cnt);

    // verify the number of same pointer
    int fail_same = 0;
    for (int i = 0; i < nums; i++) {
        for (int j = i + 1; j < nums; j++) {
            if (pointers[i] == pointers[j]) {
                fail_same++;
            }
        }
    }
    check_res(fail_same == 0, "the pointer should be different", &fail_cnt);

    // verify the pointer got by `hm_pool_block_allocate`
    int fail_NULL = 0;
    int fail_diff = 0;
    int fail_wrong_location = 0;
    for (int i = 0; i < nums; i++) {
        if (pointers[i] == NULL) {
            fail_NULL++;
        } else if (*(pointers[i]) != flag[i]) {
            fail_diff++;
        }
        if (judge_memory_location(&pool, pointers[i]) != used_block_in_pool) {
            fail_wrong_location++;
        }
    }
    check_res(fail_NULL == 0, "the pointer shoudn't have NULL", &fail_cnt);
    check_res(fail_diff == 0, "stored value should match the  value in `flag` array", &fail_cnt);

    check_res(fail_wrong_location == 0, "the val should be the used block in memory pool", &fail_cnt);

    // verify the pages 

    int pages = 0;
    hm_pool_page_node* node = pool.head_page;
    while (node) {
        node = node->next;
        pages++;
    }
    check_res(pages == 2, "the number of pages should be 2 when get over the number of blocks per page", &fail_cnt);


    hm_pool_free(&pool);
    print_end("POOL | FUNC | ALLOCATE | BLOCK SIZE: sizeof(int) BLOCKS: 2048", fail_cnt);
    HM_TEST_COUNTER

}


void test_pool_get_pages() {
    hm_pool pool;
    int blocks_per_page = 2048;
    hm_pool_init(&pool, sizeof(int), blocks_per_page);
    int fail_cnt = 0;
    
    int allocate_blocks = 10000;
    int expected_pages = (allocate_blocks + blocks_per_page - 1) / blocks_per_page;
    print_run("POOL | FUNC | GET PAGES | BLOCK SIZE: sizeof(int) BLOCKS: 2048");
    
    // allocate
    for (int i = 0; i < allocate_blocks; i++) {
        hm_pool_block_allocate(&pool);  // the work of free assgin to `hm_pool_free`
    }

    int real_pages = 0;
    hm_pool_page_node* node = pool.head_page;
    while (node) {
        node = node->next;
        real_pages++;
    }
    int get_pages = hm_pool_get_pages(&pool);
    test_pool_integrity(&pool, allocate_blocks, &fail_cnt);

    check_res(get_pages == real_pages, "the number of page got by get_pages function should be the same the real pages", &fail_cnt);
    
    hm_pool_free(&pool);
    print_end("POOL | FUNC | GET PAGES | BLOCK SIZE: sizeof(int) BLOCKS: 2048", fail_cnt);
    HM_TEST_COUNTER
    
}


void test_pool_get_bytes() {
    hm_pool pool;
    int blocks_per_page = 2048;
    hm_pool_init(&pool, sizeof(int), blocks_per_page);
    int fail_cnt = 0;
    
    int allocate_blocks = 10000;
    print_run("POOL | FUNC | GET BYTES | BLOCK SIZE: sizeof(int) BLOCKS: 2048");
    size_t expected_bytes_per_page = sizeof(hm_pool_page_node) + pool.blocks_per_page * pool.block_size;
    
    // start 

    check_res(expected_bytes_per_page == hm_pool_get_bytes_per_page(&pool), "the bytes of every page got by `get_bytes_func` is wrong after init pool", &fail_cnt);
    
    // after allocate
    for (int i = 0; i < allocate_blocks; i++) {
        hm_pool_block_allocate(&pool);  // the work of free assgin to `hm_pool_free`
    }
    
    check_res(expected_bytes_per_page == hm_pool_get_bytes_per_page(&pool), "the bytes of every page got by `get_bytes_func` is wrong after allocate some memory from pool", &fail_cnt);


    hm_pool_free(&pool);
    print_end("POOL | FUNC | GET BYTES | BLOCK SIZE: sizeof(int) BLOCKS: 2048", fail_cnt);
    HM_TEST_COUNTER

}


void test_pool_block_free() {
    hm_pool pool;
    int blocks_per_page = 2048;
    hm_pool_init(&pool, sizeof(int), blocks_per_page);
    int fail_cnt = 0;

    print_run("POOL | FUNC | FREE BLOCK | BLOCK SIZE: sizeof(int) BLOCKS: 2048");
    
    // the val should be same as the new_val because the pool is use the method of `head_insert list` 
    int* val = hm_pool_block_allocate(&pool);
    hm_pool_block_free(&pool, val);
    
    // 0 used
    test_pool_integrity(&pool, 0, &fail_cnt);
    int* new_val = hm_pool_block_allocate(&pool);
    check_res(val == new_val, "val should be same as the new_val after a allocate and a free", &fail_cnt);

    hm_pool_block_free(&pool, new_val);

    int nums = 100;
    int* pointers[nums];

    for (int i = 0; i < nums; i++) {
        pointers[i] = hm_pool_block_allocate(&pool);
    }

    // free half
    for (int i = 0; i < nums / 2; i++) {
        hm_pool_block_free(&pool, pointers[i]);
    }
    test_pool_integrity(&pool, nums - nums / 2, &fail_cnt);

    // verify

    int fail_freed = 0;
    for (int i = 0; i < nums / 2; i++) {
        if (judge_memory_location(&pool, pointers[i]) != freed_block_in_pool) {
            fail_freed++;
        }
    }
    check_res(fail_freed == 0, "the freed block isn't in freed list of memory pool", &fail_cnt);
    
    int fail_used = 0;
    
    for (int i = nums / 2; i < nums; i++) {
        if (judge_memory_location(&pool, pointers[i]) != used_block_in_pool) {
            fail_used++;
        }
    }

    check_res(fail_used == 0, "the used block isn't in used area of memory pool", &fail_cnt);

    
    // free all
    for (int i = nums / 2; i < nums; i++) {
        hm_pool_block_free(&pool, pointers[i]);
    }
    test_pool_integrity(&pool, 0, &fail_cnt);

    // verify

    fail_freed = 0;
    for (int i = 0; i < nums; i++) {
        if (judge_memory_location(&pool, pointers[i]) != freed_block_in_pool) {
            fail_freed++;
        }
    }
    check_res(fail_freed == 0, "the freed block isn't in freed list of memory pool", &fail_cnt);

    hm_pool_free(&pool);
    
    print_end("POOL | FUNC | FREE BLOCK | BLOCK SIZE: sizeof(int) BLOCKS: 2048", fail_cnt);
    HM_TEST_COUNTER
}


void test_pool_free() {
    hm_pool pool;
    int blocks_per_page = 2048;
    hm_pool_init(&pool, sizeof(int), blocks_per_page);
    int fail_cnt = 0;
    
    print_run("POOL | FUNC | FREE CONTAILER | BLOCK SIZE: sizeof(int) BLOCKS: 2048");
    
    int nums = 1000;
    for (int i = 0; i < nums; i++) {
        hm_pool_block_allocate(&pool); // the work of free assgin to `hm_pool_free`
    }


    //  **Use valgrind or other tool that can check `leak-memory` to check if there existed memory leak**
    hm_pool_free(&pool);
    
    print_end("POOL | FUNC | FREE CONTAILER | BLOCK SIZE: sizeof(int) BLOCKS: 2048", fail_cnt);
    HM_TEST_COUNTER
    
}

void test_minmax_pageblock_oper() {
    hm_pool pool;
    int fail_cnt = 0;
    print_run("POOL | BOUNDARY | MIN_MAX__PAGE_BLOCK");
    size_t blocks_per_page;
    size_t block_size;

    // blocks_per_page = 0; block_size = sizeof(int)
    blocks_per_page = 0;
    block_size = sizeof(int);

    hm_pool_init(&pool, block_size, blocks_per_page);

    void* v = hm_pool_block_allocate(&pool);
    check_res(v == NULL, "the v should be NULL when `block_per_page == 0`", &fail_cnt);
    test_pool_integrity(&pool, 0, &fail_cnt);
    hm_pool_free(&pool);

    // block_size = 0; blocks_per_page = 1024;
    block_size = 0;
    blocks_per_page = 1024;
    hm_pool_init(&pool, block_size, blocks_per_page);
    v = hm_pool_block_allocate(&pool);

    check_res(v == NULL, "the v shouldn be NULL when `block_size == 0`", &fail_cnt);
    hm_pool_free(&pool);


    // block_size = SIZE_MAX / 2, blocks_per_page = 1;
    blocks_per_page = 1;
    block_size = SIZE_MAX / 2;
    hm_pool_init(&pool, block_size, blocks_per_page);

    v = hm_pool_block_allocate(&pool);
    check_res(v == NULL, "the v should be NULL when `block_size` is too big", &fail_cnt);
    hm_pool_free(&pool);
    
    // block_size = sizeof(int), blocks_per_page = SIZE_MAX / 2;

    blocks_per_page = SIZE_MAX / 2;
    block_size = sizeof(int);
    hm_pool_init(&pool, block_size, blocks_per_page);

    v = hm_pool_block_allocate(&pool);
    check_res(v == NULL, "the v should be NULL when `blocks_per_page` is too big", &fail_cnt);
    hm_pool_free(&pool);


    print_end("POOL | BOUNDARY | MIN_MAX__PAGE_BLOCK", fail_cnt);

}


void test_free_NULL() {
    // free a nullptr
    hm_pool pool;
    int fail_cnt = 0;
    print_run("POOL | BOUNDARY | FREE NULLPTR | BLOCK SIZE: sizeof(int) BLOCKS: 2048");
    hm_pool_init(&pool, sizeof(int), 2048);

    void* v = NULL;
    hm_pool_block_free(&pool, v);

    print_end("POOL | BOUNDARY | FREE NULLPTR | BLOCK SIZE: sizeof(int) BLOCKS: 2048", fail_cnt);
}


void test_pool_allocate_stress() {
    hm_pool pool;
    int fail_cnt = 0;
    int nums[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000};
    int cnt = sizeof(nums) / sizeof(int);
    print_run("POOL | STRESS | ALLOCATE | *BLOCK SIZE: sizeof(int) BLOCKS: 2048* VS *MALLOC*");
    hm_pool_init(&pool, sizeof(int), 2048);

    for (int i = 0; i < cnt; i++) {

        int** pointers = (int**)malloc(nums[i] * sizeof(int*));

        clock_t start_a = clock();
        for (int j = 0; j < nums[i]; j++) {
            pointers[j] = hm_pool_block_allocate(&pool);
            *pointers[j] = 10 + i;
        }
        clock_t end_a = clock();
        test_pool_integrity(&pool, nums[i], &fail_cnt);

        hm_pool_free(&pool);

        clock_t start_b = clock();
        for (int j = 0; j < nums[i]; j++) {
            pointers[j] = (int*)malloc(sizeof(int));
            *pointers[j] = 10 + i;
        }
        clock_t end_b = clock();

        // free
        for (int j = 0; j < nums[i]; j++) {
            free(pointers[j]);
        }
        free(pointers);
        print_speed_vs("Pool", start_a, end_a, "Malloc", start_b, end_b, nums[i], nums[i]);
    }

    print_end("POOL | STRESS | ALLOCATE | *BLOCK SIZE: sizeof(int) BLOCKS: 2048* VS *MALLOC*", fail_cnt);


}

void function_test() {
    test_pool_init();                                               printf("\n");

    test_pool_allocate();                                           printf("\n");

    test_pool_get_pages();                                          printf("\n");

    test_pool_get_bytes();                                          printf("\n");

    test_pool_block_free();                                         printf("\n");

    test_pool_free();                                               printf("\n");
}



void boundary_test() {
    test_minmax_pageblock_oper();                                   printf("\n");

    test_free_NULL();                                               printf("\n");

}

void stress_test() {
    test_pool_allocate_stress();                                    printf("\n");
    
}




int main()
{
    function_test();

    boundary_test();

    stress_test();


    return all_failure_num;
}