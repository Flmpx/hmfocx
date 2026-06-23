/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */
#include "../../include/hm_map.h"
#include "../../function/hash/hm_hash.h"
#include "../../function/cmp/hm_cmp.h"
#include "../hm_test.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#ifdef HM_TEST_COUNTER
    all_failure_num += fail_cnt;
#endif

// This variable can record the all number of failure
int all_failure_num = 0;

void test_map_integrity(hm_map* map, int* fail_cnt) {

    // size [exist | none | del]
    // Tip: Assume the map.len is true because can't verify the correctness of map.len
    int len = map->len;

    int existed = 0, none = 0, del = 0;
    int others = 0;
    for (int i = 0; i < len; i++) {
        switch (map->buckets_status[i]) {
            case hm_exist_in_map: existed++; break;
            case hm_del_in_map: del++; break;
            case hm_none_in_map: none++; break;
            default: others++; break;
        }
    }
    
    check_res(others == 0, "TEST OF INTEGRITY: there are unkown tag of entry_status in map", fail_cnt);
    check_res(existed == map->size, "TEST OF INTEGRITY: the real size of existed entry in map is wrong", fail_cnt);
}

void test_map_init() {
    hm_map map_1;
    int fail_cnt = 0;
    print_run("MAP | FUNC | INIT");
    hm_map_init(&map_1, hash_int_1, cmp_int_up, free, free);

    check_res(map_1.buckets == NULL, "the buckets isn't NULL", &fail_cnt);
    check_res(map_1.buckets_status == NULL, "the buckets_status isn't NULL", &fail_cnt);
    check_res(map_1.cmp == cmp_int_up, "pass `cmp_int_up` but map.cmp isn't `cmp_int_up`", &fail_cnt);
    check_res(map_1.hash == hash_int_1, "pass `hash_int_1` but map.hash isn't `hash_int_1`", &fail_cnt);
    check_res(map_1.free_key == free, "pass `free` for map.free_key but map.free_key isn't `free`", &fail_cnt);   
    check_res(map_1.free_val == free, "pass `free` for map.free_val but map.free_val isn't `free`", &fail_cnt);   
    check_res(map_1.len == 0, "map.len isn't 0", &fail_cnt);
    check_res(map_1.size == 0, "map.size isn't 0", &fail_cnt);
    test_map_integrity(&map_1, &fail_cnt);
    
    hm_map map_2;
    hm_map_init(&map_2, hash_int_1, cmp_int_up, NULL, NULL);
    
    check_res(map_2.free_key == NULL, "pass `NULL` but map.free_key isn't `NULL`", &fail_cnt);
    check_res(map_2.free_val == NULL, "pass `NULL` but map.free_val isn't `NULL`", &fail_cnt);
    test_map_integrity(&map_2, &fail_cnt);
    print_end("MAP | FUNC | INIT", fail_cnt);
    #define HM_TEST_COUNTER
}

void test_map_insert() {
    int num = 100;
    hm_map map;
    hm_map_init(&map, hash_int_1, cmp_int_up, free, free);
    int fail_cnt = 0;
    int fail = 0;
    int flag[num];
    
    print_run("MAP | FUNC | INSERT | TYPE K:[INT] V:[INT]");
    // insert
    for (int i = 0; i < num; i++) {
        flag[i] = i * 10;
        int* k = (int*)malloc(sizeof(int));
        int* v = (int*)malloc(sizeof(int));
        *k = i; *v = flag[i];
        // only return `hm_map_ret_suc` if the function runing correctly
        if (hm_map_insert(&map, k, v) != hm_map_ret_suc) {
            fail++;
        }
    }

    check_res(num == map.size, "the size of map is wrong", &fail_cnt);
    test_map_integrity(&map, &fail_cnt);

    // verify
    int fail_invalid_k = 0;
    int fail_diff_v = 0;
    for (int i = 0; i < map.len; i++) {
        if (map.buckets_status[i] == hm_exist_in_map) {
            int k = *(int*)(map.buckets[i].key);
            int v = *(int*)(map.buckets[i].val);
            if (k < 0 || k >= num) {
                fail_invalid_k++;
            } else {
                if (v != flag[k]) {
                    fail_diff_v++;
                }
            }
        }
    }
    check_res(fail_diff_v == 0, "value in map is wrong", &fail_cnt);
    check_res(fail_invalid_k == 0, "key in map is wrong", &fail_cnt);

    hm_map_free(&map);

    print_end("MAP | FUNC | INSERT | TYPE K:[INT] V:[INT]", fail_cnt);
    #define HM_TEST_COUNTER

}

