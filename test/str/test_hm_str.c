/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */

#include "../../include/hm_str.h"
#include "../hm_test_tool.h"
#include <string.h>
#include <stdlib.h>

// This variable can record the total number of failures and it can be used as a return value to check whether the test passed
int all_failure_num = 0;

// use a macro to replace the repetitive code 
#define HM_TEST_COUNTER \
    all_failure_num += fail_cnt;


void test_str_integrity(hm_str* str, int* fail_cnt, int tag, size_t len) {
    check_res(str->len == len, "TEST OF INTEGRITY: str's len is unexpected", fail_cnt, tag);
    check_res(str->len <= str->capacity, "TEST OF INTEGRITY: str's `capacity` should be greater than `len`", fail_cnt, tag);
    /**
     * 1. capacity = 0, val == NULL
     * 2. capacity >= 17, val != NULL
     */
    check_res(str->capacity == 0 || str->capacity >= 17, "TEST OF INTEGRITY: str's capacity should be (c == 0 || c >= 16)", fail_cnt, tag);
    check_res(!(str->capacity == 0 && str->val != NULL), "TEST OF INTEGRITY: str's capacity is 0, but val isn't NULL", fail_cnt, tag);
    check_res(!(str->capacity >= 17 && str->val == NULL), "TEST OF INTEGRITY: str's capacity  >= 16, but val is NULL", fail_cnt, tag);

    if (str->capacity == 0) return;     // only capacity >= 17    
    size_t real_len = 0;
    const char* s = str->val;
    int cnt = str->len;
    while (*s++ && cnt--) {
        real_len++;
    }
    check_res(*(--s) == '\0', "TEST OF INTEGRITY: the end of string should be '\\0'", fail_cnt, tag++);
    check_res(real_len == str->len, "TEST OF INTEGRITY: the real len is different from the str's len", fail_cnt, tag);

}



// every test function ...

void test_str_init() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("STR | FUNC | INIT");

    hm_str str;
    hm_str_init(&str);

    // check
    check_res(str.capacity == 0, "str's capacity should be NULL", &fail_cnt, tag++);
    check_res(str.len == 0, "str's len should be 0", &fail_cnt, tag++);
    check_res(str.val == NULL, "str's val should be NULL", &fail_cnt, tag++);
    test_str_integrity(&str, &fail_cnt, tag++, 0);

    hm_str_free(&str);

    print_end("STR | FUNC | INIT", fail_cnt);
    HM_TEST_COUNTER
}

void test_str_init_reserve() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("STR | FUNC | INIT RESERVE");
    
    int capacity = 64;
    hm_str str;
    hm_str_init_reserve(&str, capacity);
    
    // check
    check_res(str.capacity == capacity, "str's capacity is unexpected", &fail_cnt, tag++);
    check_res(str.len == 0, "str's len should be 0", &fail_cnt, tag++);
    check_res(str.val != NULL, "str's val shouldn't be NULL", &fail_cnt, tag++);
    test_str_integrity(&str, &fail_cnt, tag++, 0);
    hm_str_free(&str);

    int s_capacity = 0;
    int min_capacity = 17;
    hm_str_init_reserve(&str, s_capacity);

    // check
    check_res(str.capacity == min_capacity, "str's should be min_capacity", &fail_cnt, tag++);
    check_res(str.len == 0, "str's len should be 0", &fail_cnt, tag++);
    check_res(str.val != NULL, "str's val shouldn't be NULL", &fail_cnt, tag++);
    test_str_integrity(&str, &fail_cnt, tag++, 0);
    hm_str_free(&str);

    
    print_end("STR | FUNC | INIT RESERVE", fail_cnt);
    HM_TEST_COUNTER
    
}

