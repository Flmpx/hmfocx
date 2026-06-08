#include "../../include/hm_map.h"
#include "../../function/hash/hm_hash.h"
#include "../../function/cmp/hm_cmp.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#define NUM 50
#define STR_LEN 52
#define LEN_PER_STR 21
#define MAX_NUM 1000000
static const char* all_str = "abcdefghigklmnnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
void get_num(int* a, int mod) {
    *a = rand() % mod;   
}

void get_str(char* str, int size) {
    int i;
    for (i = 0; i < size; i++) {
        str[i] = all_str[rand() % STR_LEN];
    }
    str[i] = '\0';
}

void print_map_status(hm_map* map) {
    printf("map: size = %zu, len = %zu\n", map->size, map->len);
}

void print_map(hm_map* map) {
    hm_iter_map iter;
    hm_iter_map_init(&iter, map);
    while (hm_iter_map_has_next(&iter)) {
        hm_entry* e = hm_iter_map_next(&iter);
        printf("k: %d, v: %s\n", *(int*)(e->key), (char*)(e->val));
    }
}

void test_map_init(hm_map* map) {
    printf("\n======Test map initialize=====\n");

    print_map_status(map);
    
    hm_map_init(map, hash_int_1, cmp_int_up, free, free);
    
    print_map_status(map);

}


void test_map_insert(hm_map* map, char string[NUM][LEN_PER_STR]) {

    printf("\n======Test map insert====\n");
    int suc = 0, fail = 0;
    print_map_status(map);
    
    for (int i = 0; i < NUM; i++) {
        int* k = (int*)malloc(sizeof(int));
        char* v = (char*)malloc(LEN_PER_STR * sizeof(char));
        get_num(k, NUM);
        get_str(v, LEN_PER_STR - 1);
        
        strcpy(string[*k], v);
        
        hm_map_ret ret = hm_map_insert(map, k, v);
        
        
        switch (ret) {
            case hm_map_ret_suc:
            suc++;
            break;
            
            case hm_map_ret_existed:
            free(k);
            // if key is existed, the old val will be replace and free
            break;
            
            default:
            fail++;
            break;
        }
    }
    printf("success : %d\n", suc);
    printf("failure : %d\n", fail);
    
    print_map_status(map);
    
}


void test_iter_map(hm_map* map) {

    printf("\n=====Test iterator of map=====\n");
    print_map_status(map);

    print_map(map);
    
    print_map_status(map);
}

void test_map_get(hm_map* map, char string[NUM][LEN_PER_STR]) {
    printf("\n=====Test map get=====\n");

    print_map_status(map);
    int suc = 0, fail = 0;
    
    for (int i = 0; i < NUM; i++) {
        hm_entry* e = hm_map_get(map, &i);
        if (e) {
            if (strcmp(e->val, string[i])) {
                fail++;
            } else {
                suc++;
            }
        }
    }
    
    printf("success : %d\n", suc);
    printf("failure : %d\n", fail);
    
    print_map_status(map);
}

void test_map_change(hm_map* map, char string[NUM][LEN_PER_STR]) {

    printf("\n=====Test map change=====\n");
    int suc = 0, fail = 0;
    
    print_map_status(map);
    
    for (int i = 0; i < NUM; i += 2) {
        hm_entry* e = hm_map_get(map, &i);
        if (e) {
            // let string stop in index of 4
            char* str = e->val;
            str[4] = '\0';
            string[i][4] = '\0';
            suc++;
            
            printf("change val of key %d\n", *(int*)(e->key));
            
        }
    }
    
    printf("success : %d\n", suc);
    printf("failure : %d\n", fail);
    
    print_map_status(map);




    
}

void test_map_after_change(hm_map* map, char string[NUM][LEN_PER_STR]) {
    printf("\n=====Test map after change=====\n");

    print_map_status(map);
    
    hm_iter_map iter;
    int suc = 0, fail = 0;
    hm_iter_map_init(&iter, map);
    while (hm_iter_map_has_next(&iter)) {
        hm_entry* e = hm_iter_map_next(&iter);
        if (e) {
            
            printf("k: %d, v: %s\n", *(int*)e->key, (char*)e->val);
            if (strcmp(e->val, string[*(int*)(e->key)])) {
                fail++;
            } else {
                suc++;
            }
        }
    }
    
    printf("success : %d\n", suc);
    printf("failure : %d\n", fail);
    
    print_map_status(map);
}

void test_map_del(hm_map* map, int del_index[NUM], int* del_cnt) {
    printf("\n=====Test map del=====\n");


    print_map_status(map);
    

    int suc = 0, fail = 0;
    for (int i = 1; i < NUM; i += 2) {
        hm_map_ret ret = hm_map_del(map, &i);
        if (ret == hm_map_ret_suc) {
            printf("del key of entry is %d\n", i);
            suc++;
            del_index[(*del_cnt)++] = i;
        }
    }
    
    printf("success : %d\n", suc);
    printf("failure : %d\n", fail);

    print_map_status(map);
}

void test_map_after_del(hm_map* map, int del_index[NUM], int del_cnt) {

    printf("\n=====Test map after del=====\n");

    print_map_status(map);
    int suc = 0, fail = 0;
    for (int i = 0; i < del_cnt; i++) {
        hm_entry* e = hm_map_get(map, del_index + i);
        if (e) {
            fail++;
        } else {
            suc++;
        }
    }
    printf("success : %d\n", suc);
    printf("failure : %d\n", fail);
    
    print_map_status(map);
}

