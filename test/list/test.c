/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */
#include "../../include/hm_list.h"
#include "../../function/cmp/hm_cmp.h"
#include "../hm_test.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void test_list_integrity(hm_list* list, int* fail_cnt) {
    int cnt;

    // -->
    cnt = 0;
    hm_listnode* cur = list->head;
    while (cur) {
        cur = cur->next;
        cnt++;
    }
    check_res(cnt == list->size, "TEST OF INTEGRITY: the next of listnode is wrong", fail_cnt);
    
    // <--
    cnt = 0;
    cur = list->tail;
    while (cur) {
        cur = cur->prev;
        cnt++;
    }
    
    check_res(cnt == list->size, "TEST OF INTEGRITY: the prev of listnode is wrong", fail_cnt);

}

void test_list_init() {
    
    hm_list list_1;
    int fail_cnt = 0;
    print_run("LIST | FUNC | INIT");
    hm_list_init(&list_1, free);
    
    check_res(list_1.free == free, "Pass `free` but list.free isn't `free`", &fail_cnt);
    check_res(list_1.head == NULL, "list.head isn't `NULL`", &fail_cnt);
    check_res(list_1.tail == NULL, "list.tail isn't `NULL`", &fail_cnt);
    check_res(list_1.size == 0, "list.size isn't 0", &fail_cnt);
    test_list_integrity(&list_1, &fail_cnt);
    
    hm_list list_2;
    hm_list_init(&list_2, NULL);
    
    
    check_res(list_2.free == NULL, "Pass `NULL` but list.free isn't `NULL`", &fail_cnt);
    test_list_integrity(&list_2, &fail_cnt);
    print_end("LIST | FUNC | INIT", fail_cnt);
    
}




void test_list_insert_head() {
    
    hm_list list;
    hm_list_init(&list, free);
    int fail_cnt = 0;
    int num = 100;
    int flag[num];
    int fail;
    int cnt;
    hm_listnode* curr;
    
    // insert head
    print_run("LIST | FUNC | INSERT HEAD | TYPE: [INT]");
    fail = 0;
    for (int i = 0; i < num; i++) {
        flag[i] = i * 100;
        int* v = (int*)malloc(sizeof(int));
        *v = flag[i];
        if (hm_list_insert_head(&list, v) != hm_list_ret_suc) {
            fail++;
        }
    }
    
    check_res(fail == 0, "process of insert head is error", &fail_cnt);
    check_res(list.size == num, "list.size is wrong", &fail_cnt);
    
    // test the integrity of list
    test_list_integrity(&list, &fail_cnt);

    // verify 
    
    fail = 0;
    cnt = num;
    curr = list.head;
    
    while (curr) {
        if (*(int*)(curr->val) != flag[--cnt]) {
            fail++;
        }
        curr = curr->next;
    }
    check_res(cnt == 0, "real size of list is wrong", &fail_cnt);
    check_res(fail == 0, "data in list is wrong", &fail_cnt);
    
    hm_list_free(&list);
    print_end("LIST | FUNC | INSERT HEAD | TYPE: [INT]", fail_cnt);
    
}



void test_list_insert_tail() {
    
    hm_list list;
    hm_list_init(&list, free);
    int fail_cnt = 0;
    int num = 100;
    int flag[num];
    int fail;
    int cnt;
    hm_listnode* curr;
    
    // insert tail
    print_run("LIST | FUNC | INSERT TAIL | TYPE: [INT]");
    fail = 0;
    for (int i = 0; i < num; i++) {
        flag[i] = i * 10;
        int* v = (int*)malloc(sizeof(int));
        *v = flag[i];
        if (hm_list_insert_tail(&list, v) != hm_list_ret_suc) {
            fail++;
        }
    }
    check_res(fail == 0, "process of insert tail is error", &fail_cnt);
    check_res(list.size == num, "list.size is wrong", &fail_cnt);
    
    // test the integrity of list
    test_list_integrity(&list, &fail_cnt);
    
    // verify
    fail = 0;
    curr = list.head;
    cnt = 0;
    while (curr) {
        if (*(int*)(curr->val) != flag[cnt++]) {
            fail++;
        }
        curr = curr->next;
    }
    check_res(cnt == num, "real size of list is wrong", &fail_cnt);
    check_res(fail == 0, "data in list is wrong", &fail_cnt);
    
    hm_list_free(&list);
    print_end("LIST | FUNC | INSERT TAIL | TYPE: [INT]", fail_cnt);
    
    
    
    
    
    
    
    
}



void test_list_insert_index() {
    hm_list list;
    hm_list_init(&list, free);
    hm_listnode* curr;
    // insert index
    
    print_run("LIST | FUNC | INSERT INDEX | TYPE: [INT]");
    int fail_cnt = 0;
    int fail_diff = 0;
    int fail_invalid_index = 0;
    
    size_t index[] = {1, 3, 1, 5, 3, 0, 1, 2, 3, 4, 29439, 88, 0, 8, 4, 9, 7};
    
    int size = sizeof(index) / sizeof(size_t);
    
    for (int i = 0; i < size; i++) {
        int* v = (int*)malloc(sizeof(int));
        
        // this can guarantee the value of every insert is definitely different
        *v = i;
        
        size_t prev_s = hm_list_size(&list);

        hm_list_ret ret = hm_list_insert_index(&list, v, index[i]);
        
        if (index[i] <= prev_s) {
            size_t loop_cnt = index[i];
            curr = list.head;
            int j;
            for (j = 0; j < loop_cnt && curr; j++) {
                curr = curr->next;
            }
            if (!curr || *(int*)(curr->val) != i) {
                fail_diff++;
            }
        } else {
            if (ret != hm_list_ret_warn) {
                fail_invalid_index++;
            } else {
                free(v);
            }
        }
        
    }
    check_res(fail_diff == 0, "data in list is wrong", &fail_cnt);
    check_res(fail_invalid_index == 0, "pass invalid index but the return of this function isn't `hm_list_ret_warn`", &fail_cnt);

    // test the integrity of list
    test_list_integrity(&list, &fail_cnt);
    
    hm_list_free(&list);
    print_end("LIST | FUNC | INSERT INDEX | TYPE: [INT]", fail_cnt);
    
}