void test_iter_map() {
    int num = 100;
    int flag[num];
    int fail_cnt = 0;
    hm_map map;
    hm_map_init(&map, hash_int_1, cmp_int_up, free, free);

    
    for (int i = 0; i < num; i++) {
        flag[i] = i * 10;
        int* k = (int*)malloc(sizeof(int));
        int* v = (int*)malloc(sizeof(int));
        *k = i; *v = flag[i];
        hm_map_insert(&map, k, v);
    }

    print_run("MAP | FUNC | ITERATOR | TYPE K:[INT] V:[INT]");
    int fail_invalid_k = 0;
    int fail_diff_v = 0;

    hm_iter_map iter;
    hm_iter_map_init(&iter, &map);
    while (hm_iter_map_has_next(&iter)) {
        hm_entry* e = hm_iter_map_next(&iter);
        int* k = e->key;
        int* v = e->val;
        if (*k >= num || *k < 0) {
            fail_invalid_k++;
        } else {
            if (*v != flag[*k]) {
                fail_diff_v++;
            }
        }
    }

    check_res(fail_invalid_k == 0, "the k got by iter_map is invalid ", &fail_cnt);
    check_res(fail_diff_v == 0, "the v of k is wrong got by iter_map", &fail_cnt);
    test_map_integrity(&map, &fail_cnt);

    hm_map_free(&map);

    print_end("MAP | FUNC | ITERATOR | TYPE K:[INT] V:[INT]", fail_cnt);
    #define HM_TEST_COUNTER


}

void test_map_get() {
    int num = 100;
    int flag[num];
    int fail_cnt = 0;
    hm_map map;
    hm_map_init(&map, hash_int_1, cmp_int_up, free, free);

    // insert
    for (int i = 0; i < num; i++) {
        flag[i] = i * 10;
        int* k = (int*)malloc(sizeof(int));
        int* v = (int*)malloc(sizeof(int));
        *k = i; *v = flag[i];
        hm_map_insert(&map, k, v);
    }


    print_run("MAP | FUNC | GET | TYPE K:[INT] V:[INT]");
    // verify valid k

    int fail_no_existed = 0;
    int fail_diff_v = 0;
    int fail_invalid_k = 0;
    for (int i = 0; i < num; i++) {
        hm_entry* e = hm_map_get(&map, &i);
        if (e) {
            int* k = e->key;
            int* v = e->val;
            
            if (*k != i) {
                fail_invalid_k++;
            } else {
                if (*v != flag[i]) {
                    fail_diff_v++;
                }
            }
        } else {
            fail_no_existed++;
        }

    }

    check_res(fail_no_existed == 0, "the function of `hm_map_get` return NULL when get entry of valid key", &fail_cnt);
    check_res(fail_invalid_k == 0, "the key of entry got by `hm_map_get` is different from key that needs to be request", &fail_cnt);
    check_res(fail_diff_v == 0, "val got by `hm_map_get` is wrong", &fail_cnt);
    test_map_integrity(&map, &fail_cnt);
    // verify invalid key

    int fail_exist = 0;
    for (int i = num; i < 2 * num; i++) {
        hm_entry* e = hm_map_get(&map, &i);
        if (e) {
            fail_exist++;
        }
    }

    check_res(fail_exist == 0, "the entry return by `hm_map_get` is NULL when key is invalid, but is not", &fail_cnt);

    hm_map_free(&map);

    print_end("MAP | FUNC | GET | TYPE K:[INT] V:[INT]", fail_cnt);
    #define HM_TEST_COUNTER

    


}

