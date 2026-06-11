#include "../../include/hm_map.h"
#include "../../function/hash/hm_hash.h"
#include "../../function/cmp/hm_cmp.h"
#include "../hm_test.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

// get the real size of buckets
#if defined(_WIN32) || defined(_WIN64)
    #include <malloc.h>
    #define GET_ALLOCATED_SIZE(ptr) _mszie(ptr)
#elif defined(__linux__)
    #include <malloc.h>
    #define GET_ALLOCATED_SIZE(ptr) malloc_usable_size(ptr)
#else 
    #include <malloc/malloc.h>
    #define GET_ALLOCATED_SIZE(ptr) malloc_size(ptr)
#endif

void test_map_intergrity(hm_map* map, int* fail_cnt) {
    // entrys
    int len = GET_ALLOCATED_SIZE(map->buckets) / sizeof (hm_entry);
    check_res(len == map->len, "TEST OF INTERGRITY: the real size of entry is wrong", fail_cnt);
    
    
    // entrys_status
    len = GET_ALLOCATED_SIZE(map->buckets_status) / sizeof(hm_entry_status);
    check_res(len == map->len, "TEST OF INTERGRITY: the real size of entry_status is wrong", fail_cnt);

    // size [exist | none | del]

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
    
    check_res(others == 0, "TEST INTERGRITY: there are unkown tag of entry_status in map", fail_cnt);
    check_res(existed == map->size, "TEST INTERGRITY: the real size of existed entry in map is wrong", fail_cnt);
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

int main()
{
    test_map_init();            printf("\n");
}