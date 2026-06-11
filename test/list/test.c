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
    check_res(list_2.head == NULL, "list.head isn't `NULL`", &fail_cnt);
    check_res(list_2.tail == NULL, "list.tail isn't `NULL`", &fail_cnt);
    check_res(list_2.size == 0, "list.size isn't 0", &fail_cnt);
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
        
        hm_list_ret ret = hm_list_insert_index(&list, v, index[i]);
        
        if (index[i] <= hm_list_size(&list)) {
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

}

void test_list_del_tail() {

}

void test_list_del_index() {

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
    srand(time(NULL));
    return 0;
}