void test_iter_list() {
    hm_list list;
    hm_list_init(&list, free);
    int num = 100;
    int flag[num];
    int fail_cnt = 0;



    // insert
    for (int i = 0; i < num; i++) {
        flag[i] = i * 10;
        int* v = (int*)malloc(sizeof(int));
        *v = flag[i];
        hm_list_insert_tail(&list, v);
    }

    // iterator
    print_run("LIST | FUNC | ITERATOR | TYPE: [INT]");
    int cnt = 0;
    hm_iter_list iter;
    hm_iter_list_init(&iter, &list);
    int fail_diff = 0;
    while (hm_iter_list_has_next(&iter)) {
        int* v = hm_iter_list_next(&iter);
        if (*v != flag[cnt]) {
            fail_diff++;
        }
        cnt++;
    }
    check_res(cnt == list.size, "the number of val got by list's iterator is wrong", &fail_cnt);
    check_res(fail_diff == 0, "the val got by iterator is wrong", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);
    hm_list_free(&list);
    print_end("LIST | FUNC | ITERATOR | TYPE: [INT]", fail_cnt);
}

void test_list_get() {
    hm_list list;
    hm_list_init(&list, free);
    int fail_cnt = 0;
    int fail_nullptr = 0;
    int fail_diff = 0;
    int num = 100;
    int flag[num];

    // insert
    for (int i = 0; i < num; i++) {
        flag[i] = i * 100;
        int* v = (int*)malloc(sizeof(int));
        *v = flag[i];
        hm_list_insert_tail(&list, v);
    }

    print_run("LIST | FUNC | GET | TYPE: [INT]");
    // get and verify[valid]
    for (int i = 0; i < num; i++) {
        int* v = hm_list_get(&list, i);
        if (v == NULL) {
            fail_nullptr++;
        } else {
            if (*v != flag[i]) {
                fail_diff++;
            }
        }
    }
    check_res(fail_diff == 0, "data got by `get` is wrong", &fail_cnt);
    check_res(fail_nullptr == 0, "data is existed but `get` return nullptr", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);
    // get and verify[invalid]
    int fail_exist = 0;
    for (int i = num; i < num * 2; i++) {
        int* v = hm_list_get(&list, i);
        if (v) {
            fail_exist++;
        }
    }
    check_res(fail_exist == 0, "data isn't existed but `get` return not nullptr", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);

    hm_list_free(&list);
    print_end("LIST | FUNC | GET | TYPE: [INT]", fail_cnt);

}

void test_list_change() {
    hm_list list;
    hm_list_init(&list, free);
    int fail_cnt = 0;
    int fail_diff = 0;
    int num = 100;
    int flag[num];

    // insert
    for (int i = 0; i < num; i++) {
        flag[i] = i * 100;
        int* v = (int*)malloc(sizeof(int));
        *v = flag[i];
        hm_list_insert_tail(&list, v);
    }

    print_run("LIST | FUNC | CHANGE | TYPE: [INT]");
    // change [use `hm_list_get`]
    int diff = 33;
    for (int i = 0; i < num; i++) {
        flag[i] += diff;
        int* v = hm_list_get(&list, i);
        *v += diff;
    }

    // verify
    for (int i = 0; i < num; i++) {
        int* v = hm_list_get(&list, i);
        if (*v != flag[i]) {
            fail_diff++;
        }
    }
    
    check_res(fail_diff == 0, "the data that has changed is wrong", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);
    hm_list_free(&list);

    print_end("LIST | FUNC | CHANGE | TYPE: [INT]", fail_cnt);
    
}

