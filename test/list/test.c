#include "../../include/hm_list.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM 10
void get_num(int* a) {
    *a = rand();   
}

int main()
{
    hm_list list;
    hm_list_init(&list, free);
    int flag[NUM] = {0};
    int insert_num = 0;
    for (int i = 0; i < NUM; i++) {
        int* val = (int*)malloc(sizeof(int));
        if (val == NULL) {
            break;
        }
        get_num(val);

        flag[i] = *val;

        if (hm_list_insert_tail(&list, val) != hm_success) {
            break;
        }
        insert_num++;
    }
    //auth
    printf("number of insert is %d\n", insert_num);

    int suc = 0, fail = 0;
    for (int i = 0; i < insert_num; i++) {
        int* val = hm_list_get(&list, i);
        if (val == NULL || *val != flag[i]) {
            fail++;
        } else {
            printf("index : %d , val : %d\n", i, *val);
            suc++;
        }
    }
    printf("success : %d\n", suc);
    printf("failure : %d\n", fail);
    
    
    suc = fail = 0;
    
    hm_iter_list iter;
    hm_iter_list_init(&iter, &list);
    
    int* val;
    int cnt = 0;
    while (val = hm_iter_list_next(&iter)) {
        if (*val == flag[cnt++]) {
            printf("index : %d , val : %d\n", cnt - 1, *val);
            suc++;
        } else {
            fail++;
        }
    }

    printf("success : %d\n", suc);
    printf("failure : %d\n", fail);
    


    suc = fail = 0;

    for (int i = 0; i < NUM / 2; i++) {
        if (hm_list_del_head(&list) == hm_success) {
            suc++;
        } else {
            fail++;
        }

    }

    printf("success : %d\n", suc);
    printf("failure : %d\n", fail);

    hm_iter_list_init(&iter, &list);



    suc = fail = 0;
    cnt = 0;

    while (val = hm_iter_list_next(&iter)) {
        if (*val == flag[cnt + NUM / 2]) {
            printf("index : %d , val : %d\n", cnt + NUM / 2, *val);
            suc++;
        } else {
            fail++;
        }
        cnt++;
    }

    printf("success : %d\n", suc);
    printf("failure : %d\n", fail);


    



    hm_list_free(&list);

    return 0;
}