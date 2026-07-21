/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */
#include "../../include/hm_set.h"
#include "../../function/hash/hm_hash.h"
#include "../../function/cmp/hm_cmp.h"
#include "../hm_test_tool.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#define HM_TEST_COUNTER \
    all_failure_num += fail_cnt;


// This variable can record the all number of failure
int all_failure_num = 0;

void test_set_integrity(hm_set* set, int* fail_cnt, int tag) {

    // size [exist | none | del]
    // Tip: Assume the set.len is true because can't verify the correctness of set.len
    int len = set->len;

    int existed = 0, none = 0, del = 0;
    int others = 0;
    for (int i = 0; i < len; i++) {
        switch (set->buckets_status[i]) {
            case hm_exist_in_set: existed++; break;
            case hm_del_in_set: del++; break;
            case hm_none_in_set: none++; break;
            default: others++; break;
        }
    }
    
    check_res(others == 0, "TEST OF INTEGRITY: there are unkown tag of entry_status in set", fail_cnt, tag);
    check_res(existed == set->size, "TEST OF INTEGRITY: the real size of existed entry in set is wrong", fail_cnt, tag);
}

void test_set_init() {
    hm_set set_1;
    int fail_cnt = 0;
    int tag = 0;
    print_run("SET | FUNC | INIT");
    hm_set_init(&set_1, hash_int_1, cmp_int_up, free);

    check_res(set_1.buckets == NULL, "the buckets isn't NULL", &fail_cnt, tag++);
    check_res(set_1.buckets_status == NULL, "the buckets_status isn't NULL", &fail_cnt, tag++);
    check_res(set_1.cmp == cmp_int_up, "pass `cmp_int_up` but set.cmp isn't `cmp_int_up`", &fail_cnt, tag++);
    check_res(set_1.hash == hash_int_1, "pass `hash_int_1` but set.hash isn't `hash_int_1`", &fail_cnt, tag++);
    check_res(set_1.free_key == free, "pass `free` for set.free_key but set.free_key isn't `free`", &fail_cnt, tag++);     
    check_res(set_1.len == 0, "set.len isn't 0", &fail_cnt, tag++);
    check_res(set_1.size == 0, "set.size isn't 0", &fail_cnt, tag++);
    test_set_integrity(&set_1, &fail_cnt, tag++);
    
    hm_set set_2;
    hm_set_init(&set_2, hash_int_1, cmp_int_up, NULL);
    
    check_res(set_2.free_key == NULL, "pass `NULL` but set.free_key isn't `NULL`", &fail_cnt, tag++);
    test_set_integrity(&set_2, &fail_cnt, tag++);
    print_end("SET | FUNC | INIT", fail_cnt);
    HM_TEST_COUNTER
}