void test_map_change() {
    int num = 100;
    int flag[num];
    int fail_cnt = 0;
    hm_map map;
    hm_map_init(&map, hash_int_1, cmp_int_up, free, free);

    // insert
    for (int i = 0; i < num; i++) {
        flag[i] = i * 10;
        int* k = (int*)malloc(sizeof(int));
        int* v = (int*)malloc(sizeof(int));
        *k = i; *v = flag[i];
        hm_map_insert(&map, k, v);
    }


    print_run("MAP | FUNC | CHANGE | TYPE K:[INT] V:[INT]");

    // change
    int diff = 99;
    for (int i = 0; i < num; i++) {
        hm_entry* e = hm_map_get(&map, &i);
        flag[i] += diff;
        *(int*)(e->val) += diff;
    }

    // verify 
    int fail_diff = 0;
    int fail_no_exist = 0;
    for (int i = 0; i < num; i++) {
        hm_entry* e = hm_map_get(&map, &i);
        if (e) {
            int* v = e->val;
            if (*v != flag[i]) {
                fail_diff++;
            }
        } else {
            fail_no_exist++;
        }
    }

    check_res(fail_diff == 0, "the val is wrong after change all vals", &fail_cnt);
    check_res(fail_no_exist == 0, "the entry is not found after change", &fail_cnt);
    test_map_integrity(&map, &fail_cnt);
    hm_map_free(&map);

    print_end("MAP | FUNC | CHANGE | TYPE K:[INT] V:[INT]", fail_cnt);
    #define HM_TEST_COUNTER


}

void test_map_del() {
    int num = 100;
    int flag[num];
    int fail_cnt = 0;
    hm_map map;
    hm_map_init(&map, hash_int_1, cmp_int_up, free, free);

    // insert
    for (int i = 0; i < num; i++) {
        flag[i] = i * 10;
        int* k = (int*)malloc(sizeof(int));
        int* v = (int*)malloc(sizeof(int));
        *k = i; *v = flag[i];
        hm_map_insert(&map, k, v);
    }


    print_run("MAP | FUNC | DEL | TYPE K:[INT] V:[INT]");

    // del half of map

    int fail_del = 0;
    for (int i = 0; i < num / 2; i++) {
        if (hm_map_del(&map, &i) != hm_map_ret_suc) {
            fail_del++;
        }
    }
    check_res(fail_del == 0, "the function should return `hm_map_ret_suc`, but it is not", &fail_cnt);
    test_map_integrity(&map, &fail_cnt);

    // verify

    int fail_no_exist = 0;

    for (int i = 0; i < num / 2; i++) {
        hm_entry* e = hm_map_get(&map, &i);
        if (e) {
            fail_no_exist++;
        }
    }
    check_res(fail_no_exist == 0, "the del entry is still existed in map after del", &fail_cnt);


    // verify existed entry
    int fail_exist = 0;
    int fail_diff_v = 0;
    for (int i = num / 2; i < num; i++) {
        hm_entry* e = hm_map_get(&map, &i);

        if (e) {
            int* v = e->val;
            if (*v != flag[i]) {
                fail_diff_v++;
            }
        } else {
            fail_exist++;
        }

    }

    check_res(fail_diff_v == 0, "the existed entry's val is wrong after del half of map", &fail_cnt);
    check_res(fail_exist == 0, "the existed entry become NULL in map after del half of map", &fail_cnt);
    
    // del all 
    for (int i = num / 2; i < num; i++) {
        hm_map_del(&map, &i);
    }

    check_res(map.size == 0, "map.size isn't 0 after del all entrys", &fail_cnt);
    test_map_integrity(&map, &fail_cnt);

    // del empty map

    int k[] = {2, 3, 2, 100};
    int cnt = sizeof(k) / sizeof(int);

    int fail_del_invalid_k = 0;
    for (int i = 0; i < cnt; i++) {
        if (hm_map_del(&map, &i) != hm_map_ret_none) {
            fail_del_invalid_k++;
        }
    }
    check_res(fail_del_invalid_k == 0, "the tag of return isn't `hm_map_ret_none` when del some invlaid key in empty map", &fail_cnt);

    hm_map_free(&map);
    print_end("MAP | FUNC | DEL | TYPE K:[INT] V:[INT]", fail_cnt);
    #define HM_TEST_COUNTER



}