void test_list_del_head() {
    int fail_cnt = 0;
    hm_list list;
    hm_list_init(&list, free);
    int num = 100;
    int flag[num];

    // insert
    for (int i = 0; i < num; i++) {
        flag[i] = i * 100;
        int* v = (int*)malloc(sizeof(int));
        *v = flag[i];
        hm_list_insert_tail(&list, v);
    }

    print_run("LIST | FUNC | DEL HEAD | TYPE: [INT]");

    // del some vals
    int fail_del = 0;
    for (int i = 0; i < num / 2; i++) {
        if (hm_list_del_head(&list) != hm_list_ret_suc) {
            fail_del++;
        }
    }
    check_res(fail_del == 0, "it should return suc but return others", &fail_cnt);
    check_res(list.size == (num - num /2), "list.size is wrong after del half of vals in list", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);


    // verify
    hm_iter_list iter;
    hm_iter_list_init(&iter, &list);
    int fail_diff = 0;
    int cur = num / 2;
    while (hm_iter_list_has_next(&iter)) {
        int* v = hm_iter_list_next(&iter);
        if (*v != flag[cur]) {
            fail_diff++;
        }
        cur++;
    }
    check_res(fail_diff == 0, "data in list is wrong after del half of vals in list", &fail_cnt);

    // del all vals
    fail_del = 0;
    for (int i = num / 2; i < num; i++) {
        if (hm_list_del_head(&list) != hm_list_ret_suc) {
            fail_del++;
        }
    }
    check_res(fail_del == 0, "it should return suc but return others", &fail_cnt);
    
    check_res(list.size == 0, "list.size isn't 0 after del all vals", &fail_cnt);
    check_res(list.head == NULL, "list.head isn't 0 after del all vals", &fail_cnt);
    check_res(list.tail == NULL, "list.tail isn't 0 after del all vals", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);

    // del empty list
    int fail_del_empty = 0;
    int test_cnt_del_empty = 10;
    for (int i = 0; i < test_cnt_del_empty; i++) {
        if (hm_list_del_head(&list) != hm_list_ret_none) {
            fail_del_empty++;
        }
    }

    check_res(fail_del_empty == 0, "it should return `hm_list_ret_none` when del empty list, but return others", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);
    hm_list_free(&list);


    print_end("LIST | FUNC | DEL HEAD | TYPE: [INT]", fail_cnt);
}

void test_list_del_tail() {
    int fail_cnt = 0;
    hm_list list;
    hm_list_init(&list, free);
    int num = 100;
    int flag[num];
    
    // insert
    for (int i = 0; i < num; i++) {
        flag[i] = i * 100;
        int* v = (int*)malloc(sizeof(int));
        *v = flag[i];
        hm_list_insert_tail(&list, v);
    }
    
    print_run("LIST | FUNC | DEL TAIL | TYPE: [INT]");
    
    // del some vals
    int fail_del = 0;
    for (int i = 0; i < num / 2; i++) {
        if (hm_list_del_tail(&list) != hm_list_ret_suc) {
            fail_del++;
        }
    }
    check_res(fail_del == 0, "it should return suc but return others", &fail_cnt);
    check_res(list.size == (num - num /2), "list.size is wrong after del half of vals in list", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);
    
    
    // verify
    hm_iter_list iter;
    hm_iter_list_init(&iter, &list);
    int fail_diff = 0;
    int cur = 0;
    while (hm_iter_list_has_next(&iter)) {
        int* v = hm_iter_list_next(&iter);
        if (*v != flag[cur]) {
            fail_diff++;
        }
        cur++;
    }
    check_res(fail_diff == 0, "data in list is wrong after del half of vals in list", &fail_cnt);
    
    // del all vals
    fail_del = 0;
    for (int i = num / 2; i < num; i++) {
        if (hm_list_del_tail(&list) != hm_list_ret_suc) {
            fail_del++;
        }
    }
    check_res(fail_del == 0, "it should return suc but return others", &fail_cnt);
    
    check_res(list.size == 0, "list.size isn't 0 after del all vals", &fail_cnt);
    check_res(list.head == NULL, "list.head isn't 0 after del all vals", &fail_cnt);
    check_res(list.tail == NULL, "list.tail isn't 0 after del all vals", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);
    
    // del empty list
    int fail_del_empty = 0;
    int test_cnt_del_empty = 10;
    for (int i = 0; i < test_cnt_del_empty; i++) {
        if (hm_list_del_tail(&list) != hm_list_ret_none) {
            fail_del_empty++;
        }
    }
    
    check_res(fail_del_empty == 0, "it should return `hm_list_ret_none` when del empty list, but return others", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);
    
    hm_list_free(&list);
    
    
    print_end("LIST | FUNC | DEL TAIL | TYPE: [INT]", fail_cnt);
    

}

void test_list_del_index() {
    int num = 100;
    int fail_cnt = 0;
    hm_list list;
    hm_list_init(&list, free);

    // insert
    for (int i = 0; i < 100; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 100;
        hm_list_insert_tail(&list, v);
    }


    print_run("LIST | FUNC | DEL INDEX | TYPE: [INT]");

    // del
    
    
    size_t index[] = {2, 3, 10000, 2, 3, 1};


    int fail_del_empty = 0;     // del invalid index of val
    int fail_del_exist = 0;     // fail in del existed val
    int n = sizeof(index) / sizeof(size_t);
    for (int i = 0; i < n; i++) {
        int prev_size = hm_list_size(&list);
        int* del_v = hm_list_get(&list, index[i]);

        hm_list_ret ret = hm_list_del_index(&list, index[i]);

        if (index[i] >= prev_size) {
            if (ret != hm_list_ret_none) {
                fail_del_empty++;
            }
        } else {
            hm_iter_list iter;
            hm_iter_list_init(&iter, &list);
            while (hm_iter_list_has_next(&iter)) {
                int* v = hm_iter_list_next(&iter);
                if (*v == *del_v) {
                    fail_del_exist++;
                    break;
                }
            }
        }

    }

    test_list_integrity(&list, &fail_cnt);
    check_res(fail_del_empty == 0, "the return tag of del function isn't `hm_list_ret_none` when del invalid index", &fail_cnt);
    check_res(fail_del_exist == 0, "the val still existed in list when del this val", &fail_cnt);

    hm_list_free(&list);

    print_end("LIST | FUNC | DEL INDEX | TYPE: [INT]", fail_cnt);


}

