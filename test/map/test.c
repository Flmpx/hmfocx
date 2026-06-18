#include "../../include/hm_map.h"
#include "../../function/hash/hm_hash.h"
#include "../../function/cmp/hm_cmp.h"
#include "../hm_test.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

void test_map_intergrity(hm_map* map, int* fail_cnt) {

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
    
    check_res(others == 0, "TEST OF INTERGRITY: there are unkown tag of entry_status in map", fail_cnt);
    check_res(existed == map->size, "TEST OF INTERGRITY: the real size of existed entry in map is wrong", fail_cnt);
}

void test_map_init() {
    hm_map map_1;
    int fail_cnt = 0;
    print_run("MAP: initailize");
    hm_map_init(&map_1, hash_int_1, cmp_int_up, free, free);

    check_res(map_1.buckets == NULL, "the buckets isn't NULL", &fail_cnt);
    check_res(map_1.buckets_status == NULL, "the buckets_status isn't NULL", &fail_cnt);
    check_res(map_1.cmp == cmp_int_up, "pass `cmp_int_up` but map.cmp isn't `cmp_int_up`", &fail_cnt);
    check_res(map_1.hash == hash_int_1, "pass `hash_int_1` but map.hash isn't `hash_int_1`", &fail_cnt);
    check_res(map_1.free_key == free, "pass `free` for map.free_key but map.free_key isn't `free`", &fail_cnt);   
    check_res(map_1.free_val == free, "pass `free` for map.free_val but map.free_val isn't `free`", &fail_cnt);   
    check_res(map_1.len == 0, "map.len isn't 0", &fail_cnt);
    check_res(map_1.size == 0, "map.size isn't 0", &fail_cnt);
    
    hm_map map_2;
    hm_map_init(&map_2, hash_int_1, cmp_int_up, NULL, NULL);
    
    check_res(map_2.free_key == NULL, "pass `NULL` but map.free_key isn't `NULL`", &fail_cnt);
    check_res(map_2.free_val == NULL, "pass `NULL` but map.free_val isn't `NULL`", &fail_cnt);
    print_end("MAP: initailize", fail_cnt);
}

void test_map_insert() {
    int num = 100;
    hm_map map;
    hm_map_init(&map, hash_int_1, cmp_int_up, free, free);
    int fail_cnt = 0;
    int fail = 0;
    int flag[num];
    
    print_run("INSERT | TYPE K:[INT] V:[INT]");
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
    test_map_intergrity(&map, &fail_cnt);

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

    print_end("INSERT | TYPE K:[INT] V:[INT]", fail_cnt);

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

    print_run("ITERATOR | TYPE K:[INT] V:[INT]");
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

    hm_map_free(&map);

    print_end("ITERATOR | TYPE K:[INT] V:[INT]", fail_cnt);


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


    print_run("GET | TYPE K:[INT] V:[INT]");
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

    print_end("GET | TYPE K:[INT] V:[INT]", fail_cnt);

    


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


    print_run("CHANGE | TYPE K:[INT] V:[INT]");

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

    hm_map_free(&map);

    print_end("CHANGE | TYPE K:[INT] V:[INT]", fail_cnt);


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


    print_run("DEL | TYPE K:[INT] V:[INT]");

    // del half of map

    int fail_del = 0;
    for (int i = 0; i < num / 2; i++) {
        if (hm_map_del(&map, &i) != hm_map_ret_suc) {
            fail_del++;
        }
    }
    check_res(fail_del == 0, "the function should return `hm_map_ret_suc`, but it is not", &fail_cnt);
    test_map_intergrity(&map, &fail_cnt);

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
    test_map_intergrity(&map, &fail_cnt);

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
    print_end("DEL | TYPE K:[INT] V:[INT]", fail_cnt);



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

    print_run("SHRINK | TYPE K:[INT] V:[INT]");
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
    test_map_intergrity(&map, &fail_cnt);
    check_res(fail_shrink == 0, "it shouldn't to shrink map but it do", &fail_cnt);
    check_res(fail_no_shrink == 0, "it should to shrink map but it not do", &fail_cnt);
    

    // shrink empty map
    hm_map_free(&map);
    check_res(hm_map_shrink(&map) == hm_map_ret_none, "it shouldn't to shrink empty map but it do", &fail_cnt);


    print_end("SHRINK | TYPE K:[INT] V:[INT]", fail_cnt);


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

    print_run("CLEAR | TYPE K:[INT] V:[INT]");
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
    test_map_intergrity(&map, &fail_cnt);
    check_res(fail_exist == 0, "some entry still existed in map after clear this map", &fail_cnt);

    hm_map_free(&map);
    print_end("CLEAR | TYPE K:[INT] V:[INT]", fail_cnt);
    
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

    print_run("FREE | TYPE K:[INT] V:[INT]");

    hm_map_free(&map);

    check_res(map.size == 0, "map.size isn't 0 after free map", &fail_cnt);
    check_res(map.len == 0, "map.len isn't 0 after free map", &fail_cnt);
    test_map_intergrity(&map, &fail_cnt);
    
    // double free
    hm_map_free(&map);
    check_res(map.size == 0, "map.size isn't 0 after double free map", &fail_cnt);
    check_res(map.len == 0, "map.len isn't 0 after double free map", &fail_cnt);
    test_map_intergrity(&map, &fail_cnt);
    

    print_end("FREE | TYPE K:[INT] V:[INT]", fail_cnt);



}

