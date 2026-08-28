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

    check_res(str->val != NULL, "TEST OF INTEGRITY: str's val shouldn't be NULL", fail_cnt, tag);
    check_res(str->val[str->len] == '\0', "TEST OF INTEGRITY: str's character at the index of str's len isn't '\\0'", fail_cnt, tag);

}



// every test function ...

void test_str_init() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("STR | FUNC | INIT");

    hm_str str;
    hm_str_init(&str);

    // check
    check_res(str.capacity == 0, "str's capacity should be 0", &fail_cnt, tag++);
    check_res(str.len == 0, "str's len should be 0", &fail_cnt, tag++);
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
    hm_str_init_reserve(&str, s_capacity);

    // check
    check_res(str.capacity == s_capacity, "str's should be min_capacity", &fail_cnt, tag++);
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
            if (hm_str_append(&str, strings[j], strlen(strings[j])) != hm_str_ret_suc) {
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
            hm_str_append(&str, strings[j], strlen(strings[j]));
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
            hm_str_append(&str, strings[j], strlen(strings[j]));
        }
    }
    // pop
    char* s = hm_str_pop(&str);
    // it can't test integrity because str can't be used after pop

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
            hm_str_append(&str, strings[j], strlen(strings[j]));
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
            hm_str_append(&str, strings[j], strlen(strings[j]));
        }
    }

    // free
    hm_str_free(&str);              // use valgrid to check memory leak
    // can't use after free
    

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
            hm_str_append(&str, strings[j], strlen(strings[j]));
        }
    }

    // clear
    hm_str_clear(&str);
    // append
    for (int i = 0; i < repeat_cnt / 10; i++) {
        for (int j = 0; j < num; j++) {
            hm_str_append(&str, strings[j], strlen(strings[j]));
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

void test_append_empty_string_in_str() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("STR | BOUNDARY | APPEND EMPTY STRING");

    hm_str str;
    hm_str_init(&str);

    const char* string = "";
    int cnt = 10000;

    int fail = 0;
    for (int i = 0; i < cnt; i++) {
        if (hm_str_append(&str, string, 0) != hm_str_ret_suc) {
            fail++;
        }
    }
    check_res(fail == 0, "append function should return suc when append many empty string", &fail_cnt, tag++);
    check_res(str.capacity <= 17, "the str's capacity shouldn't be big when append many empty string", &fail_cnt, tag++);
    test_str_integrity(&str, &fail_cnt, tag++, 0);

    const char* s = hm_str_get(&str, 0);
    check_res(strcmp(s, string) == 0, "the string should be empty string when append many empty string", &fail_cnt, tag++);

    hm_str_free(&str);
    

    print_end("STR | BOUNDARY | APPEND EMPTY STRING", fail_cnt);
    HM_TEST_COUNTER
}

void test_append_empty_ch_in_str() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("STR | BOUNDARY | APPEND EMPTY CHARACTER");
    
    hm_str str;
    hm_str_init(&str);
    
    char ch = '\0';
    int cnt = 10000;
    
    int fail = 0;
    for (int i = 0; i < cnt; i++) {
        if (hm_str_append_ch(&str, ch) != hm_str_ret_suc) {
            fail++;
        }
    }
    check_res(fail == 0, "append function should return suc when append many empty characters", &fail_cnt, tag++);
    test_str_integrity(&str, &fail_cnt, tag++, cnt);
    
    // verify
    fail = 0;
    const char* s = hm_str_get(&str, 0);
    for (int i = 0; i < cnt; i++) {
        if (s[i] != '\0') {
            fail++;
        }
    }
    check_res(fail == 0, "the val should include many empty characters", &fail_cnt, tag++);
    
    hm_str_free(&str);
    
    
    print_end("STR | BOUNDARY | APPEND EMPTY CHARACTER", fail_cnt);
    HM_TEST_COUNTER

}

void test_oper_empty_str() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("STR | BOUNDARY | OPER EMTPY STR");
    
    hm_str str;
    
    // get
    hm_str_init(&str);

    check_res(strcmp(hm_str_get(&str, 0), "") == 0, "get on empty str should return empty string", &fail_cnt, tag++);
    test_str_integrity(&str, &fail_cnt, tag++, 0);
    hm_str_free(&str);
    
    // shrink
    hm_str_init(&str);

    check_res(hm_str_shrink(&str) == hm_str_ret_none, "shrink on a empty str(capacity == 17) should return none", &fail_cnt, tag++);
    test_str_integrity(&str, &fail_cnt, tag++, 0);
    hm_str_free(&str);
    
    
    // append
    hm_str_init(&str);
    
    char* string = "abcdefg";
    hm_str_append(&str, string, strlen(string));
    check_res(strcmp(hm_str_get(&str, 0), string) == 0, "the string is wrong when append a string on a empty str", &fail_cnt, tag++);
    test_str_integrity(&str, &fail_cnt, tag++, strlen(string));
    hm_str_free(&str);


    // append character
    hm_str_init(&str);    // let it to empty, not no-capacity
    
    char ch = 'A';
    hm_str_append_ch(&str, ch);
    check_res(*hm_str_get(&str, 0) == ch, "the string is wrong when append a character on a empty str", &fail_cnt, tag++);
    test_str_integrity(&str, &fail_cnt, tag++, 1);
    hm_str_free(&str);
    
    
    // clear
    hm_str_init(&str);

    hm_str_clear(&str);
    test_str_integrity(&str, &fail_cnt, tag++, 0);
    hm_str_free(&str);
    
    // pop
    hm_str_init(&str);
    
    char* s = hm_str_pop(&str);
    check_res(strcmp(s, "") == 0, "the pop on empty str should return empty string", &fail_cnt, tag++);
    // can't use after pop
    free(s);
    
    
    print_end("STR | BOUNDARY | OPER EMPTY STR", fail_cnt);
    HM_TEST_COUNTER
    
}