void test_map_shrink() {
    int num = 100;
    int flag[num];
    int fail_cnt = 0;
    hm_map map;
    hm_map_init(&map, hash_int_1, cmp_int_up, free, free);

    // insert
    for (int i = 0; i < num; i++) {
        flag[i] = i * 10;
        int* k = (int*)malloc(sizeof(int));
        int* v = (int*)malloc(sizeof(int));
        *k = i; *v = flag[i];
        hm_map_insert(&map, k, v);
    }

    // del some vals

    for (int i = 0; i < num; i += 2) {
        hm_map_del(&map, &i);
    }

    print_run("MAP | FUNC | SHRINK | TYPE K:[INT] V:[INT]");
    // shrink and verify

    int cnt = 10;
    int fail_shrink = 0;
    int fail_no_shrink = 0;
    int start_s = map.size;
    for (int i = 0; i < cnt; i++) {
        size_t l = map.len, s = map.size;
        hm_map_ret ret = hm_map_shrink(&map);
        if (l < 34 || 4 * s > l) {
            if (ret != hm_map_ret_none) {
                fail_shrink++;
            }
        } else {
            if (ret != hm_map_ret_suc) {
                fail_no_shrink++;
            }
        }
    }

    check_res(map.size == start_s, "the size fo entrys in map is wrong after shrink map", &fail_cnt);
    test_map_integrity(&map, &fail_cnt);
    check_res(fail_shrink == 0, "it shouldn't to shrink map but it do", &fail_cnt);
    check_res(fail_no_shrink == 0, "it should to shrink map but it not do", &fail_cnt);
    

    // shrink empty map
    hm_map_free(&map);
    check_res(hm_map_shrink(&map) == hm_map_ret_none, "it shouldn't to shrink empty map but it do", &fail_cnt);
    test_map_integrity(&map, &fail_cnt);

    print_end("MAP | FUNC | SHRINK | TYPE K:[INT] V:[INT]", fail_cnt);
    #define HM_TEST_COUNTER


}

void test_map_clear() {
    int num = 100;
    int flag[num];
    int fail_cnt = 0;
    hm_map map;
    hm_map_init(&map, hash_int_1, cmp_int_up, free, free);

    // insert
    for (int i = 0; i < num; i++) {
        flag[i] = i * 10;
        int* k = (int*)malloc(sizeof(int));
        int* v = (int*)malloc(sizeof(int));
        *k = i; *v = flag[i];
        hm_map_insert(&map, k, v);
    }

    print_run("MAP | FUNC | CLEAR | TYPE K:[INT] V:[INT]");
    // clear

    hm_map_clear(&map);

    // verfiy

    int fail_exist = 0;
    for (int i = 0; i < num; i++) {
        hm_entry* e = hm_map_get(&map, &i);
        if (e) {
            fail_exist++;
        }
    }
    test_map_integrity(&map, &fail_cnt);
    check_res(fail_exist == 0, "some entry still existed in map after clear this map", &fail_cnt);

    hm_map_free(&map);
    print_end("MAP | FUNC | CLEAR | TYPE K:[INT] V:[INT]", fail_cnt);
    #define HM_TEST_COUNTER
    
}

void test_map_free() {
    int num = 100;
    int flag[num];
    int fail_cnt = 0;
    hm_map map;
    hm_map_init(&map, hash_int_1, cmp_int_up, free, free);

    // insert
    for (int i = 0; i < num; i++) {
        flag[i] = i * 10;
        int* k = (int*)malloc(sizeof(int));
        int* v = (int*)malloc(sizeof(int));
        *k = i; *v = flag[i];
        hm_map_insert(&map, k, v);
    }

    print_run("MAP | FUNC | FREE | TYPE K:[INT] V:[INT]");

    hm_map_free(&map);

    check_res(map.size == 0, "map.size isn't 0 after free map", &fail_cnt);
    check_res(map.len == 0, "map.len isn't 0 after free map", &fail_cnt);
    test_map_integrity(&map, &fail_cnt);
    
    // double free
    hm_map_free(&map);
    check_res(map.size == 0, "map.size isn't 0 after double free map", &fail_cnt);
    check_res(map.len == 0, "map.len isn't 0 after double free map", &fail_cnt);
    test_map_integrity(&map, &fail_cnt);
    

    print_end("MAP | FUNC | FREE | TYPE K:[INT] V:[INT]", fail_cnt);
    #define HM_TEST_COUNTER



}