void test_list_free() {
    int num = 100;
    int fail_cnt = 0;
    hm_list list;
    hm_list_init(&list, free);

    // insert
    for (int i = 0; i < 100; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = i * 100;
        hm_list_insert_tail(&list, v);
    }


    print_run("LIST | FUNC | FREE | TYPE: [INT]");



    hm_list_free(&list);
    check_res(list.size == 0, "list.size isn't 0 after free all list", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);
    
    // double free
    hm_list_free(&list);
    check_res(list.size == 0, "list.size isn't 0 after double free all list", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);

    print_end("LIST | FUNC | FREE | TYPE: [INT]", fail_cnt);


}

void test_list_sort() {
    int fail_cnt = 0;
    hm_list list;
    hm_list_init(&list, free);

    print_run("LIST | FUNC | SORT | TYPE: [INT]");
    int flag[] = {2, 1, 3, 4, 5, 3, 2, 4};
    int cnt = sizeof(flag) / sizeof(int);

    for (int i = 0; i < cnt; i++) {
        int* v = (int*)malloc(sizeof(int));
        *v = flag[i];
        hm_list_insert_tail(&list, v);
    }

    qsort(flag, cnt, sizeof(int), cmp_int_up);

    hm_list_sort(&list, cmp_int_up);
    test_list_integrity(&list, &fail_cnt);

    int fail_sort = 0;
    for (int i = 0; i < cnt; i++) {
        int* v = hm_list_get(&list, i);
        if (*v != flag[i]) {
            fail_sort++;
        }
    }
    check_res(fail_sort == 0, "the value is wrong after sort list", &fail_cnt);
    hm_list_free(&list);
    print_end("LIST | FUNC | SORT | TYPE: [INT]", fail_cnt);
}


void test_list_insert_tail_stress() {
    int v = 666666;
    hm_list list;
    hm_list_init(&list, NULL);
    size_t nums[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums) / sizeof(size_t);

    print_run("LIST | STRESS | INSERT TAIL | TYPE: [INT]");
    int fail_cnt = 0;
    for (int i = 0; i < cnt; i++) {
        size_t suc = 0;
        clock_t start = clock();
        for (size_t j = 0; j < nums[i]; j++) {
            if (hm_list_insert_tail(&list, &v) == hm_list_ret_suc) {
                suc++;
            }
        }
        clock_t end = clock();
        test_list_integrity(&list, &fail_cnt);
        print_run_time("INSERT", start, end, nums[i], nums[i]);
        check_res(suc == list.size, "the list.size is wrong when insert many vals", &fail_cnt);
        hm_list_free(&list);
    }

    print_end("LIST | STRESS | INSERT TAIL | TYPE: [INT]", fail_cnt);



}

void test_list_insert_head_stress() {
    int v = 666666;
    hm_list list;
    hm_list_init(&list, NULL);
    size_t nums[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums) / sizeof(size_t);

    print_run("LIST | STRESS | INSERT HEAD | TYPE: [INT]");
    int fail_cnt = 0;
    for (int i = 0; i < cnt; i++) {
        size_t suc = 0;
        clock_t start = clock();
        for (size_t j = 0; j < nums[i]; j++) {
            if (hm_list_insert_head(&list, &v) == hm_list_ret_suc) {
                suc++;
            }
        }
        clock_t end = clock();
        test_list_integrity(&list, &fail_cnt);
        print_run_time("INSERT", start, end, nums[i], nums[i]);
        check_res(suc == list.size, "the list.size is wrong when insert many vals", &fail_cnt);
        hm_list_free(&list);
    }

    print_end("LIST | STRESS | INSERT HEAD | TYPE: [INT]", fail_cnt);
}


