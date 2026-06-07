#include "../../include/hm_list.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#define NUM 50
void get_num(int* a) {
    *a = rand();   
}

void print_list_size(hm_list* list) {
    printf("the size of list is %zu\n", hm_list_size(list));
}

int main()
{
    srand(time(NULL));
    int suc = 0, fail = 0;
    hm_list list;

/**
 * test for initialize list
 */
    printf("initialize list : start\n");
    print_list_size(&list);

    hm_list_init(&list, free);
    
    print_list_size(&list);
    printf("initialize list : end\n\n\n\n");





    // use array named flag to test the correctness of vals in list
    int flag[NUM] = {0};
    int insert_num = 0;



/**
 * test of insert
 */

    printf("insert value in list : start\n");
    print_list_size(&list);

    for (int i = 0; i < NUM; i++) {
        int* val = (int*)malloc(sizeof(int));
        if (val == NULL) {
            break;
        }
        get_num(val);
        
        flag[i] = *val;
        
        if (hm_list_insert_tail(&list, val) != hm_list_ret_suc) {
            break;
        }
        insert_num++;
    }
    printf("number of insert is %d\n", insert_num);
    
    print_list_size(&list);
    printf("insert value in list : end\n\n\n\n");
    




/**
 * test for iterator of list
 */

    printf("test fo iterator of list : start\n");
    print_list_size(&list);

    suc = fail = 0;
    hm_iter_list iter;
    hm_iter_list_init(&iter, &list);
    
    int* val;
    int cnt = 0;
    while (hm_iter_list_has_next(&iter)) {
        val = hm_iter_list_next(&iter);
        if (*val == flag[cnt++]) {
            printf("index : %d , val : %d\n", cnt - 1, *val);
            suc++;
        } else {
            fail++;
        }
    }
    
    printf("success : %d\n", suc);
    printf("failure : %d\n", fail);
    
    print_list_size(&list);
    printf("test fo iterator of list : end\n\n\n\n");


    
/**
 * verify the function of insert and test for get function
 */

    printf("verify the correctness of the process of insert : start\n");
    print_list_size(&list);

    suc = 0; fail = 0;
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
    
    print_list_size(&list);
    printf("verify the correctness of the process of insert : end\n\n\n\n");
    


    
    

    
    
    


/**
 * test for change value
 */

    suc = fail = 0;
    printf("test for fucntion of change value(use get function to change) : start\n");
    print_list_size(&list);

    for (int i = 0; i < NUM; i += 2) {
        int* val = hm_list_get(&list, i);
        
        //prevent int type overflow
        int diff = rand() % 9080;
        
        printf("index: %d, diff: %d\n", i, diff);
        flag[i] += diff;
        *val += diff;

        if (*val == flag[i]) {
            suc++;
        } else {
            fail++;
        }

    }
    printf("success : %d\n", suc);
    printf("failure : %d\n", fail);

    print_list_size(&list);
    printf("test for fucntion of change value(use get function to change) : end\n\n\n");



/**
 * verify the correctness of values in list after change some values
 */


    printf("verify the correctness after change some values : start\n");
    print_list_size(&list);


    suc = fail = 0;
    
    hm_iter_list_init(&iter, &list);
    
    cnt = 0;
    while (hm_iter_list_has_next(&iter)) {
        int* val = hm_iter_list_next(&iter);
        printf("index: %d, val: %d\n", cnt, *val);
        if (*val == flag[cnt]) {
            suc++;
        } else {
            fail++;
        }
        cnt++;
    }
    
    printf("success : %d\n", suc);
    printf("failure : %d\n", fail);

    print_list_size(&list);
    printf("verify the correctness after change some values : end\n\n\n\n");
    
    








    
    
/**
 * test for del
 */
    suc = fail = 0;
    printf("test for del val in list : start\n");
    print_list_size(&list);

    for (int i = 0; i < NUM / 2; i++) {
        if (hm_list_del_head(&list) == hm_list_ret_suc) {
            suc++;
        } else {
            fail++;
        }
        
    }
    
    printf("success : %d\n", suc);
    printf("failure : %d\n", fail);
    
    hm_iter_list_init(&iter, &list);
    
    print_list_size(&list);
    printf("test for del val in list : end\n\n\n\n");
    
    

/**
 * verify the correctness of values in list after del some values
 */

    printf("verify the correctness after del some values: start\n");
    print_list_size(&list);


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
    
    print_list_size(&list);
    printf("verify the correctness after del some values: end\n\n\n\n");
    



    hm_list_free(&list);

    return 0;
}