void test_map_insert_stress() {
    // type : int
    int fail_cnt = 0;
    size_t nums[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums) / sizeof(size_t);
    print_run("MAP | STRESS | INSERT RANDOM | TYPE K:[INT] V:[INT]");
    for (int i = 0; i < cnt; i++) {
        int* k = (int*)malloc(nums[i] * sizeof(int));
        int* v = (int*)malloc(nums[i] * sizeof(int));
        for (size_t j = 0; j < nums[i]; j++) {
            k[j] = j;
            v[j] = j;
        }

        size_t same = 0, suc = 0, fail = 0;

        hm_map map;
        hm_map_init(&map, hash_int_1, cmp_int_up, NULL, NULL);
        clock_t start = clock();
        for (size_t j = 0; j < nums[i]; j++) {
            hm_map_ret ret = hm_map_insert(&map, &k[j], &v[j]);
            
            switch (ret) {
                case hm_map_ret_suc: suc++; break;
                case hm_map_ret_error: fail++; break;
                case hm_map_ret_existed: same++; break;
            }

        }
        clock_t end = clock();
        check_res(suc == map.size, "the successful counter is different from map.size", &fail_cnt);
        check_res(suc + fail + same == nums[i], "the all tag of reurn from map_insert is different from the size of this insert stressful test", &fail_cnt);
        test_map_integrity(&map, &fail_cnt);
        print_run_time("INSERT", start, end, nums[i], nums[i]);

        free(k);
        free(v);
        hm_map_free(&map);
    }
    print_end("MAP | STRESS | INSERT RANDOM | TYPE K:[INT] V:[INT]", fail_cnt);
    #define HM_TEST_COUNTER
}

void test_map_insert_same() {

    print_run("MAP | BOUNDARY | INSRET SAME ENTRY | TYPE K:[INT] V:[INT]");
    int cnt = 1000;
    // the key is different from every element of keys
    int fail_cnt = 0;
    int keys[] = {100, 200};
    int vals[] = {200, 300};
    int num = sizeof(keys) / sizeof(int);
    hm_map map;
    hm_map_init(&map, hash_int_1, cmp_int_up, free, free);
    for (int i = 0; i < num; i++) {

        for (int j = 0; j < cnt; j++) {
            int* k = (int*)malloc(sizeof(int));
            int* v = (int*)malloc(sizeof(int));
            *k = keys[i];
            *v = vals[i];
            hm_map_ret ret = hm_map_insert(&map, k, v);
            if (ret == hm_map_ret_existed) {
                free(k);
            }
        }
    }


    check_res(map.size == num, "insert many some key in map, but map.size is wrong", &fail_cnt);
    test_map_integrity(&map, &fail_cnt);
    hm_map_free(&map);
    print_end("MAP | BOUNDARY | INSRET SAME ENTRY | TYPE K:[INT] V:[INT]", fail_cnt);
    #define HM_TEST_COUNTER


}

void test_map_get_stress() {

    int fail_cnt = 0;
    // the value of num cann't greater than 10^9
    int nums[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums) / sizeof(int);
    hm_map map;
    hm_map_init(&map, hash_int_1, cmp_int_up, free, free);

    print_run("MAP | STRESS | GET | TYPE K:[INT] V:[INT]");


    for (int i = 0; i < cnt; i++) {
        // insert
        for (int j = 0; j < nums[i]; j++) {
            int* k = (int*)malloc(sizeof(int));
            int* v = (int*)malloc(sizeof(int));
            *k = j;
            *v = j;
            hm_map_insert(&map, k, v);
        }
        
        // find existent
        
        clock_t start = clock();
        int fail_existed = 0;
        for (int j = 0; j < nums[i]; j++) {
            hm_entry* e = hm_map_get(&map, &j);
            if (e == NULL) {
                fail_existed++;
            }
        }
        clock_t end = clock();
        test_map_integrity(&map, &fail_cnt);
        check_res(fail_existed == 0, "the get function get NULL when key is existent in map", &fail_cnt);
        print_run_time("GET EXISTENT ENTRY", start, end, nums[i], nums[i]);
        
        // find non-existent
        
        int fail_no_existed = 0;
        start = clock();
        for (int j = nums[i]; j < 2 * nums[i]; j++) {
            hm_entry* e = hm_map_get(&map, &j);
            if (e) {
                fail_no_existed++;
            }
        }
        end = clock();
        test_map_integrity(&map, &fail_cnt);
        check_res(fail_no_existed == 0, "the get function get entry when key is non-existent in map", &fail_cnt);
        print_run_time("GET NON-EXISTENT ENTRY", start, end, nums[i], nums[i]);
        
        hm_map_free(&map);

    }
    print_end("MAP | STRESS | GET | TYPE K:[INT] V:[INT]", fail_cnt);
    #define HM_TEST_COUNTER
}

