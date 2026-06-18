#include "../../include/hm_list.h"
#include "../hm_test.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

void test_list_intergrity(hm_list* list, int* fail_cnt) {
    int cnt;

    // -->
    cnt = 0;
    hm_listnode* cur = list->head;
    while (cur) {
        cur = cur->next;
        cnt++;
    }
    check_res(cnt == list->size, "TEST OF INTERGRITY: the next of listnode is wrong", fail_cnt);
    
    // <--
    cnt = 0;
    cur = list->tail;
    while (cur) {
        cur = cur->prev;
        cnt++;
    }
    
    check_res(cnt == list->size, "TEST OF INTERGRITY: the prev of listnode is wrong", fail_cnt);

}

void test_list_init() {
    
    hm_list list_1;
    int fail_cnt = 0;
    print_run("LIST:initialize");
    hm_list_init(&list_1, free);
    
    check_res(list_1.free == free, "Pass `free` but list.free isn't `free`", &fail_cnt);
    check_res(list_1.head == NULL, "list.head isn't `NULL`", &fail_cnt);
    check_res(list_1.tail == NULL, "list.tail isn't `NULL`", &fail_cnt);
    check_res(list_1.size == 0, "list.size isn't 0", &fail_cnt);
    
    
    hm_list list_2;
    hm_list_init(&list_2, NULL);
    
    
    check_res(list_2.free == NULL, "Pass `NULL` but list.free isn't `NULL`", &fail_cnt);
    print_end("LIST:initialize", fail_cnt);
    
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
    print_run("INSERT HEAD | TYPE: [INT]");
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
    
    // test the intergrity of list
    test_list_intergrity(&list, &fail_cnt);

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
    print_end("INSERT HEAD | TYPE: [INT]", fail_cnt);
    
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
    print_run("INSERT TAIL | TYPE: [INT]");
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
    
    // test the intergrity of list
    test_list_intergrity(&list, &fail_cnt);
    
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
    print_end("INSERT TAIL | TYPE: [INT]", fail_cnt);
    
    
    
    
    
    
    
    
}



