#ifndef HM_LIST_H
#define HM_LIST_H
#include <stdio.h>





typedef struct hm_listnode {
    struct hm_listnode* prev;
    struct hm_listnode* next;
    void* v;
} hm_listnode;

typedef struct hm_list {
    hm_listnode* head;
    hm_listnode* tail;
    size_t size;
} hm_list;



#endif