void test_map_del_stress() {
    int fail_cnt = 0;
    // the value of num cann't greater than 10^9
    int nums[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums) / sizeof(int);
    hm_map map;
    hm_map_init(&map, hash_int_1, cmp_int_up, free, free);

    print_run("MAP | STRESS | DEL | TYPE K:[INT] V:[INT]");


    for (int i = 0; i < cnt; i++) {
        // insert
        for (int j = 0; j < nums[i]; j++) {
            int* k = (int*)malloc(sizeof(int));
            int* v = (int*)malloc(sizeof(int));
            *k = j;
            *v = j;
            hm_map_insert(&map, k, v);
        }
        
        // del non-existent


        int fail_del_noexist = 0;
        clock_t start = clock();
        for (int j = nums[i]; j < 2 * nums[i]; j++) {
            hm_map_ret ret = hm_map_del(&map, &j);
            if (ret != hm_map_ret_none) {
                fail_del_noexist++;
            }
        }
        clock_t end = clock();
        test_map_integrity(&map, &fail_cnt);
        check_res(fail_del_noexist == 0, "del non-existent entry should return none", &fail_cnt);
        print_run_time("DEL NON-EXISTENT ENTRY", start, end, nums[i], nums[i]);
        
        // del existent
        
        int fail_del_exist = 0;
        start = clock();
        for (int j = 0; j < nums[i]; j++) {
            hm_map_ret ret = hm_map_del(&map, &j);
            if (ret != hm_map_ret_suc) {
                fail_del_exist++;
            }
        }
        end = clock();
        test_map_integrity(&map, &fail_cnt);
        check_res(fail_del_exist == 0, "del existent entry should return suc", &fail_cnt);
        print_run_time("DEL EXISTENT ENTRY", start, end, nums[i], nums[i]);
        
        hm_map_free(&map);

    }
    print_end("MAP | STRESS | DEL | TYPE K:[INT] V:[INT]", fail_cnt);
    #define HM_TEST_COUNTER
}