void test_str_append_stress() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("STR | STRESS | APPEND");

    hm_str str;
    hm_str_init(&str);

    const char* string = "abcdefghigklmnopqrstuvwxyz1234567890, Hello, I'm Flmpx, Could you give me a star. This a library about hm. HM is my flag";

    size_t len_every = strlen(string);

    size_t nums[] = {1000000, 2000000, 5000000, 10000000, 20000000};
    int cnt = sizeof(nums) / sizeof(size_t);
    for (int i = 0; i < cnt; i++) {
        hm_str str;
        hm_str_init(&str);
        int fail = 0;
        clock_t start = clock();
        for (int j = 0; j < nums[i]; j++) {
            if (hm_str_append(&str, string, strlen(string)) != hm_str_ret_suc) {
                fail++;
            }
        }
        clock_t end = clock();
        print_run_time("APPEND", start, end, nums[i] * len_every, nums[i]);
        check_res(fail == 0, "append should return suc", &fail_cnt, tag++);
        test_str_integrity(&str, &fail_cnt, tag++, len_every * nums[i]);
        // verify
        fail = 0;
        for (int j = 0; j < nums[i]; j++) {
            const char* s = hm_str_get(&str, j * len_every);
            if (strncmp(s, string, len_every)) {
                fail++;
            }
        }
        check_res(fail == 0, "the string in str is wrong after append stressful", &fail_cnt, tag++);
        hm_str_free(&str);
    }
    

    print_end("STR | STRESS | APPEND", fail_cnt);
    HM_TEST_COUNTER
}


void test_str_append_with_reserve_stress() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("STR | STRESS | APPEND WITH RESERVE");

    hm_str str;
    hm_str_init(&str);

    const char* string = "abcdefghigklmnopqrstuvwxyz1234567890, Hello, I'm Flmpx, Could you give me a star. This a library about hm. HM is my flag";

    size_t len_every = strlen(string);

    size_t nums[] = {1000000, 2000000, 5000000, 10000000, 20000000};
    int cnt = sizeof(nums) / sizeof(size_t);
    for (int i = 0; i < cnt; i++) {
        hm_str str;
        hm_str_init_reserve(&str, nums[i] * len_every);
        int fail = 0;
        clock_t start = clock();
        for (int j = 0; j < nums[i]; j++) {
            if (hm_str_append(&str, string, strlen(string)) != hm_str_ret_suc) {
                fail++;
            }
        }
        clock_t end = clock();
        print_run_time("APPEND", start, end, nums[i] * len_every, nums[i]);
        check_res(fail == 0, "append should return suc", &fail_cnt, tag++);
        test_str_integrity(&str, &fail_cnt, tag++, len_every * nums[i]);
        // verify
        fail = 0;
        for (int j = 0; j < nums[i]; j++) {
            const char* s = hm_str_get(&str, j * len_every);
            if (strncmp(s, string, len_every)) {
                fail++;
            }
        }
        check_res(fail == 0, "the string in str is wrong after append with reserve stressful", &fail_cnt, tag++);
        hm_str_free(&str);
    }
    

    print_end("STR | STRESS | APPEND WITH RESERVE", fail_cnt);
    HM_TEST_COUNTER
}

