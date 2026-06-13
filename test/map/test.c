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

    check_res(fail_invalid_k != 0, "the k got by iter_map is invalid ", &fail_cnt);
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

}

void test_map_del() {

}

void test_map_shrink() {

}

int main()
{
    test_map_init();            printf("\n");

    test_map_insert();          printf("\n");

    test_iter_map();            printf("\n");

    test_map_get();             printf("\n");
    return 0;
}