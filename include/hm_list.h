/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */
#ifndef HM_LIST_H
#define HM_LIST_H
#include <stdio.h>
#include "hm_base.h"
#include <stdbool.h>

/**
 * the return signal of function in list
 */
typedef enum hm_list_ret {
    hm_list_ret_error = 0x0,    // malloc failed
    hm_list_ret_warn,           // the passed parameter is incorret
    hm_list_ret_none,           // operation invalid, like del start node when list is empty
    hm_list_ret_suc             // operation successful
} hm_list_ret;

/**
 * the node of list
 */
typedef struct hm_listnode {
    struct hm_listnode* prev;
    struct hm_listnode* next;
    void* val;
} hm_listnode;



/**
 * list
 */
typedef struct hm_list {
    hm_listnode* head;
    hm_listnode* tail;
    size_t size;
    hm_free free;
} hm_list;


/**
 * the iterator of list
 */

typedef struct hm_iter_list {
    hm_listnode* next;
} hm_iter_list;


/**
 * some simple functions
 */

#define hm_list_size(l) ((l)->size)


/**
 * initialize and free function
 */

extern void hm_list_init(hm_list* list, hm_free free);
extern void hm_list_free(hm_list* list);

/**
 * add functions
 */

extern hm_list_ret hm_list_insert_head(hm_list* list, void* val);
extern hm_list_ret hm_list_insert_tail(hm_list* list, void* val);
extern hm_list_ret hm_list_insert_index(hm_list* list, void* val, size_t index);



/**
 * delete functions
 */

extern hm_list_ret hm_list_del_head(hm_list* list);
extern hm_list_ret hm_list_del_tail(hm_list* list);
extern hm_list_ret hm_list_del_index(hm_list* list, size_t index);

/**
 * get functions
 */

extern void* hm_list_get(hm_list* list, size_t index);

/**
 * iterator functions
 */

extern void hm_iter_list_init(hm_iter_list* iter, hm_list* list);
extern bool hm_iter_list_has_next(hm_iter_list* iter);
extern void* hm_iter_list_next(hm_iter_list* iter);


/**
 * sort functions
 */

extern void hm_list_sort(hm_list* list, hm_cmp cmp);

#endif