void test_str_append_ch_stress() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("STR | STRESS | APPEND CHARACTER");

    hm_str str;
    hm_str_init(&str);

    char chs[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 
                  'H', 'I', 'G', 'K', 'L', 'M', 'N', 
                  'O', 'P', 'Q',      'R', 'S', 'T', 
                  'U', 'V', 'W',      'X', 'Y', 'Z'};

    size_t ch_num = sizeof(chs) / sizeof(char);

    size_t nums[] = {1000000, 5000000, 10000000, 50000000, 100000000};
    int cnt = sizeof(nums) / sizeof(size_t);
    for (int i = 0; i < cnt; i++) {
        hm_str str;
        hm_str_init(&str);
        int fail = 0;
        clock_t start = clock();
        for (int j = 0; j < nums[i]; j++) {
            for (int k = 0; k < ch_num; k++) {
                if (hm_str_append_ch(&str, chs[k]) != hm_str_ret_suc) {
                    fail++;
                }
            }
        }
        clock_t end = clock();
        print_run_time("APPEND", start, end, nums[i] * ch_num, nums[i] * ch_num);
        check_res(fail == 0, "append should return suc", &fail_cnt, tag++);
        test_str_integrity(&str, &fail_cnt, tag++, ch_num * nums[i]);

        // verify
        fail = 0;
        const char* val = hm_str_get(&str, 0);
        for (size_t j = 0; j < nums[i] * ch_num; j++) {
            if (val[j] != chs[j % ch_num]) {
                fail++;
            }
        }
        check_res(fail == 0, "the string in str is wrong after append characters stressful", &fail_cnt, tag++);
        hm_str_free(&str);
    }
    

    print_end("STR | STRESS | APPEND CHARACTER", fail_cnt);
    HM_TEST_COUNTER
}


void test_str_append_ch_with_reserve_stress() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("STR | STRESS | APPEND CHARACTER WITH RESERVE");

    hm_str str;
    hm_str_init(&str);

    char chs[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 
                  'H', 'I', 'G', 'K', 'L', 'M', 'N', 
                  'O', 'P', 'Q',      'R', 'S', 'T', 
                  'U', 'V', 'W',      'X', 'Y', 'Z'};

    size_t ch_num = sizeof(chs) / sizeof(char);

    size_t nums[] = {1000000, 5000000, 10000000, 50000000, 100000000};
    int cnt = sizeof(nums) / sizeof(size_t);
    for (int i = 0; i < cnt; i++) {
        hm_str str;
        hm_str_init_reserve(&str, nums[i] * cnt);
        int fail = 0;
        clock_t start = clock();
        for (int j = 0; j < nums[i]; j++) {
            for (int k = 0; k < ch_num; k++) {
                if (hm_str_append_ch(&str, chs[k]) != hm_str_ret_suc) {
                    fail++;
                }
            }
        }
        clock_t end = clock();
        print_run_time("APPEND", start, end, nums[i] * ch_num, nums[i] * ch_num);
        check_res(fail == 0, "append should return suc", &fail_cnt, tag++);
        test_str_integrity(&str, &fail_cnt, tag++, ch_num * nums[i]);

        // verify
        fail = 0;
        const char* val = hm_str_get(&str, 0);
        for (size_t j = 0; j < nums[i] * ch_num; j++) {
            if (val[j] != chs[j % ch_num]) {
                fail++;
            }
        }
        check_res(fail == 0, "the string in str is wrong after append characters with reserve stressful", &fail_cnt, tag++);
        hm_str_free(&str);
    }
    

    print_end("STR | STRESS | APPEND CHARACTER WITH RESERVE", fail_cnt);
    HM_TEST_COUNTER
}

void test_str_append_ch() {
    int fail_cnt = 0;
    int tag = 0;
    print_run("STR | FUNC | APPEND CHARACTER");

    hm_str str;
    hm_str_init(&str);

    char chs[] = {'I', 'L', 'X', 'L', 'B', 'M', 'H', 'L', 'H', ','};
    int num = sizeof(chs) / sizeof(char);

    int repeat_cnt = 64;
    
    int fail = 0;
    // append character
    for (int i = 0; i <repeat_cnt; i++) {

        for (int j = 0; j < num; j++) {
            if (hm_str_append_ch(&str, chs[j]) != hm_str_ret_suc) {
                fail++;
            }
        }
    }
    test_str_integrity(&str, &fail_cnt, tag++, num * repeat_cnt);
    check_res(fail == 0, "append character function should return suc", &fail_cnt, tag++);

    // verify
    const char* val = hm_str_get(&str, 0);  // get the string
    fail = 0;

    for (int i = 0; i < repeat_cnt * num; i++) {
        if (val[i] != chs[i % num]) {
            fail++;
        }
    }
    check_res(fail == 0, "the string in str is wrong after append character", &fail_cnt, tag++);

    hm_str_free(&str);


    print_end("STR | FUNC | APPEND CHARACTER", fail_cnt);
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

    test_str_append_ch();                                                                   printf("\n");

}

void boundary_test() {
    test_append_empty_string_in_str();                                                      printf("\n");

    test_append_empty_ch_in_str();                                                          printf("\n");

    test_oper_empty_str();                                                                  printf("\n");    
    
}

void stress_test() {
    test_str_append_stress();                                                               printf("\n");   

    test_str_append_with_reserve_stress();                                                  printf("\n");

    test_str_append_ch_stress();                                                            printf("\n");

    test_str_append_ch_with_reserve_stress();                                               printf("\n");
}

int main()
{
    // Group the test roughly
    function_test();
    boundary_test();
    stress_test();

    return all_failure_num;
}