void test_list_insert_index() {
    hm_list list;
    hm_list_init(&list, free);
    hm_listnode* curr;
    // insert index
    
    print_run("INSERT INDEX | TYPE: [INT]");
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

    // test the intergrity of list
    test_list_intergrity(&list, &fail_cnt);
    
    hm_list_free(&list);
    print_end("INSERT INDEX | TYPE: [INT]", fail_cnt);
    
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
    print_run("ITERATOR | TYPE: [INT]");
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

    hm_list_free(&list);
    print_end("ITERATOR | TYPE: [INT]", fail_cnt);
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

    print_run("GET | TYPE: [INT]");
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

    // get and verify[invalid]
    int fail_exist = 0;
    for (int i = num; i < num * 2; i++) {
        int* v = hm_list_get(&list, i);
        if (v) {
            fail_exist++;
        }
    }
    check_res(fail_exist == 0, "data isn't existed but `get` return not nullptr", &fail_cnt);
    
    hm_list_free(&list);
    print_end("GET | TYPE: [INT]", fail_cnt);

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

    print_run("CHANGE | TYPE: [INT]");
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
    hm_list_free(&list);

    print_end("CHANGE | TYPE: [INT]", fail_cnt);
    
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

    print_run("DEL HEAD TEST | TYPE: [INT]");

    // del some vals
    int fail_del = 0;
    for (int i = 0; i < num / 2; i++) {
        if (hm_list_del_head(&list) != hm_list_ret_suc) {
            fail_del++;
        }
    }
    check_res(fail_del == 0, "it should return success but return others", &fail_cnt);
    check_res(list.size == (num - num /2), "list.size is wrong after del half of vals in list", &fail_cnt);
    test_list_intergrity(&list, &fail_cnt);


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
    check_res(fail_del == 0, "it should return success but return others", &fail_cnt);
    
    check_res(list.size == 0, "list.size isn't 0 after del all vals", &fail_cnt);
    check_res(list.head == NULL, "list.head isn't 0 after del all vals", &fail_cnt);
    check_res(list.tail == NULL, "list.tail isn't 0 after del all vals", &fail_cnt);

    // del empty list
    int fail_del_empty = 0;
    int test_cnt_del_empty = 10;
    for (int i = 0; i < test_cnt_del_empty; i++) {
        if (hm_list_del_head(&list) != hm_list_ret_none) {
            fail_del_empty++;
        }
    }

    check_res(fail_del_empty == 0, "it should return `hm_list_ret_none` when del empty list, but return others", &fail_cnt);
    
    hm_list_free(&list);


    print_end("DEL HEAD TEST | TYPE: [INT]", fail_cnt);
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
    
    print_run("DEL TAIL TEST | TYPE: [INT]");
    
    // del some vals
    int fail_del = 0;
    for (int i = 0; i < num / 2; i++) {
        if (hm_list_del_tail(&list) != hm_list_ret_suc) {
            fail_del++;
        }
    }
    check_res(fail_del == 0, "it should return success but return others", &fail_cnt);
    check_res(list.size == (num - num /2), "list.size is wrong after del half of vals in list", &fail_cnt);
    test_list_intergrity(&list, &fail_cnt);
    
    
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
    check_res(fail_del == 0, "it should return success but return others", &fail_cnt);
    
    check_res(list.size == 0, "list.size isn't 0 after del all vals", &fail_cnt);
    check_res(list.head == NULL, "list.head isn't 0 after del all vals", &fail_cnt);
    check_res(list.tail == NULL, "list.tail isn't 0 after del all vals", &fail_cnt);
    
    // del empty list
    int fail_del_empty = 0;
    int test_cnt_del_empty = 10;
    for (int i = 0; i < test_cnt_del_empty; i++) {
        if (hm_list_del_tail(&list) != hm_list_ret_none) {
            fail_del_empty++;
        }
    }
    
    check_res(fail_del_empty == 0, "it should return `hm_list_ret_none` when del empty list, but return others", &fail_cnt);
    
    hm_list_free(&list);
    
    
    print_end("DEL TAIL TEST | TYPE: [INT]", fail_cnt);
    

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


    print_run("DEL INDEX TEST | TYPE: [INT]");

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

    test_list_intergrity(&list, &fail_cnt);
    check_res(fail_del_empty == 0, "the return tag of del function isn't `hm_list_ret_none` when del invalid index", &fail_cnt);
    check_res(fail_del_exist == 0, "the val still existed in list when del this val", &fail_cnt);

    hm_list_free(&list);

    print_end("DEL INDEX TEST | TYPE: [INT]", fail_cnt);


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


    print_run("FREE TEST | TYPE: [INT]");



    hm_list_free(&list);
    check_res(list.size == 0, "list.size isn't 0 after free all list", &fail_cnt);
    test_list_intergrity(&list, &fail_cnt);
    
    // double free
    hm_list_free(&list);
    check_res(list.size == 0, "list.size isn't 0 after double free all list", &fail_cnt);
    test_list_intergrity(&list, &fail_cnt);

    print_end("FREE TEST | TYPE: [INT]", fail_cnt);


}


void test_list_insert_tail_stress() {
    int v = 666666;
    hm_list list;
    hm_list_init(&list, NULL);
    size_t nums[] = {500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums) / sizeof(size_t);

    print_run("INSERT TAIL STRESS TEST | TYPE: [INT]");
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
        print_run_time("INSERT", start, end, nums[i], nums[i]);
        check_res(suc == list.size, "the list.size is wrong when insert many vals", &fail_cnt);
        hm_list_free(&list);
    }

    print_end("INSERT TAIL STRESS TEST | TYPE: [INT]", fail_cnt);



}

void test_list_insert_head_stress() {
    int v = 666666;
    hm_list list;
    hm_list_init(&list, NULL);
    size_t nums[] = {500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums) / sizeof(size_t);

    print_run("INSERT HEAD STRESS TEST | TYPE: [INT]");
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
        print_run_time("INSERT", start, end, nums[i], nums[i]);
        check_res(suc == list.size, "the list.size is wrong when insert many vals", &fail_cnt);
        hm_list_free(&list);
    }

    print_end("INSERT HEAD STRESS TEST | TYPE: [INT]", fail_cnt);
}