void test_set_insert() {
    int num = 100;
    int counters[num];
    hm_set set;
    hm_set_init(&set, hash_int_1, cmp_int_up, free);
    int fail_cnt = 0;
    int tag = 0;
    int fail = 0;
    
    print_run("SET | FUNC | INSERT | TYPE: [INT]");
    // insert
    for (int i = 0; i < num; i++) {
        int* k = (int*)malloc(sizeof(int));
        *k = i;
        // only return `hm_set_ret_suc` if the function runing correctly
        if (hm_set_insert(&set, k) != hm_set_ret_suc) {
            fail++;
        }
    }

    check_res(num == set.size, "the size of set is wrong", &fail_cnt, tag++);
    test_set_integrity(&set, &fail_cnt, tag++);

    // verify
    int fail_no_exist_k = 0;
    int fail_count_k = 0;
    int fail_invalid_k = 0;
    memset(counters, 0, sizeof(counters));

    for (int i = 0; i < set.len; i++) {
        if (set.buckets_status[i] == hm_exist_in_set) {
            int* k = (set.buckets[i].key);
            if (k == NULL) {
                fail_no_exist_k++;
            } else if (*k < 0 || *k >= num) {
                fail_invalid_k++;
            } else {
                if (counters[*k]) {
                    fail_count_k++;
                }
                counters[*k]++;
            }
            
        }
    }
    check_res(fail_count_k == 0, "key in set is repetitive", &fail_cnt, tag++);
    check_res(fail_invalid_k == 0, "key in set is invalid(out of bound)", &fail_cnt, tag++);
    check_res(fail_no_exist_k == 0, "key in set is NULL", &fail_cnt, tag++);

    hm_set_free(&set);

    print_end("SET | FUNC | INSERT | TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER

}

void test_iter_set() {
    int num = 100;
    int counters[num];
    int fail_cnt = 0;
    int tag = 0;
    hm_set set;
    hm_set_init(&set, hash_int_1, cmp_int_up, free);

    
    for (int i = 0; i < num; i++) {
        int* k = (int*)malloc(sizeof(int));
        *k = i;
        hm_set_insert(&set, k);
    }

    print_run("SET | FUNC | ITERATOR | TYPE: [INT]");


    int fail_no_exist_k = 0;
    int fail_count_k = 0;
    int fail_invalid_k = 0;
    int fail_no_exist_e = 0;
    memset(counters, 0, sizeof(counters));
    

    hm_iter_set iter;
    hm_iter_set_init(&iter, &set);
    while (hm_iter_set_has_next(&iter)) {
        hm_set_entry* e = hm_iter_set_next(&iter);
        if (e) {
            int* k = e->key;
                if (k == NULL) {
                    fail_no_exist_k++;
                } else if (*k < 0 || *k >= num) {
                    fail_invalid_k++;
                } else {
                    if (counters[*k]) {
                        fail_count_k++;
                    }
                    counters[*k]++;
                }
        } else {
            fail_no_exist_e++;
        }
    }

    check_res(fail_no_exist_e == 0, "entry is NULL in iterating", &fail_cnt, tag++);
    check_res(fail_count_k == 0, "key in set is repetitive in iterating", &fail_cnt, tag++);
    check_res(fail_invalid_k == 0, "key in set is invalid(out of bound) in iterating", &fail_cnt, tag++);
    check_res(fail_no_exist_k == 0, "key in set is NULL in iterating", &fail_cnt, tag++);

    test_set_integrity(&set, &fail_cnt, tag++);

    hm_set_free(&set);

    print_end("SET | FUNC | ITERATOR | TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER


}

void test_set_get() {
    int num = 100;
    int counters[num];
    int fail_cnt = 0;
    int tag = 0;
    hm_set set;
    hm_set_init(&set, hash_int_1, cmp_int_up, free);

    // insert
    for (int i = 0; i < num; i++) {
        int* k = (int*)malloc(sizeof(int));
        *k = i;
        hm_set_insert(&set, k);
    }


    print_run("SET | FUNC | GET | TYPE: [INT]");
    // verify valid k

    int fail_no_exist_k = 0;
    int fail_count_k = 0;
    int fail_invalid_k = 0;
    int fail_no_exist_e = 0;
    memset(counters, 0, sizeof(counters));
    for (int i = 0; i < num; i++) {
        hm_set_entry* e = hm_set_get(&set, &i);
        if (e) {
            int* k = e->key;
                if (k == NULL) {
                    fail_no_exist_k++;
                } else if (*k < 0 || *k >= num) {
                    fail_invalid_k++;
                } else {
                    if (counters[*k]) {
                        fail_count_k++;
                    }
                    counters[*k]++;
                }
        } else {
            fail_no_exist_e++;
        }

    }

    check_res(fail_no_exist_e == 0, "entry is NULL when get valid key", &fail_cnt, tag++);
    check_res(fail_count_k == 0, "key in set is repetitive when get valid key", &fail_cnt, tag++);
    check_res(fail_invalid_k == 0, "key in set is invalid(out of bound) when get valid key", &fail_cnt, tag++);
    check_res(fail_no_exist_k == 0, "key in set is NULL when get valid key", &fail_cnt, tag++);
    
    
    // verify invalid key

    int fail_exist = 0;
    for (int i = num; i < 2 * num; i++) {
        hm_set_entry* e = hm_set_get(&set, &i);
        if (e) {
            fail_exist++;
        }
    }

    check_res(fail_exist == 0, "entry is valid when get invalid key", &fail_cnt, tag++);

    hm_set_free(&set);

    print_end("SET | FUNC | GET | TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER

    


}


void test_set_del() {
    int num = 100;
    int fail_cnt = 0;
    int tag = 0;
    int counters[num];
    hm_set set;
    hm_set_init(&set, hash_int_1, cmp_int_up, free);

    // insert
    for (int i = 0; i < num; i++) {
        int* k = (int*)malloc(sizeof(int));
        *k = i;
        hm_set_insert(&set, k);
    }


    print_run("SET | FUNC | DEL | TYPE: [INT]");

    // del half of set

    int fail_del = 0;
    for (int i = 0; i < num / 2; i++) {
        if (hm_set_del(&set, &i) != hm_set_ret_suc) {
            fail_del++;
        }
    }
    check_res(fail_del == 0, "the function should return `hm_set_ret_suc`, but it is not", &fail_cnt, tag++);
    test_set_integrity(&set, &fail_cnt, tag++);

    // verify

    int fail_no_exist = 0;

    for (int i = 0; i < num / 2; i++) {
        hm_set_entry* e = hm_set_get(&set, &i);
        if (e) {
            fail_no_exist++;
        }
    }
    check_res(fail_no_exist == 0, "the del entry is still existed in set after del", &fail_cnt, tag++);


    // verify existed entry
    int fail_no_exist_k = 0;
    int fail_count_k = 0;
    int fail_invalid_k = 0;
    int fail_no_exist_e = 0;
    memset(counters, 0, sizeof(counters));
    for (int i = num / 2; i < num; i++) {
        hm_set_entry* e = hm_set_get(&set, &i);

        if (e) {
            int* k = e->key;
                if (k == NULL) {
                    fail_no_exist_k++;
                } else if (*k > num || *k < num / 2) {
                    fail_invalid_k++;
                } else {
                    if (counters[*k]) {
                        fail_count_k++;
                    }
                    counters[*k]++;
                }
        } else {
            fail_no_exist_e++;
        }

    }

    check_res(fail_no_exist_e == 0, "existed entry is NULL after del half of set", &fail_cnt, tag++);
    check_res(fail_count_k == 0, "existed key in set is repetitive after del half of set", &fail_cnt, tag++);
    check_res(fail_invalid_k == 0, "existed key in set is invalid(out of bound) after del half of set", &fail_cnt, tag++);
    check_res(fail_no_exist_k == 0, "existed key in set is NULL after del half of set", &fail_cnt, tag++);
    
    
    // del all 
    for (int i = num / 2; i < num; i++) {
        hm_set_del(&set, &i);
    }

    check_res(set.size == 0, "set.size isn't 0 after del all entrys", &fail_cnt, tag++);
    test_set_integrity(&set, &fail_cnt, tag++);

    // del empty set

    int k[] = {2, 3, 2, 100};
    int cnt = sizeof(k) / sizeof(int);

    int fail_del_invalid_k = 0;
    for (int i = 0; i < cnt; i++) {
        if (hm_set_del(&set, &i) != hm_set_ret_none) {
            fail_del_invalid_k++;
        }
    }
    check_res(fail_del_invalid_k == 0, "the tag of return isn't `hm_set_ret_none` when del some invlaid key in empty set", &fail_cnt, tag++);

    hm_set_free(&set);
    print_end("SET | FUNC | DEL | TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER



}

void test_set_shrink() {
    int num = 100;
    int fail_cnt = 0;
    int tag = 0;
    hm_set set;
    hm_set_init(&set, hash_int_1, cmp_int_up, free);

    // insert
    for (int i = 0; i < num; i++) {
        int* k = (int*)malloc(sizeof(int));
        *k = i;
        hm_set_insert(&set, k);
    }

    // del some

    for (int i = 0; i < num; i += 2) {
        hm_set_del(&set, &i);
    }

    print_run("SET | FUNC | SHRINK | TYPE: [INT]");
    // shrink and verify

    int cnt = 10;
    int fail_shrink = 0;
    int fail_no_shrink = 0;
    int start_s = set.size;
    for (int i = 0; i < cnt; i++) {
        size_t l = set.len, s = set.size;
        hm_set_ret ret = hm_set_shrink(&set);
        if (l < 34 || ((double)s / l) > 0.25) {
            if (ret != hm_set_ret_none) {
                fail_shrink++;
            }
        } else {
            if (ret != hm_set_ret_suc) {
                fail_no_shrink++;
            }
        }
    }

    check_res(set.size == start_s, "the size fo entrys in set is wrong after shrink set", &fail_cnt, tag++);
    test_set_integrity(&set, &fail_cnt, tag++);
    check_res(fail_shrink == 0, "it shouldn't to shrink set but it do", &fail_cnt, tag++);
    check_res(fail_no_shrink == 0, "it should to shrink set but it not do", &fail_cnt, tag++);
    

    // shrink empty set
    hm_set_free(&set);
    check_res(hm_set_shrink(&set) == hm_set_ret_none, "it shouldn't to shrink empty set but it do", &fail_cnt, tag++);
    test_set_integrity(&set, &fail_cnt, tag++);

    print_end("SET | FUNC | SHRINK | TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER


}

void test_set_clear() {
    int num = 100;
    int counters[num];
    int fail_cnt = 0;
    int tag = 0;
    hm_set set;
    hm_set_init(&set, hash_int_1, cmp_int_up, free);

    // insert
    for (int i = 0; i < num; i++) {
        int* k = (int*)malloc(sizeof(int));
        *k = i;
        hm_set_insert(&set, k);
    }

    print_run("SET | FUNC | CLEAR | TYPE: [INT]");
    // clear

    hm_set_clear(&set);

    // verfiy

    int fail_exist = 0;
    for (int i = 0; i < num; i++) {
        hm_set_entry* e = hm_set_get(&set, &i);
        if (e) {
            fail_exist++;
        }
    }
    test_set_integrity(&set, &fail_cnt, tag++);
    check_res(fail_exist == 0, "some entry still existed in set after clear this set", &fail_cnt, tag++);

    hm_set_free(&set);
    print_end("SET | FUNC | CLEAR | TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER
    
}

void test_set_free() {
    int num = 100;
    int fail_cnt = 0;
    int tag = 0;
    hm_set set;
    hm_set_init(&set, hash_int_1, cmp_int_up, free);

    // insert
    for (int i = 0; i < num; i++) {
        int* k = (int*)malloc(sizeof(int));
        *k = i;
        hm_set_insert(&set, k);
    }

    print_run("SET | FUNC | FREE | TYPE: [INT]");

    hm_set_free(&set);

    check_res(set.size == 0, "set.size isn't 0 after free set", &fail_cnt, tag++);
    check_res(set.len == 0, "set.len isn't 0 after free set", &fail_cnt, tag++);
    test_set_integrity(&set, &fail_cnt, tag++);
    
    // double free
    hm_set_free(&set);
    check_res(set.size == 0, "set.size isn't 0 after double free set", &fail_cnt, tag++);
    check_res(set.len == 0, "set.len isn't 0 after double free set", &fail_cnt, tag++);
    test_set_integrity(&set, &fail_cnt, tag++);
    

    print_end("SET | FUNC | FREE | TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER



}

void test_set_insert_stress() {
    // type : int
    int fail_cnt = 0;
    int tag = 0;
    size_t nums[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums) / sizeof(size_t);
    print_run("SET | STRESS | INSERT RANDOM | TYPE: [INT]");
    for (int i = 0; i < cnt; i++) {
        int* k = (int*)malloc(nums[i] * sizeof(int));
        for (size_t j = 0; j < nums[i]; j++) {
            k[j] = j;
        }

        size_t same = 0, suc = 0, fail = 0;

        hm_set set;
        hm_set_init(&set, hash_int_1, cmp_int_up, NULL);
        clock_t start = clock();
        for (size_t j = 0; j < nums[i]; j++) {
            hm_set_ret ret = hm_set_insert(&set, &k[j]);
            
            switch (ret) {
                case hm_set_ret_suc: suc++; break;
                case hm_set_ret_error: fail++; break;
                case hm_set_ret_existed: same++; break;
            }

        }
        clock_t end = clock();
        check_res(suc == set.size, "the successful counter is different from set.size", &fail_cnt, tag++);
        check_res(suc + fail + same == nums[i], "the all tag of reurn from set_insert is different from the size of this insert stressful test", &fail_cnt, tag++);
        test_set_integrity(&set, &fail_cnt, tag++);
        print_run_time("INSERT", start, end, nums[i], nums[i]);

        free(k);
        hm_set_free(&set);
    }
    print_end("SET | STRESS | INSERT RANDOM | TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER
}

void test_set_insert_same() {

    print_run("SET | BOUNDARY | INSRET SAME ENTRY | TYPE: [INT]");
    int cnt = 1000;
    // the key is different from every element of keys
    int fail_cnt = 0;
    int tag = 0;
    int keys[] = {100, 200};
    int num = sizeof(keys) / sizeof(int);
    hm_set set;
    hm_set_init(&set, hash_int_1, cmp_int_up, free);
    int fail_ret = 0;
    for (int i = 0; i < num; i++) {

        // insert a key firstly
        int* k = (int*)malloc(sizeof(int));
        *k = keys[i];
        hm_set_ret ret = hm_set_insert(&set, k);

        for (int j = 0; j < cnt; j++) {
            k = (int*)malloc(sizeof(int));
            *k = keys[i];
            hm_set_ret ret = hm_set_insert(&set, k);
            if (ret == hm_set_ret_existed) {
                free(k);
            } else {
                fail_ret++;
            }
        }
    }

    check_res(fail_ret == 0, "insert function should return existed when key is repetitive", &fail_cnt, tag++);
    check_res(set.size == num, "insert many some key in set, but set.size is wrong", &fail_cnt, tag++);
    test_set_integrity(&set, &fail_cnt, tag++);
    hm_set_free(&set);
    print_end("SET | BOUNDARY | INSRET SAME ENTRY | TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER


}

void test_set_get_stress() {

    int fail_cnt = 0;
    int tag = 0;
    // the value of num cann't greater than 10^9
    int nums[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums) / sizeof(int);
    hm_set set;
    hm_set_init(&set, hash_int_1, cmp_int_up, free);

    print_run("SET | STRESS | GET | TYPE: [INT]");


    for (int i = 0; i < cnt; i++) {
        // insert
        for (int j = 0; j < nums[i]; j++) {
            int* k = (int*)malloc(sizeof(int));
            *k = j;
            hm_set_insert(&set, k);
        }
        
        // find existent
        
        clock_t start = clock();
        int fail_existed = 0;
        for (int j = 0; j < nums[i]; j++) {
            hm_set_entry* e = hm_set_get(&set, &j);
            if (e == NULL) {
                fail_existed++;
            }
        }
        clock_t end = clock();
        test_set_integrity(&set, &fail_cnt, tag++);
        check_res(fail_existed == 0, "the get function get NULL when key is existent in set", &fail_cnt, tag++);
        print_run_time("GET EXISTENT ENTRY", start, end, nums[i], nums[i]);
        
        // find non-existent
        
        int fail_no_existed = 0;
        start = clock();
        for (int j = nums[i]; j < 2 * nums[i]; j++) {
            hm_set_entry* e = hm_set_get(&set, &j);
            if (e) {
                fail_no_existed++;
            }
        }
        end = clock();
        test_set_integrity(&set, &fail_cnt, tag++);
        check_res(fail_no_existed == 0, "the get function get entry when key is non-existent in set", &fail_cnt, tag++);
        print_run_time("GET NON-EXISTENT ENTRY", start, end, nums[i], nums[i]);
        
        hm_set_free(&set);

    }
    print_end("SET | STRESS | GET | TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER
}

void test_set_del_stress() {
    int fail_cnt = 0;
    int tag = 0;
    // the value of num cann't greater than 10^9
    int nums[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums) / sizeof(int);
    hm_set set;
    hm_set_init(&set, hash_int_1, cmp_int_up, free);

    print_run("SET | STRESS | DEL | TYPE: [INT]");


    for (int i = 0; i < cnt; i++) {
        // insert
        for (int j = 0; j < nums[i]; j++) {
            int* k = (int*)malloc(sizeof(int));
            *k = j;
            hm_set_insert(&set, k);
        }
        
        // del non-existent


        int fail_del_noexist = 0;
        clock_t start = clock();
        for (int j = nums[i]; j < 2 * nums[i]; j++) {
            hm_set_ret ret = hm_set_del(&set, &j);
            if (ret != hm_set_ret_none) {
                fail_del_noexist++;
            }
        }
        clock_t end = clock();
        test_set_integrity(&set, &fail_cnt, tag++);
        check_res(fail_del_noexist == 0, "del non-existent entry should return none", &fail_cnt, tag++);
        print_run_time("DEL NON-EXISTENT ENTRY", start, end, nums[i], nums[i]);
        
        // del existent
        
        int fail_del_exist = 0;
        start = clock();
        for (int j = 0; j < nums[i]; j++) {
            hm_set_ret ret = hm_set_del(&set, &j);
            if (ret != hm_set_ret_suc) {
                fail_del_exist++;
            }
        }
        end = clock();
        test_set_integrity(&set, &fail_cnt, tag++);
        check_res(fail_del_exist == 0, "del existent entry should return suc", &fail_cnt, tag++);
        print_run_time("DEL EXISTENT ENTRY", start, end, nums[i], nums[i]);
        
        hm_set_free(&set);

    }
    print_end("SET | STRESS | DEL | TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER
}

void test_set_clear_stress() {
    int fail_cnt = 0;
    int tag = 0;
    // the value of num cann't greater than 10^9
    int nums_free[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums_free) / sizeof(int);
    hm_set set;

    // clear the set including entry that have power to free the key
    hm_set_init(&set, hash_int_1, cmp_int_up, free);
    
    print_run("SET | STRESS | CLEAR (hm_free: free) | TYPE: [INT]");
    
    
    for (int i = 0; i < cnt; i++) {
        // insert
        for (int j = 0; j < nums_free[i]; j++) {
            int* k = (int*)malloc(sizeof(int));
            *k = j;
            hm_set_insert(&set, k);
        }
        
        // clear
        clock_t start = clock();
        
        hm_set_clear(&set);
        
        clock_t end = clock();
        test_set_integrity(&set, &fail_cnt, tag++);
        print_run_time("CLEAR", start, end, nums_free[i], nums_free[i]);
        
        hm_set_free(&set);
        
    }
    print_end("SET | STRESS | CLEAR (hm_free: free) | TYPE: [INT]\n", fail_cnt);
    HM_TEST_COUNTER
    
    
    fail_cnt = 0;
    // the value of num cann't greater than 10^9
    int nums_null[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    cnt = sizeof(nums_null) / sizeof(int);
    // clear the set including entry that don't have power to free the key
    hm_set_init(&set, hash_int_1, cmp_int_up, NULL);

    print_run("SET | STRESS | CLEAR (hm_free: NULL) | TYPE: [INT]");


    for (int i = 0; i < cnt; i++) {

        // insert
        int* keys = (int*)malloc(nums_null[i] * sizeof(int));
        
        for (int j = 0; j < nums_null[i]; j++) {
            keys[j] = j;
            hm_set_insert(&set, &keys[j]);
        }
        
        // clear
        clock_t start = clock();

        hm_set_clear(&set);

        clock_t end = clock();
        test_set_integrity(&set, &fail_cnt, tag++);
        print_run_time("CLEAR", start, end, nums_null[i], nums_null[i]);

        free(keys);
        hm_set_free(&set);

    }
    print_end("SET | STRESS | CLEAR (hm_free: NULL) | TYPE: [INT]\n", fail_cnt);
    HM_TEST_COUNTER
}


void test_set_free_stress() {
    int fail_cnt = 0;
    int tag = 0;
    // the value of num cann't greater than 10^9
    int nums_free[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums_free) / sizeof(int);
    hm_set set;
    
    // free the set including entry that have power to free the key
    hm_set_init(&set, hash_int_1, cmp_int_up, free);
    
    print_run("SET | STRESS | FREE (hm_free: free) | TYPE: [INT]");
    
    
    for (int i = 0; i < cnt; i++) {
        // insert
        for (int j = 0; j < nums_free[i]; j++) {
            int* k = (int*)malloc(sizeof(int));
            *k = j;
            hm_set_insert(&set, k);
        }
        
        // free
        clock_t start = clock();
        
        hm_set_free(&set);
        
        clock_t end = clock();
        test_set_integrity(&set, &fail_cnt, tag++);
        print_run_time("FREE", start, end, nums_free[i], nums_free[i]);
        
        
    }
    print_end("SET | STRESS | FREE (hm_free: free) | TYPE: [INT]\n", fail_cnt);
    HM_TEST_COUNTER
    
    
    fail_cnt = 0;
    // the value of num cann't greater than 10^9
    int nums_null[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    cnt = sizeof(nums_null) / sizeof(int);
    // free the set including entry that don't have power to free the key
    hm_set_init(&set, hash_int_1, cmp_int_up, NULL);
    
    print_run("SET | STRESS | FREE (hm_free: NULL) | TYPE: [INT]");
    
    
    for (int i = 0; i < cnt; i++) {
    
        // insert
        int* keys = (int*)malloc(nums_null[i] * sizeof(int));
        
        for (int j = 0; j < nums_null[i]; j++) {
            keys[j] = j;
            hm_set_insert(&set, &keys[j]);
        }
        
        // free
        clock_t start = clock();
    
        hm_set_free(&set);        
    
        clock_t end = clock();
        test_set_integrity(&set, &fail_cnt, tag++);
        print_run_time("FREE", start, end, nums_null[i], nums_null[i]);
    
        free(keys);
    }
    print_end("SET | STRESS | FREE (hm_free: NULL) | TYPE: [INT]\n", fail_cnt);
    HM_TEST_COUNTER

}

void test_empty_set_oper() {
    int fail_cnt = 0;
    int tag = 0;
    hm_set set;
    hm_set_init(&set, hash_int_1, cmp_int_up, free);

    print_run("SET | BOUNDARY | OPER EMPTY SET | TYPE: [INT]");

    int k = 0;
    // get
    check_res(hm_set_get(&set, &k) == NULL, "get on empty set should return `NULL`", &fail_cnt, tag++);

    // del
    k = 10;
    check_res(hm_set_del(&set, &k) == hm_set_ret_none, "del on empty set should return `NULL`", &fail_cnt, tag++);

    // iter

    hm_iter_set iter;
    hm_iter_set_init(&iter, &set);
    int loop_cnt = 0;
    while (hm_iter_set_has_next(&iter)) {
        hm_iter_set_next(&iter);
        loop_cnt++;
    }
    check_res(loop_cnt == 0, "iterator over empty set should yield zero entrys", &fail_cnt, tag++);


    print_end("SET | BOUNDARY | OPER EMPTY SET | TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER


}

void test_single_entry_oper() {
    int fail_cnt = 0;
    int tag = 0;
    hm_set set;
    hm_set_init(&set, hash_int_1, cmp_int_up, NULL);
    int k = 1;
    print_run("SET | BOUNDARY | OPER SINGLE ENTRY'S SET | TYPE: [INT]");

    // insert single entry and get

    hm_set_insert(&set, &k);
    hm_set_entry* e = hm_set_get(&set, &k);
    check_res(*(int*)(e->key) == k, "the key is wrong when run `set_get` on single entry's set", &fail_cnt, tag++);
    hm_set_free(&set);

    // insert single entry and delete it 

    hm_set_insert(&set, &k);
    check_res(hm_set_del(&set, &k) == hm_set_ret_suc, "del on single entry's set should return suc", &fail_cnt, tag++);
    check_res(set.size == 0, "set.size should be `zero` after del on single entry's set", &fail_cnt, tag++);
    test_set_integrity(&set, &fail_cnt, tag++);
    hm_set_free(&set);

    // insert two indetical keys

    hm_set_insert(&set, &k);
    int new_k = k;
    hm_set_insert(&set, &new_k);
    e = hm_set_get(&set, &new_k);
    check_res(e->key == &k, "the key should be old key when insert two indetical keys", &fail_cnt, tag++);

    check_res(set.size == 1, "the set.size should be 1 when insert two indetical keys", &fail_cnt, tag++);
    test_set_integrity(&set, &fail_cnt, tag++);
    hm_set_free(&set);


    print_end("SET | BOUNDARY | OPER SINGLE ENTRY'S SET | TYPE: [INT]", fail_cnt);
    HM_TEST_COUNTER
}

void function_test() {
    test_set_init();                                printf("\n");
    
    test_set_insert();                              printf("\n");
    
    test_iter_set();                                printf("\n");
    
    test_set_get();                                 printf("\n");
    
    test_set_del();                                 printf("\n");
    
    test_set_shrink();                              printf("\n");
    
    test_set_clear();                               printf("\n");
    
    test_set_free();                                printf("\n");

}


void boundary_test() {
    test_set_insert_same();                         printf("\n");

    test_empty_set_oper();                          printf("\n");

    test_single_entry_oper();                       printf("\n");

}

void stress_test() {
    test_set_insert_stress();                       printf("\n");
    
    test_set_get_stress();                          printf("\n");

    test_set_del_stress();                          printf("\n");

    test_set_clear_stress();                        printf("\n");

    test_set_free_stress();                         printf("\n");

}




int main()
{
    function_test();

    boundary_test();

    stress_test();

    return all_failure_num;
}