void test_map_clear_stress() {
    int fail_cnt = 0;
    // the value of num cann't greater than 10^9
    int nums_free[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums_free) / sizeof(int);
    hm_map map;

    // clear the map including entry that have power to free the key and value
    hm_map_init(&map, hash_int_1, cmp_int_up, free, free);
    
    print_run("MAP | STRESS | CLEAR (hm_free: free) | TYPE K:[INT] V:[INT]");
    
    
    for (int i = 0; i < cnt; i++) {
        // insert
        for (int j = 0; j < nums_free[i]; j++) {
            int* k = (int*)malloc(sizeof(int));
            int* v = (int*)malloc(sizeof(int));
            *k = j;
            *v = j;
            hm_map_insert(&map, k, v);
        }
        
        // clear
        clock_t start = clock();
        
        hm_map_clear(&map);
        
        clock_t end = clock();
        test_map_integrity(&map, &fail_cnt);
        print_run_time("CLEAR", start, end, nums_free[i], nums_free[i]);
        
        hm_map_free(&map);
        
    }
    print_end("MAP | STRESS | CLEAR (hm_free: free) | TYPE K:[INT] V:[INT]\n", fail_cnt);
    #define HM_TEST_COUNTER
    
    
    fail_cnt = 0;
    // the value of num cann't greater than 10^9
    int nums_null[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    cnt = sizeof(nums_null) / sizeof(int);
    // clear the map including entry that don't have power to free the key and value
    hm_map_init(&map, hash_int_1, cmp_int_up, NULL, NULL);

    print_run("MAP | STRESS | CLEAR (hm_free: NULL) | TYPE K:[INT] V:[INT]");


    for (int i = 0; i < cnt; i++) {

        // insert
        int* keys = (int*)malloc(nums_null[i] * sizeof(int));
        int* vals = (int*)malloc(nums_null[i] * sizeof(int));
        
        for (int j = 0; j < nums_null[i]; j++) {
            keys[j] = j;
            vals[j] = j;
            hm_map_insert(&map, &keys[j], &vals[j]);
        }
        
        // clear
        clock_t start = clock();

        hm_map_clear(&map);

        clock_t end = clock();
        test_map_integrity(&map, &fail_cnt);
        print_run_time("CLEAR", start, end, nums_null[i], nums_null[i]);

        free(keys);
        free(vals);
        hm_map_free(&map);

    }
    print_end("MAP | STRESS | CLEAR (hm_free: NULL) | TYPE K:[INT] V:[INT]\n", fail_cnt);
    #define HM_TEST_COUNTER
}


void test_map_free_stress() {
    int fail_cnt = 0;
    // the value of num cann't greater than 10^9
    int nums_free[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums_free) / sizeof(int);
    hm_map map;
    
    // free the map including entry that have power to free the key and value
    hm_map_init(&map, hash_int_1, cmp_int_up, free, free);
    
    print_run("MAP | STRESS | FREE (hm_free: free) | TYPE K:[INT] V:[INT]");
    
    
    for (int i = 0; i < cnt; i++) {
        // insert
        for (int j = 0; j < nums_free[i]; j++) {
            int* k = (int*)malloc(sizeof(int));
            int* v = (int*)malloc(sizeof(int));
            *k = j;
            *v = j;
            hm_map_insert(&map, k, v);
        }
        
        // free
        clock_t start = clock();
        
        hm_map_free(&map);
        
        clock_t end = clock();
        test_map_integrity(&map, &fail_cnt);
        print_run_time("FREE", start, end, nums_free[i], nums_free[i]);
        
        
    }
    print_end("MAP | STRESS | FREE (hm_free: free) | TYPE K:[INT] V:[INT]\n", fail_cnt);
    #define HM_TEST_COUNTER
    
    
    fail_cnt = 0;
    // the value of num cann't greater than 10^9
    int nums_null[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    cnt = sizeof(nums_null) / sizeof(int);
    // free the map including entry that don't have power to free the key and value
    hm_map_init(&map, hash_int_1, cmp_int_up, NULL, NULL);
    
    print_run("MAP | STRESS | FREE (hm_free: NULL) | TYPE K:[INT] V:[INT]");
    
    
    for (int i = 0; i < cnt; i++) {
    
        // insert
        int* keys = (int*)malloc(nums_null[i] * sizeof(int));
        int* vals = (int*)malloc(nums_null[i] * sizeof(int));
        
        for (int j = 0; j < nums_null[i]; j++) {
            keys[j] = j;
            vals[j] = j;
            hm_map_insert(&map, &keys[j], &vals[j]);
        }
        
        // free
        clock_t start = clock();
    
        hm_map_free(&map);        
    
        clock_t end = clock();
        test_map_integrity(&map, &fail_cnt);
        print_run_time("FREE", start, end, nums_null[i], nums_null[i]);
    
        free(keys);
        free(vals);
    }
    print_end("MAP | STRESS | FREE (hm_free: NULL) | TYPE K:[INT] V:[INT]\n", fail_cnt);
    #define HM_TEST_COUNTER

}


void test_map_iter_stress() {
    int fail_cnt = 0;
    // the value of num cann't greater than 10^9
    int nums[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums) / sizeof(int);
    hm_map map;
    hm_map_init(&map, hash_int_1, cmp_int_up, free, free);
    
    print_run("MAP | STRESS | ITERATOR | TYPE K:[INT] V:[INT]");

    for (int i = 0; i < cnt; i++) {
        // insert
        for (int j = 0; j < nums[i]; j++) {
            int* k = (int*)malloc(sizeof(int));
            int* v = (int*)malloc(sizeof(int));
            *k = j;
            *v = j * 2;
            hm_map_insert(&map, k, v);
        }

        // itering map

        hm_iter_map iter;
        hm_iter_map_init(&iter, &map);

        int fail_iter = 0;
        clock_t start = clock();

        while (hm_iter_map_has_next(&iter)) {
            hm_entry* e = hm_iter_map_next(&iter);
            int k = *(int*)(e->key);
            int v = *(int*)(e->val);
            if (v != 2 * k) {
                fail_iter++;
            }
        }
        clock_t end = clock();
        check_res(fail_iter == 0, "the value shoule be twice key", &fail_cnt);
        test_map_integrity(&map, &fail_cnt);
        print_run_time("ITER", start, end, nums[i], nums[i]);
        hm_map_free(&map);

    }
    print_end("MAP | STRESS | ITERATOR | TYPE K:[INT] V:[INT]", fail_cnt);
    #define HM_TEST_COUNTER
}

void test_empty_map_oper() {
    int fail_cnt = 0;
    hm_map map;
    hm_map_init(&map, hash_int_1, cmp_int_up, free, free);

    print_run("MAP | BOUNDARY | OPER EMPTY MAP | TYPE K:[INT] V:[INT]");

    int k = 0;
    // get
    check_res(hm_map_get(&map, &k) == NULL, "get on empty map should return `NULL`", &fail_cnt);

    // del
    k = 10;
    check_res(hm_map_del(&map, &k) == hm_map_ret_none, "del on empty map should return `NULL`", &fail_cnt);

    // iter

    hm_iter_map iter;
    hm_iter_map_init(&iter, &map);
    int loop_cnt = 0;
    while (hm_iter_map_has_next(&iter)) {
        hm_iter_map_next(&iter);
        loop_cnt++;
    }
    check_res(loop_cnt == 0, "iterator over empty map should yield zero entrys", &fail_cnt);


    print_end("MAP | BOUNDARY | OPER EMPTY MAP | TYPE K:[INT] V:[INT]", fail_cnt);
    #define HM_TEST_COUNTER


}

void test_single_entry_oper() {
    int fail_cnt = 0;
    hm_map map;
    hm_map_init(&map, hash_int_1, cmp_int_up, NULL, NULL);
    int k = 1, v = 10;
    print_run("MAP | BOUNDARY | OPER SINGLE ENTRY'S LIST | TYPE K:[INT] V:[INT]");

    // insert single entry and get

    hm_map_insert(&map, &k, &v);
    hm_entry* e = hm_map_get(&map, &k);
    check_res(*(int*)(e->key) == k, "the key is wrong when run `map_get` on single entry's map", &fail_cnt);
    check_res(*(int*)(e->val) == v, "the val is wrong when run `map_get` on single entry's map", &fail_cnt);
    hm_map_free(&map);

    // insert single entry and delete it 

    hm_map_insert(&map, &k, &v);
    check_res(hm_map_del(&map, &k) == hm_map_ret_suc, "del on single entry's map should return suc", &fail_cnt);
    check_res(map.size == 0, "map.size should be `zero` after del on single entry's map", &fail_cnt);
    test_map_integrity(&map, &fail_cnt);
    hm_map_free(&map);

    // insert two indetical keys

    hm_map_insert(&map, &k, &v);
    int new_v = 10;
    hm_map_insert(&map, &k, &new_v);
    e = hm_map_get(&map, &k);
    check_res(*(int*)(e->key) == k, "the key is wrong when insert two indetical keys", &fail_cnt);
    check_res(*(int*)(e->val) == new_v, "the key is wrong when insert two indetical keys", &fail_cnt);

    check_res(map.size == 1, "the map.size should be 1 when insert two indetical keys", &fail_cnt);
    test_map_integrity(&map, &fail_cnt);
    hm_map_free(&map);


    print_end("MAP | BOUNDARY | OPER SINGLE ENTRY'S LIST | TYPE K:[INT] V:[INT]", fail_cnt);
    #define HM_TEST_COUNTER
}

void function_test() {
    test_map_init();                                printf("\n");
    
    test_map_insert();                              printf("\n");
    
    test_iter_map();                                printf("\n");
    
    test_map_get();                                 printf("\n");
    
    test_map_change();                              printf("\n");
    
    test_map_del();                                 printf("\n");
    
    test_map_shrink();                              printf("\n");
    
    test_map_clear();                               printf("\n");
    
    test_map_free();                                printf("\n");

}


void boundary_test() {
    test_map_insert_same();                         printf("\n");

    test_empty_map_oper();                          printf("\n");

    test_single_entry_oper();                       printf("\n");

}

void stress_test() {
    test_map_insert_stress();                       printf("\n");
    
    test_map_get_stress();                          printf("\n");

    test_map_del_stress();                          printf("\n");

    test_map_clear_stress();                        printf("\n");

    test_map_free_stress();                         printf("\n");

    test_map_iter_stress();                         printf("\n");

}




int main()
{
    function_test();

    boundary_test();

    // stress_test();


    return all_failure_num;
}