void test_map_shrink(hm_map* map) {
    printf("\n=====Test map shrink=====\n");

    printf("shrink start: \n");
    print_map_status(map);
    for (int i = 0; i < 2; i++) {
        hm_map_ret ret = hm_map_shrink(map);
        if (ret == hm_map_ret_suc) {
            printf("shrink : suc\n");
        } else {
            printf("shrink : fail\n");
        }
        printf("the status of map after %d shrink\n", i + 1);
        print_map_status(map);
    }
}

double get_diff_time(clock_t s, clock_t e) {
    return (double)(e - s) / CLOCKS_PER_SEC;
}

void test_map_insert_stress(hm_map* map) {

    int STRESS_NUM[11] = {100, 500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    printf("\n=============STRESS INSERT  | TYPE : [STRING]=============\n");
    hm_map_init(map, hash_str_1, cmp_str_up, free, free);

    
    for (int i = 0; i < 0; i++) {
        clock_t start = clock();
        int loop_cnt = 0;
        int same_cnt = 0;
        int insert_suc = 0;
        int insert_fail = 0;
        printf("\n+++++++++STRESS INSERT NUMBER %d ++++++++++++++\n", STRESS_NUM[i]);
        for (int j = 0; j < STRESS_NUM[i]; j++) {
            char* k = (char*)malloc(LEN_PER_STR * sizeof(char));
            if (k == NULL) {
                break;
            }
            char* v = (char*)malloc(LEN_PER_STR * sizeof(char));
            if (v == NULL) {
                free(k);
                break;
            }
            get_str(k, LEN_PER_STR - 1); get_str(v, LEN_PER_STR - 1);
    
            hm_map_ret ret = hm_map_insert(map, k, v);
            
            if (ret == hm_map_ret_suc) {
                insert_suc++;
            } else if (ret == hm_map_ret_error) {
                insert_fail++;
            } else if (ret == hm_map_ret_existed) {
                same_cnt++;
                free(k);
            }
            loop_cnt++;
        }
    
        clock_t end = clock();
    
        printf("INSERT COST TIME: %lfs\n", get_diff_time(start, end));
        
        print_map_status(map);
        printf("loop_cnt : %d\nsame_cnt : %d\ninsert_suc : %d\ninsert_fail : %d\n", loop_cnt, same_cnt, insert_suc, insert_fail);

        start = clock();
        hm_map_free(map);
        end = clock();
        printf("FREE COST TIME: %lfs\n", get_diff_time(start, end));
    }
    printf("\n=========================STRESS INSERT  | TYPE : [STRING]===========================\n");


    printf("\n===============================STRESS INSERT  | TYPE : [INT]============================\n");
    hm_map_init(map, hash_int_1, cmp_int_up, free, free);

    
    for (int i = 0; i < 11; i++) {
        clock_t start = clock();
        int loop_cnt = 0;
        int same_cnt = 0;
        int insert_suc = 0;
        int insert_fail = 0;
        printf("\n+++++++++STRESS INSERT NUMBER %d ++++++++++++++\n", STRESS_NUM[i]);
        for (int j = 0; j < STRESS_NUM[i]; j++) {
            int* k = (int*)malloc(sizeof(int));
            if (k == NULL) {
                break;
            }
            int* v = (int*)malloc(sizeof(int));
            if (v == NULL) {
                free(k);
                break;
            }
            get_num(k, INT_MAX); get_num(v, INT_MAX);
    
            hm_map_ret ret = hm_map_insert(map, k, v);
            
            if (ret == hm_map_ret_suc) {
                insert_suc++;
            } else if (ret == hm_map_ret_error) {
                insert_fail++;
            } else if (ret == hm_map_ret_existed) {
                same_cnt++;
                free(k);
            }
            loop_cnt++;
        }
    
        clock_t end = clock();
    
        printf("INSERT COST TIME: %lfs\n", get_diff_time(start, end));
        
        print_map_status(map);
        printf("loop_cnt : %d\nsame_cnt : %d\ninsert_suc : %d\ninsert_fail : %d\n", loop_cnt, same_cnt, insert_suc, insert_fail);

        start = clock();
        hm_map_free(map);
        end = clock();
        printf("FREE COST TIME: %lfs\n", get_diff_time(start, end));
    }
    printf("\n=========================STRESS INSERT  | TYPE : [INT]===========================\n");


}

int main() 
{
    srand(time(NULL));
    int suc = 0, fail = 0;
    hm_map map;



/*
=============================MIN AND FUNCTION TEST=====================================
*/    
    
    
    /**
     * test for initialize map
     */
    test_map_init(&map);



    // use array of string to check the correctness of map
    char string[NUM][LEN_PER_STR];


    /**
     * test for map insert
     */
    test_map_insert(&map, string);


    /**
     * test for iterator of map
     */
    test_iter_map(&map);
    
    

    /**
     * 
     * verify the val of key in map by the function of `hm_map_get`
     */
    test_map_get(&map, string);

    
    /**
     * test for change some value of key in map
     */
    test_map_change(&map, string);

    /**
     * test the correctness of val of key after change some value
     */
    test_map_after_change(&map, string);


    /**
     * test for del
     */      
    
    // use the array of del_index to verify the existence of the key of del
    int del_index[NUM];
    int del_cnt = 0;
    test_map_del(&map, del_index, &del_cnt);
    /**
     * verify the correctness of the key , the function of del is wrong if key is deleted and the entry get by key is existed 
     */
    test_map_after_del(&map, del_index, del_cnt);


    /**
     * test for map_shrink
     */
    // shrink twice   
    test_map_shrink(&map);
    


    hm_map_free(&map);


/*

====================================STRESS TEST===================================

*/

    

    /**
     * stree insert 
     */
    test_map_insert_stress(&map);




    hm_map_free(&map);
    return 0;
}