void test_map_insert_random_stress() {
    // type : int
    int fail_cnt = 0;
    size_t nums[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000, 50000000};
    int cnt = sizeof(nums) / sizeof(size_t);
    print_run("INSERT RANDOM STRESS TEST | TYPE K:[INT] V:[INT]");
    srand(time(NULL));
    for (int i = 0; i < cnt; i++) {
        int* k = (int*)malloc(nums[i] * sizeof(int));
        check_res(k, "the keys in stress test is failed in alloct", &fail_cnt);
        if (k == NULL) {
            break;
        }
        int* v = (int*)malloc(nums[i] * sizeof(int));
        check_res(v, "the keys in stress test is failed in alloct", &fail_cnt);
        if (v == NULL) {
            free(k);
            break;
        }
        for (size_t j = 0; j < nums[i]; j++) {
            k[j] = rand();
            v[j] = rand();
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
        test_map_intergrity(&map, &fail_cnt);
        print_run_time("INSERT", start, end, nums[i], nums[i]);

        free(k);
        free(v);
        hm_map_free(&map);
    }
    print_end("INSERT RANDOM STRESS TEST | TYPE K:[INT] V:[INT]", fail_cnt);
}

void test_map_insert_same() {

    print_run("INSRET SAME ENTRY | TYPE K:[INT] V:[INT]");
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
    test_map_intergrity(&map, &fail_cnt);
    hm_map_free(&map);
    print_end("INSRET SAME ENTRY | TYPE K:[INT] V:[INT]", fail_cnt);


}

void test_map_get_stress() {

    int fail_cnt = 0;
    // the value of num cann't greater than 10^9
    int num = 10000000;
    hm_map map;
    hm_map_init(&map, hash_int_1, cmp_int_up, free, free);

    print_run("GET STRESS | TYPE K:[INT] V:[INT]");

    // insert
    for (int i = 0; i < num; i++) {
        int* k = (int*)malloc(sizeof(int));
        check_res(k, "the key in stress test is failed in alloct, stop this test", &fail_cnt);
        if (k == NULL) {
            break;
        }
        int* v = (int*)malloc(sizeof(int));
        if (v == NULL) {
            check_res(k, "the val in stress test is failed in alloct, stop this test", &fail_cnt);
            free(k);
            break;
        }
        *k = i;
        *v = rand();
        hm_map_insert(&map, k, v);
    }
    if (fail_cnt != 0) {
        hm_map_free(&map);
        print_end("GET STRESS | TYPE K:[INT] V:[INT]", fail_cnt);
        return;
    }
    
    // find existent
    
    clock_t start = clock();
    int fail_existed = 0;
    for (int i = 0; i < num; i++) {
        hm_entry* e = hm_map_get(&map, &i);
        if (e == NULL) {
            fail_existed++;
        }
    }
    clock_t end = clock();
    check_res(fail_existed == 0, "the get function get NULL when key is existent in map", &fail_cnt);
    print_run_time("GET EXISTENT ENTRY", start, end, num, num);
    
    // find non-existent
    
    int fail_no_existed = 0;
    start = clock();
    for (int i = num; i < 2 * num; i++) {
        hm_entry* e = hm_map_get(&map, &i);
        if (e) {
            fail_no_existed++;
        }
    }
    end = clock();
    
    check_res(fail_no_existed == 0, "the get function get entry when key is non-existent in map", &fail_cnt);
    print_run_time("GET NON-EXISTENT ENTRY", start, end, num, num);
    
    hm_map_free(&map);
    print_end("GET STRESS | TYPE K:[INT] V:[INT]", fail_cnt);
}

int main()
{
    test_map_init();            printf("\n");

    test_map_insert();          printf("\n");

    test_iter_map();            printf("\n");

    test_map_get();             printf("\n");

    test_map_change();          printf("\n");

    test_map_del();             printf("\n");

    test_map_shrink();          printf("\n");

    test_map_clear();           printf("\n");

    test_map_free();            printf("\n");

    test_map_insert_same();                         printf("\n");

    test_map_insert_random_stress();                printf("\n");

    test_map_get_stress();                          printf("\n");
    return 0;
}