void test_list_insert_index_stress() {
    int v = 666666;
    hm_list list;
    hm_list_init(&list, NULL);

    // insert head
    size_t nums_head[] = {500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums_head) / sizeof(size_t);

    print_run("INSERT INDEX(HEAD) STRESS TEST | TYPE: [INT]");
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
        print_run_time("INSERT", start, end, nums_head[i], nums_head[i]);
        check_res(suc == list.size, "the list.size is wrong when insert many vals", &fail_cnt);
        hm_list_free(&list);
    }

    print_end("INSERT INDEX(HEAD) STRESS TEST | TYPE: [INT]\n", fail_cnt);


    // insert tail
    size_t nums_tail[] = {500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    cnt = sizeof(nums_tail) / sizeof(size_t);

    print_run("INSERT INDEX(TAIL) STRESS TEST | TYPE: [INT]");
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

    print_end("INSERT INDEX(TAIL) STRESS TEST | TYPE: [INT]\n", fail_cnt);
    



    // insert ++list.size - 1++

    size_t nums_tail_sub_1[] = {500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    cnt = sizeof(nums_tail_sub_1) / sizeof(size_t);

    print_run("INSERT INDEX(TAIL - 1) STRESS TEST | TYPE: [INT]");
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

    print_end("INSERT INDEX(TAIL - 1) STRESS TEST | TYPE: [INT]\n", fail_cnt);
    


    // insert half of list.size
    size_t nums_mid[] = {500, 1000, 5000, 10000, 50000, 100000};
    cnt = sizeof(nums_mid) / sizeof(size_t);

    print_run("INSERT INDEX(MID) STRESS TEST | TYPE: [INT]");
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

    print_end("INSERT INDEX(MID) STRESS TEST | TYPE: [INT]\n", fail_cnt);

}


void test_list_get_stress() {   
    int v = 666666;
    int fail_cnt = 0;
    hm_list list;
    hm_list_init(&list, NULL);



    // get head
    print_run("GET HEAD STRESS TEST | TYPE: [INT]");
    // the numbers of list cann't too big
    size_t nums_head[] = {500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
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
        check_res(fail_get == 0, "the value that got by get function is NULL when get head stress test", &fail_cnt);
        hm_list_free(&list);
        print_run_time("GET", start, end, nums_head[i], oper_cnt);
    }
    print_end("GET HEAD STRESS TEST | TYPE: [INT]\n", fail_cnt);
    
    fail_cnt = 0;



    // get tail
    print_run("GET TAIL STRESS TEST | TYPE: [INT]");
    size_t nums_tail[] = {500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
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
    print_end("GET TAIL STRESS TEST | TYPE: [INT]\n", fail_cnt);
    
    
    
    // get mid
    print_run("GET MID STRESS TEST | TYPE: [INT]");
    
    // the nums cann't to big because it's time complexity is O(n^2)
    size_t nums_mid[] = {500, 1000, 5000, 10000, 50000};
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
    print_end("GET MID STRESS TEST | TYPE: [INT]", fail_cnt);
}

void test_list_del_head_stress() {
    int fail_cnt = 0;
    int v = 666666;
    hm_list list;
    hm_list_init(&list, NULL);
    size_t nums[] = {500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums) / sizeof(size_t);

    print_run("DEL HEAD STRESS TEST | TYPE: [INT]");
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
        check_res(fail_del == 0, "the tag of return isn't suc when run stressful del head test", &fail_cnt);
        print_run_time("DEL", start, end, nums[i], nums[i]);
        hm_list_free(&list);

    }
    print_end("DEL HEAD STRESS TEST | TYPE: [INT]", fail_cnt);
}

void test_list_del_tail_stress() {
    int fail_cnt = 0;
    int v = 666666;
    hm_list list;
    hm_list_init(&list, NULL);
    size_t nums[] = {500, 1000, 5000, 10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    int cnt = sizeof(nums) / sizeof(size_t);

    print_run("DEL TAIL STRESS TEST | TYPE: [INT]");
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
        check_res(fail_del == 0, "the tag of return isn't suc when run stressful del tail test", &fail_cnt);
        print_run_time("DEL", start, end, nums[i], nums[i]);
        hm_list_free(&list);

    }
    print_end("DEL TAIL STRESS TEST | TYPE: [INT]", fail_cnt);
}


int main()
{
    
    test_list_init();           printf("\n");
    
    test_list_insert_head();    printf("\n");
    test_list_insert_tail();    printf("\n");
    test_list_insert_index();   printf("\n");
    
    test_iter_list();           printf("\n");

    test_list_get();            printf("\n");

    test_list_change();         printf("\n");

    test_list_del_head();       printf("\n");
    test_list_del_tail();       printf("\n");
    test_list_del_index();      printf("\n");

    test_list_free();           printf("\n");

    test_list_insert_tail_stress();  printf("\n");

    test_list_insert_head_stress();  printf("\n");

    test_list_insert_index_stress();                printf("\n");

    test_list_get_stress();                         printf("\n");

    test_list_del_head_stress();                    printf("\n");

    test_list_del_tail_stress();                    printf("\n");
    
    srand(time(NULL));
    return 0;
}