void test_str_append() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("STR | FUNC | APPEND");

    hm_str str;
    hm_str_init(&str);

    const char* strings[] = {"a", "bb", "ccc", "dddd", "eeeee", "ffffff", "ggggggg"};
    int num = sizeof(strings) / sizeof(const char*);

    int repeat_cnt = 64;
    int len_every_repeat = 0;
    // get the length of every repeat append
    for (int i = 0; i < num; i++) {
        len_every_repeat += strlen(strings[i]);
    }

    int fail = 0;
    // append
    for (int i = 0; i < repeat_cnt; i++) {
        
        for (int j = 0; j < num; j++) {
            if (hm_str_append(&str, strings[j]) != hm_str_ret_suc) {
                fail++;
            }
        }

    }
    test_str_integrity(&str, &fail_cnt, tag++, repeat_cnt * len_every_repeat);
    check_res(fail == 0, "append function should return suc", &fail_cnt, tag++);

    // verify
    const char* s = str.val;
    size_t now_len = 0;
    fail = 0;
    for (int i = 0; i < repeat_cnt; i++) {

        for (int j = 0; j < num; j++) {
            int len = strlen(strings[j]);
            if (strncmp(s + now_len, strings[j], len)) {
                fail++;
            }
            now_len += len;
        }
    }
    check_res(fail == 0, "the some string in str is wrong", &fail_cnt, tag++);

    hm_str_free(&str);

    print_end("STR | FUNC | APPEND", fail_cnt);
    HM_TEST_COUNTER
}


void test_str_get() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("STR | FUNC | GET");

    hm_str str;
    hm_str_init(&str);

    const char* strings[] = {"a", "bb", "ccc", "dddd", "eeeee", "ffffff", "ggggggg"};
    int num = sizeof(strings) / sizeof(const char*);

    int repeat_cnt = 32;
    int len_every_repeat = 0;
    // get the length of every repeat append
    for (int i = 0; i < num; i++) {
        len_every_repeat += strlen(strings[i]);
    }
    char compare_str[repeat_cnt * len_every_repeat + 100];
    compare_str[0] = '\0';

    // append
    for (int i = 0; i < repeat_cnt; i++) {
        for (int j = 0; j < num; j++) {
            hm_str_append(&str, strings[j]);
            strcat(compare_str, strings[j]);
        }
    }
    size_t capacity = hm_str_capacity(&str);

    // get
    size_t l = hm_str_len(&str);
    // valid index
    int fail_null = 0;
    int fail_diff = 0;
    for (int i = 0; i <= l; i++) {
        const char* index_s = hm_str_get(&str, i);
        if (index_s == NULL) {
            fail_null++;
        } else if (strcmp(index_s, compare_str + i)) {
            fail_diff++;
        }
    }
    check_res(str.capacity == capacity, "the capacity is wrong after get", &fail_cnt, tag++);
    test_str_integrity(&str, &fail_cnt, tag++, repeat_cnt * len_every_repeat);
    check_res(fail_null == 0, "the string get by `get` should be valid when index is valid", &fail_cnt, tag++);
    check_res(fail_diff == 0, "the string get by `get` is wrong", &fail_cnt, tag++);
    
    // invalid index
    int fail_no_null = 0;
    for (int i = l + 1; i < l * 2; i++) {
        const char* index_s = hm_str_get(&str, i);
        if (index_s != NULL) {
            fail_no_null++;
        }
    }
    check_res(str.capacity == capacity, "the capacity is wrong after get", &fail_cnt, tag++);
    test_str_integrity(&str, &fail_cnt, tag++, repeat_cnt * len_every_repeat);
    check_res(fail_no_null == 0, "the string get by `get` should be invalid when index is invalid", &fail_cnt, tag++);
    
    hm_str_free(&str);


    print_end("STR | FUNC | GET", fail_cnt);
    HM_TEST_COUNTER
}

void test_str_pop() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("STR | FUNC | POP");

    hm_str str;
    hm_str_init(&str);

    const char* strings[] = {"a", "bb", "ccc", "dddd", "eeeee", "ffffff", "ggggggg"};
    int num = sizeof(strings) / sizeof(const char*);

    int repeat_cnt = 64;
    // append
    for (int i = 0; i < repeat_cnt; i++) {
        for (int j = 0; j < num; j++) {
            hm_str_append(&str, strings[j]);
        }
    }
    // pop
    char* s = hm_str_pop(&str);
    test_str_integrity(&str, &fail_cnt, tag++, 0);

    // verify
    size_t now_len = 0;
    int fail = 0;
    for (int i = 0; i < repeat_cnt; i++) {

        for (int j = 0; j < num; j++) {
            int len = strlen(strings[j]);
            if (strncmp(s + now_len, strings[j], len)) {
                fail++;
            }
            now_len += len;
        }
    }
    check_res(fail == 0, "the pop string is wrong", &fail_cnt, tag++);

    hm_str_free(&str);
    free(s);

    print_end("STR | FUNC | POP", fail_cnt);
    HM_TEST_COUNTER
}