void test_list_insert_index_stress() {
    int v = 666666;
    hm_list list;
    hm_list_init(&list, NULL);

    // insert head
    size_t nums_head[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums_head) / sizeof(size_t);

    print_run("LIST | STRESS | INSERT INDEX(HEAD) | TYPE: [INT]");
    int fail_cnt = 0;
    for (int i = 0; i < cnt; i++) {
        size_t suc = 0;
        clock_t start = clock();
        for (size_t j = 0; j < nums_head[i]; j++) {
            if (hm_list_insert_index(&list, &v, 0) == hm_list_ret_suc) {
                suc++;
            }
        }
        clock_t end = clock();
        test_list_integrity(&list, &fail_cnt);
        print_run_time("INSERT", start, end, nums_head[i], nums_head[i]);
        check_res(suc == list.size, "the list.size is wrong when insert many vals", &fail_cnt);
        hm_list_free(&list);
    }

    print_end("LIST | STRESS | INSERT INDEX(HEAD) | TYPE: [INT]\n", fail_cnt);


    // insert tail
    size_t nums_tail[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    cnt = sizeof(nums_tail) / sizeof(size_t);

    print_run("LIST | STRESS | INSERT INDEX(TAIL) | TYPE: [INT]");
    fail_cnt = 0;
    for (int i = 0; i < cnt; i++) {
        size_t suc = 0;
        clock_t start = clock();
        for (size_t j = 0; j < nums_tail[i]; j++) {
            if (hm_list_insert_index(&list, &v, list.size) == hm_list_ret_suc) {
                suc++;
            }
        }
        clock_t end = clock();
        print_run_time("INSERT", start, end, nums_tail[i], nums_tail[i]);
        check_res(suc == list.size, "the list.size is wrong when insert many vals", &fail_cnt);
        hm_list_free(&list);
    }

    print_end("LIST | STRESS | INSERT INDEX(TAIL) | TYPE: [INT]\n", fail_cnt);
    



    // insert ++list.size - 1++

    size_t nums_tail_sub_1[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    cnt = sizeof(nums_tail_sub_1) / sizeof(size_t);

    print_run("LIST | STRESS | INSERT INDEX(TAIL - 1) | TYPE: [INT]");
    fail_cnt = 0;
    // this can test the perf of `hm_list_insert_index`

    for (int i = 0; i < cnt; i++) {
        size_t suc = 0;
        clock_t start = clock();

        // Ensure the list.size - 1 > 0
        hm_list_insert_tail(&list, &v);
        suc++;

        for (size_t j = 0; j < nums_tail_sub_1[i]; j++) {
            if (hm_list_insert_index(&list, &v, list.size - 1) == hm_list_ret_suc) {
                suc++;
            }
        }
        clock_t end = clock();
        print_run_time("INSERT", start, end, nums_tail_sub_1[i], nums_tail_sub_1[i]);
        check_res(suc == list.size, "the list.size is wrong when insert many vals", &fail_cnt);
        hm_list_free(&list);
    }

    print_end("LIST | STRESS | INSERT INDEX(TAIL - 1) | TYPE: [INT]\n", fail_cnt);
    


    // insert half of list.size
    size_t nums_mid[] = {10000, 50000, 100000};
    cnt = sizeof(nums_mid) / sizeof(size_t);

    print_run("LIST | STRESS | INSERT INDEX(MID) | TYPE: [INT]");
    fail_cnt = 0;
    for (int i = 0; i < cnt; i++) {
        size_t suc = 0;
        clock_t start = clock();
        for (size_t j = 0; j < nums_mid[i]; j++) {
            if (hm_list_insert_index(&list, &v, list.size / 2) == hm_list_ret_suc) {
                suc++;
            }
        }
        clock_t end = clock();
        print_run_time("INSERT", start, end, nums_mid[i], nums_mid[i]);
        check_res(suc == list.size, "the list.size is wrong when insert many vals", &fail_cnt);
        hm_list_free(&list);
    }

    print_end("LIST | STRESS | INSERT INDEX(MID) | TYPE: [INT]\n", fail_cnt);

}


void test_list_get_stress() {   
    int v = 666666;
    int fail_cnt = 0;
    hm_list list;
    hm_list_init(&list, NULL);



    // get head
    print_run("LIST | STRESS | GET HEAD | TYPE: [INT]");
    // the numbers of list cann't too big
    size_t nums_head[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums_head) / sizeof(size_t);

    for (int i = 0; i < cnt; i++) {
        // insert
        for (size_t j = 0; j < nums_head[i]; j++) {
            hm_list_insert_tail(&list, &v);
        }


        // get
        size_t oper_cnt = 10000000;
        int fail_get = 0;
        clock_t start = clock();
        for (size_t j = 0; j < oper_cnt; j++) {
            if (hm_list_get(&list, 0) == NULL) {
                fail_get++;
            }
        }
        clock_t end = clock();
        test_list_integrity(&list, &fail_cnt);
        check_res(fail_get == 0, "the value that got by get function is NULL when get head stress test", &fail_cnt);
        hm_list_free(&list);
        print_run_time("GET", start, end, nums_head[i], oper_cnt);
    }
    print_end("LIST | STRESS | GET HEAD | TYPE: [INT]\n", fail_cnt);
    
    fail_cnt = 0;



    // get tail
    print_run("LIST | STRESS | GET TAIL | TYPE: [INT]");
    size_t nums_tail[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    cnt = sizeof(nums_tail) / sizeof(size_t);

    for (int i = 0; i < cnt; i++) {
        // insert
        for (size_t j = 0; j < nums_tail[i]; j++) {
            hm_list_insert_tail(&list, &v);
        }

        // get
        size_t oper_cnt = 10000000;
        int fail_get = 0;
        clock_t start = clock();
        for (size_t j = 0; j < oper_cnt; j++) {
            if (hm_list_get(&list, list.size - 1) == NULL) {
                fail_get++;
            }
        }
        clock_t end = clock();
        check_res(fail_get == 0, "the value that got by get function is NULL when get tail stress test", &fail_cnt);
        hm_list_free(&list);
        print_run_time("GET", start, end, nums_tail[i], oper_cnt);
    }
    print_end("LIST | STRESS | GET TAIL | TYPE: [INT]\n", fail_cnt);
    
    
    
    // get mid
    print_run("LIST | STRESS | GET MID | TYPE: [INT]");
    
    // the nums cann't to big because it's time complexity is O(n^2)
    size_t nums_mid[] = {10000, 50000};
    cnt = sizeof(nums_mid) / sizeof(size_t);
    
    for (int i = 0; i < cnt; i++) {
        // insert
        for (size_t j = 0; j < nums_mid[i]; j++) {
            hm_list_insert_tail(&list, &v);
        }
        
        // get
        int fail_get = 0;
        clock_t start = clock();
        size_t oper_cnt = 10000;
        for (size_t j = 0; j < oper_cnt; j++) {
            if (hm_list_get(&list, list.size / 2) == NULL) {
                fail_get++;
            }
        }
        clock_t end = clock();
        check_res(fail_get == 0, "the value that got by get function is NULL when get mid stress test", &fail_cnt);
        hm_list_free(&list);
        print_run_time("GET", start, end, nums_mid[i], oper_cnt);
    }
    print_end("LIST | STRESS | GET MID | TYPE: [INT]", fail_cnt);
}

void test_list_del_head_stress() {
    int fail_cnt = 0;
    int v = 666666;
    hm_list list;
    hm_list_init(&list, NULL);
    size_t nums[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums) / sizeof(size_t);

    print_run("LIST | STRESS | DEL HEAD | TYPE: [INT]");
    for (int i = 0; i < cnt; i++) {
        // insert
        for (size_t j = 0; j < nums[i]; j++) {
            hm_list_insert_tail(&list, &v);
        }

        // del

        int fail_del = 0;
        
        clock_t start = clock();
        for (size_t j = 0; j < nums[i]; j++) {
            if (hm_list_del_head(&list) != hm_list_ret_suc) {
                fail_del++;
            }
        }
        clock_t end = clock();
        test_list_integrity(&list, &fail_cnt);
        check_res(fail_del == 0, "the tag of return isn't suc when run stressful del head test", &fail_cnt);
        print_run_time("DEL", start, end, nums[i], nums[i]);
        hm_list_free(&list);

    }
    print_end("LIST | STRESS | DEL HEAD | TYPE: [INT]", fail_cnt);
}

void test_list_del_tail_stress() {
    int fail_cnt = 0;
    int v = 666666;
    hm_list list;
    hm_list_init(&list, NULL);
    size_t nums[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums) / sizeof(size_t);

    print_run("LIST | STRESS | DEL TAIL | TYPE: [INT]");
    for (int i = 0; i < cnt; i++) {
        // insert
        for (size_t j = 0; j < nums[i]; j++) {
            hm_list_insert_tail(&list, &v);
        }

        // del

        int fail_del = 0;
        
        clock_t start = clock();
        for (size_t j = 0; j < nums[i]; j++) {
            if (hm_list_del_tail(&list) != hm_list_ret_suc) {
                fail_del++;
            }
        }
        clock_t end = clock();
        test_list_integrity(&list, &fail_cnt);
        check_res(fail_del == 0, "the tag of return isn't suc when run stressful del tail test", &fail_cnt);
        print_run_time("DEL", start, end, nums[i], nums[i]);
        hm_list_free(&list);

    }
    print_end("LIST | STRESS | DEL TAIL | TYPE: [INT]", fail_cnt);
}

void test_list_del_index_stress() {
    int fail_cnt = 0;
    int v = 666666;
    hm_list list;
    hm_list_init(&list, NULL);


    // del head
    size_t nums_head[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums_head) / sizeof(size_t);

    print_run("LIST | STRESS | DEL INDEX(HEAD) | TYPE: [INT]");
    for (int i = 0; i < cnt; i++) {
        // insert
        for (size_t j = 0; j < nums_head[i]; j++) {
            hm_list_insert_tail(&list, &v);
        }

        // del

        int fail_del = 0;
        
        clock_t start = clock();
        for (size_t j = 0; j < nums_head[i]; j++) {
            if (hm_list_del_index(&list, 0) != hm_list_ret_suc) {
                fail_del++;
            }
        }
        clock_t end = clock();
        test_list_integrity(&list, &fail_cnt);
        check_res(fail_del == 0, "the tag of return isn't suc when run stressful del index(head) test", &fail_cnt);
        print_run_time("DEL", start, end, nums_head[i], nums_head[i]);
        hm_list_free(&list);

    }
    print_end("LIST | STRESS | DEL INDEX(HEAD) | TYPE: [INT]\n", fail_cnt);


    // del tail

    size_t nums_tail[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    cnt = sizeof(nums_tail) / sizeof(size_t);

    print_run("LIST | STRESS | DEL INDEX(TAIL) | TYPE: [INT]");
    for (int i = 0; i < cnt; i++) {
        // insert
        for (size_t j = 0; j < nums_head[i]; j++) {
            hm_list_insert_tail(&list, &v);
        }

        // del

        int fail_del = 0;
        
        clock_t start = clock();
        for (size_t j = 0; j < nums_head[i]; j++) {
            if (hm_list_del_index(&list, list.size - 1) != hm_list_ret_suc) {
                fail_del++;
            }
        }
        clock_t end = clock();
        check_res(fail_del == 0, "the tag of return isn't suc when run stressful del index(tail) test", &fail_cnt);
        print_run_time("DEL", start, end, nums_head[i], nums_head[i]);
        hm_list_free(&list);

    }
    print_end("LIST | STRESS | DEL INDEX(TAIL) | TYPE: [INT]\n", fail_cnt);
    
    
    // del ++list.size - 2 == The one before the last one in list++
    
    size_t nums_tail_sub_1[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    cnt = sizeof(nums_tail_sub_1) / sizeof(size_t);
    // this can test the perf of `hm_list_del_index`

    print_run("LIST | STRESS | DEL INDEX(TAIL - 1) | TYPE: [INT]");
    for (int i = 0; i < cnt; i++) {
        // insert
        for (size_t j = 0; j < nums_tail_sub_1[i]; j++) {
            hm_list_insert_tail(&list, &v);
        }
    
        // del
    
        int fail_del = 0;
        
        clock_t start = clock();
        
        // the loop count must be limited at `nums_tail_sub_1[i] - 1` because the `index` that pass in when `list.size == 1`
        for (size_t j = 0; j < nums_tail_sub_1[i] - 1; j++) {
            if (hm_list_del_index(&list, list.size - 2) != hm_list_ret_suc) {
                fail_del++;
            }
        }
        clock_t end = clock();
        check_res(fail_del == 0, "the tag of return isn't suc when run stressful del index(tail - 1) test", &fail_cnt);
        print_run_time("DEL", start, end, nums_tail_sub_1[i], nums_tail_sub_1[i] - 1);
        hm_list_free(&list);
    
    }
    print_end("LIST | STRESS | DEL INDEX(TAIL - 1) | TYPE: [INT]\n", fail_cnt);

    // del index at the middle of list

    size_t nums_mid[] = {10000, 50000, 100000};
    cnt = sizeof(nums_mid) / sizeof(size_t);

    print_run("LIST | STRESS | DEL INDEX(MID) | TYPE: [INT]");
    for (int i = 0; i < cnt; i++) {
        // insert
        for (size_t j = 0; j < nums_mid[i]; j++) {
            hm_list_insert_tail(&list, &v);
        }

        // del

        int fail_del = 0;
        
        clock_t start = clock();
        for (size_t j = 0; j < nums_mid[i]; j++) {
            if (hm_list_del_index(&list, list.size / 2) != hm_list_ret_suc) {
                fail_del++;
            }
        }
        clock_t end = clock();
        check_res(fail_del == 0, "the tag of return isn't suc when run stressful del index(middle) test", &fail_cnt);
        print_run_time("DEL", start, end, nums_mid[i], nums_mid[i]);
        hm_list_free(&list);

    }
    print_end("LIST | STRESS | DEL INDEX(MID) | TYPE: [INT]", fail_cnt);
    
}


void test_list_free_stress() {
    int fail_cnt = 0;
    hm_list list;
    size_t nums[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums) / sizeof(size_t);
    
    
    // del node (value is located in stack memory of system) test
    hm_list_init(&list, NULL);
    int stack_v = 888;
    print_run("LIST | STRESS | FREE(ONLY NODE) | TYPE: [INT]");
    for (int i = 0; i < cnt; i++) {
        
        // insert

        for (size_t j = 0; j < nums[i]; j++) {
            hm_list_insert_tail(&list, &stack_v);
        }

        // free
        clock_t start = clock();

        hm_list_free(&list);

        clock_t end = clock();

        check_res(list.size == 0, "the list.size isn't 0 after insert many vals and free it", &fail_cnt);
        test_list_integrity(&list, &fail_cnt);

        print_run_time("FREE", start, end, nums[i], nums[i]);

    }
    print_end("LIST | STRESS | FREE(ONLY NODE) | TYPE: [INT]\n", fail_cnt);
    
    // del node and val(val is allocted) test
    hm_list_init(&list, free);
    fail_cnt = 0;
    print_run("LIST | STRESS | FREE(NODE & VAL) | TYPE: [INT]");
    for (int i = 0; i < cnt; i++) {
        
        // insert

        for (size_t j = 0; j < nums[i]; j++) {
            int* heap_v = (int*)malloc(sizeof(int));
            hm_list_insert_tail(&list, heap_v);
        }

        // free
        clock_t start = clock();

        hm_list_free(&list);

        clock_t end = clock();

        check_res(list.size == 0, "the list.size isn't 0 after insert many vals and free it", &fail_cnt);
        test_list_integrity(&list, &fail_cnt);

        print_run_time("FREE", start, end, nums[i], nums[i]);

    }
    print_end("LIST | STRESS | FREE(NODE & VAL) | TYPE: [INT]", fail_cnt);



}

void test_empty_list_oper() {
    int fail_cnt = 0;
    hm_list list;
    hm_list_init(&list, free);


    print_run("LIST | BOUNDARY | OPER EMPTY LIST | TYPE: [INT]");

    // get

    check_res(hm_list_get(&list, 0) == NULL, "get on empty list should return NULL", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);
    
    check_res(hm_list_get(&list, 100) == NULL, "get with large index on empty list should return NULL", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);

    // del
    check_res(hm_list_del_head(&list) == hm_list_ret_none, "del_head on empty list should return none", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);

    check_res(hm_list_del_tail(&list) == hm_list_ret_none, "del_tail on empty list should return none", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);

    check_res(hm_list_del_index(&list, 0) == hm_list_ret_none, "del_index on empty list should return none", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);

    // iter
    hm_iter_list iter;
    hm_iter_list_init(&iter, &list);
    int loop_cnt = 0;
    while (hm_iter_list_has_next(&iter)) {
        hm_iter_list_next(&iter);       // the return value don't have any function for this test
        loop_cnt++;
    }
    check_res(loop_cnt == 0, "iterator over empty list should yield zero elements", &fail_cnt);
    
    int* v = (int*)malloc(sizeof(int));
    hm_list_insert_tail(&list, v);
    
    hm_list_free(&list);
    
    // still operate on freed list

    // get

    check_res(hm_list_get(&list, 0) == NULL, "get on freed list should return NULL", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);
    
    check_res(hm_list_get(&list, 100) == NULL, "get with large index on freed list should return NULL", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);

    // del
    check_res(hm_list_del_head(&list) == hm_list_ret_none, "del_head on freed list should return none", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);

    check_res(hm_list_del_tail(&list) == hm_list_ret_none, "del_tail on freed list should return none", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);

    check_res(hm_list_del_index(&list, 0) == hm_list_ret_none, "del_index on freed list should return none", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);

    // iter
    hm_iter_list_init(&iter, &list);
    loop_cnt = 0;
    while (hm_iter_list_has_next(&iter)) {
        hm_iter_list_next(&iter);       // the return value don't have any function for this test
        loop_cnt++;
    }
    check_res(loop_cnt == 0, "iterator over freed list should yield zero elements", &fail_cnt);

    print_end("LIST | BOUNDARY | OPER EMPTY LIST | TYPE: [INT]", fail_cnt);

}

void test_single_listnode_oper() {
    int fail_cnt = 0;
    hm_list list;
    hm_list_init(&list, NULL);
    int v = 666;

    print_run("LIST | BOUNDARY | OPER SINGLE LISTNODE'S LIST | TYPE: [INT]");


    hm_list_insert_tail(&list, &v);
    // del head
    check_res(hm_list_del_head(&list) == hm_list_ret_suc, "del_head on single listnode's list should return suc", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);
    
    hm_list_insert_tail(&list, &v);
    // del tail
    check_res(hm_list_del_head(&list) == hm_list_ret_suc, "del_tail on single listnode's list should return suc", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);
    
    hm_list_insert_tail(&list, &v);
    // del index
    check_res(hm_list_del_index(&list, 0) == hm_list_ret_suc, "del_index on single listnode's list should return suc", &fail_cnt);
    test_list_integrity(&list, &fail_cnt);



    hm_list_insert_tail(&list, &v);
    // insert new node at tail of now node
    int v_new = 10;
    hm_list_insert_tail(&list, &v_new);
    test_list_integrity(&list, &fail_cnt);

    int* res = hm_list_get(&list, 1);
    check_res(*res == v_new, "the new node's val isn't the new val when insert node in the tail of single listnode's list", &fail_cnt);
    
    hm_list_free(&list);
    
    hm_list_insert_tail(&list, &v);
    // insert new node at tail of now node
    v_new = 100;
    hm_list_insert_head(&list, &v_new);
    test_list_integrity(&list, &fail_cnt);

    res = hm_list_get(&list, 0);
    check_res(*res == v_new, "the new node's val isn't the new val when insert node in the head of single listnode's list", &fail_cnt);

    hm_list_free(&list);
    test_list_integrity(&list, &fail_cnt);

    print_end("LIST | BOUNDARY | OPER SINGLE LISTNODE'S LIST | TYPE: [INT]", fail_cnt);

}

void test_list_sort_stress() {
    srand(666);
    int fail_cnt = 0;
    hm_list list;
    size_t nums[] = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    hm_list_init(&list, free);
    int cnt = sizeof(nums) / sizeof(size_t);

    print_run("LIST | STRESS | SORT | TYPE: [INT]");
    for (int i = 0; i < cnt; i++) {
        // random generate and insert

        int* vals = (int*)malloc(nums[i] * sizeof(int));

        for (size_t j = 0; j < nums[i]; j++) {
            vals[j] = rand();
            int* v = (int*)malloc(sizeof(int));
            *v = vals[j];
            hm_list_insert_tail(&list, v);
        }

        // sort

        qsort(vals, nums[i], sizeof(int), cmp_int_up);

        clock_t start = clock();
        hm_list_sort(&list, cmp_int_up);
        clock_t end = clock();


        // verify
        test_list_integrity(&list, &fail_cnt);

        int fail_sort = 0;
        size_t n = 0;
        hm_iter_list iter;
        hm_iter_list_init(&iter, &list);

        while (hm_iter_list_has_next(&iter)) {
            int* v = hm_iter_list_next(&iter);
            if (*v != vals[n]) {
                fail_sort++;
            }
            n++;
        }
        check_res(fail_sort == 0, "the value is wrong after sort list with many vals", &fail_cnt);
        
        print_run_time("SORT", start, end, nums[i], nums[i]);

        free(vals);
        hm_list_free(&list);
    }

    print_end("LIST | STRESS | SORT | TYPE: [INT]", fail_cnt);
}

void function_test() {
    test_list_init();                               printf("\n");
    
    test_list_insert_head();                        printf("\n");
    test_list_insert_tail();                        printf("\n");
    test_list_insert_index();                       printf("\n");
    
    test_iter_list();                               printf("\n");
    
    test_list_get();                                printf("\n");
    
    test_list_change();                             printf("\n");
    
    test_list_del_head();                           printf("\n");
    test_list_del_tail();                           printf("\n");
    test_list_del_index();                          printf("\n");
    
    test_list_free();                               printf("\n");

    test_list_sort();                               printf("\n");

}

void boundary_test() {
    test_empty_list_oper();                         printf("\n");

    test_single_listnode_oper();                    printf("\n");

}

void stress_test() {

    test_list_insert_tail_stress();                 printf("\n");
    
    test_list_insert_head_stress();                 printf("\n");

    test_list_insert_index_stress();                printf("\n");
    
    test_list_get_stress();                         printf("\n");
    
    test_list_del_head_stress();                    printf("\n");
    
    test_list_del_tail_stress();                    printf("\n");
    
    test_list_del_index_stress();                   printf("\n");
    
    test_list_free_stress();                        printf("\n");
    
    test_list_sort_stress();                        printf("\n");

}



int main()
{
    function_test();
    
    boundary_test();

    stress_test();

    
    return 0;
}