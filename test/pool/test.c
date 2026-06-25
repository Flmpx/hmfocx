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

#define HM_TEST_COUNTER \
    all_failure_num += fail_cnt;


// This variable can record the all number of failure
int all_failure_num = 0;



void test_pool_init() {

    print_run("POOL | FUNC | INIT | BLOCK SIZE: sizeof(int) BLOCKS: 1024");
    hm_pool pool;
    int fail_cnt = 0;
    int blocks_per_page = 1024;
    hm_pool_init(&pool, sizeof(int), blocks_per_page);

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

    check_res(val != NULL, "the allocated block shouldn't be NULL when allocate one block", &fail_cnt);
    // because this test exclude free test and `hm_pool_free` should destroy all the memory, so, cancel the free the block

    int nums = blocks_per_page;
    int* pointers[nums];
    int flag[nums];
    int need_check_num = 0;
    // allocate more than an page's blocks, **Tip: val still existed**
    for (int i = 0; i < nums; i++) {
        int* v = hm_pool_block_allocate(&pool);
        if (v == NULL) {
            break;
        }
        *v = flag[i] = i * 10;
        pointers[i] = v;
        need_check_num++;
    }

    // verify the pointer
    int fail_dup = 0;
    for (int i = 0; i < need_check_num; i++) {
        for (int j = i + 1; j < need_check_num; j++) {
            if (pointers[i] == pointers[j]) {
                fail_dup++;
            }
        }
    }
    check_res(fail_dup == 0, "the pointer should be different", &fail_cnt);

    // verify the pointer got by `hm_pool_block_allocate`
    int fail_NULL = 0;
    int fail_diff = 0;
    for (int i = 0; i < need_check_num; i++) {
        if (pointers[i] == NULL) {
            fail_NULL++;
        } else if (*(pointers[i]) != flag[i]) {
            fail_diff++;
        }
    }
    check_res(fail_NULL == 0, "the pointer shoudn't have NULL", &fail_cnt);
    check_res(fail_diff == 0, "stored value should match the  value in `flag` array", &fail_cnt);


    // verify the pages 

    int pages = 0;
    hm_pool_page_node* node = pool.head_page;
    while (node) {
        node = node->next;
        pages++;
    }
    check_res(pages == (need_check_num == nums ? 2 : 1), "the number of pages should be 2 when get over the number of blocks per page", &fail_cnt);


    hm_pool_free(&pool);
    print_end("POOL | FUNC | ALLOCATE | BLOCK SIZE: sizeof(int) BLOCKS: 2048", fail_cnt);
    HM_TEST_COUNTER

}




void function_test() {
    test_pool_init();                                               printf("\n");

    test_pool_allocate();                                           printf("\n");
}



void boundary_test() {

}

void stress_test() {

}




int main()
{
    function_test();

    boundary_test();

    stress_test();


    return all_failure_num;
}