void test_str_clear() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("STR | FUNC | CLEAR");

    hm_str str;
    hm_str_init(&str);

    const char* strings[] = {"a", "bb", "ccc", "dddd", "eeeee", "ffffff", "ggggggg"};
    int num = sizeof(strings) / sizeof(const char*);

    int repeat_cnt = 64;
    // append
    for (int i = 0; i < repeat_cnt; i++) {
        for (int j = 0; j < num; j++) {
            hm_str_append(&str, strings[j]);
        }
    }
    size_t capacity = hm_str_capacity(&str);

    // clear
    hm_str_clear(&str);
    check_res(str.capacity == capacity, "the capacity is wrong after clear", &fail_cnt, tag++);
    test_str_integrity(&str, &fail_cnt, tag++, 0);
    
    // double clear
    hm_str_clear(&str);
    check_res(str.capacity == capacity, "the capacity is wrong after double clear", &fail_cnt, tag++);
    test_str_integrity(&str, &fail_cnt, tag++, 0);

    hm_str_free(&str);

    print_end("STR | FUNC | CLEAR", fail_cnt);
    HM_TEST_COUNTER
}

void test_str_free() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("STR | FUNC | FREE");

    hm_str str;
    hm_str_init(&str);

    const char* strings[] = {"a", "bb", "ccc", "dddd", "eeeee", "ffffff", "ggggggg"};
    int num = sizeof(strings) / sizeof(const char*);

    int repeat_cnt = 64;
    // append
    for (int i = 0; i < repeat_cnt; i++) {
        for (int j = 0; j < num; j++) {
            hm_str_append(&str, strings[j]);
        }
    }

    // free
    hm_str_free(&str);
    test_str_integrity(&str, &fail_cnt, tag++, 0);
    
    // double free
    hm_str_free(&str);
    test_str_integrity(&str, &fail_cnt, tag++, 0);

    hm_str_free(&str);

    print_end("STR | FUNC | FREE", fail_cnt);
    HM_TEST_COUNTER
}

void test_str_shrink() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("STR | FUNC | SHRINK");
    
    hm_str str;
    hm_str_init(&str);
    
    const char* strings[] = {"a", "bb", "ccc", "dddd", "eeeee", "ffffff", "ggggggg"};
    int num = sizeof(strings) / sizeof(const char*);
    
    int repeat_cnt = 64;
    // append
    for (int i = 0; i < repeat_cnt; i++) {
        for (int j = 0; j < num; j++) {
            hm_str_append(&str, strings[j]);
        }
    }

    // clear
    hm_str_clear(&str);
    // append
    for (int i = 0; i < repeat_cnt / 10; i++) {
        for (int j = 0; j < num; j++) {
            hm_str_append(&str, strings[j]);
        }
    }

    size_t len = hm_str_len(&str);

    int fail_shrink = 0;
    int fail_no_shrink = 0;
    int fail_len = 0;
    int cnt = 10;
    int min_capacity = 17;
    for (int i = 0; i < 10; i++) {
        size_t c = hm_str_capacity(&str), l = hm_str_len(&str);
        hm_str_ret ret = hm_str_shrink(&str);
        if (c < 2 * min_capacity || l >= c / 2) {
            // shouldn't shrink
            if (ret != hm_str_ret_none) {
                fail_shrink++;
            }
        } else {
            // should shrink
            if (ret != hm_str_ret_suc) {
                fail_no_shrink++;
            }
        }
        if (str.len != len) {
            // len should be fixed
            fail_len++;
        }
    }
    check_res(fail_len == 0, "len is wrong when shrink str", &fail_cnt, tag++);
    check_res(fail_shrink == 0, "it shouldn't shrink str but it do", &fail_cnt, tag++);
    check_res(fail_no_shrink == 0, "it should to shrink but it not do", &fail_cnt, tag++);
    
    hm_str_free(&str);

    
    print_end("STR | FUNC | SHRINK", fail_cnt);
    HM_TEST_COUNTER
    
}


void function_test() {
    test_str_init();                                                                        printf("\n");

    test_str_init_reserve();                                                                printf("\n");

    test_str_append();                                                                      printf("\n");

    test_str_get();                                                                         printf("\n");

    test_str_pop();                                                                         printf("\n");

    test_str_clear();                                                                       printf("\n");

    test_str_free();                                                                        printf("\n");

    test_str_shrink();                                                                      printf("\n");

}

void boundary_test() {
    
}

void stress_test() {
    
}

int main()
{
    // Group the test roughly
    function_test();
    boundary_test();
    stress_test();

    return all_failure_num;
}

