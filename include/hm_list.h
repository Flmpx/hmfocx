/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */


/*********************************************************************
 * Latest API Description see https://github.com/Flmpx/hmfocx/blob/main/docs/hm_list.md
 * 
 ********************************************************************/

#ifndef HM_LIST_H
#define HM_LIST_H
#include <stdio.h>
#include "hm_base.h"
#include <stdbool.h>

/**
 * The return signal of function in list
 */
typedef enum hm_list_ret {
    hm_list_ret_error = 0x0,    // Malloc failed
    hm_list_ret_warn,           // The passed parameter is incorret
    hm_list_ret_none,           // Operation invalid, like del start node when list is empty
    hm_list_ret_suc             // Operation successful
} hm_list_ret;

/**
 * The node of list
 */
typedef struct hm_listnode {
    struct hm_listnode* prev;
    struct hm_listnode* next;
    void* val;
} hm_listnode;



/**
 * List with head and tail pointer
 */
typedef struct hm_list {
    hm_listnode* head;
    hm_listnode* tail;
    size_t size;
    hm_free free;
} hm_list;


/**
 * The iterator of list
 */

typedef struct hm_list_iter {
    hm_listnode* cur;
} hm_list_iter;


/**
 * Some simple functions
 */

extern size_t hm_list_size(hm_list* list);


/**
 * Initialize and free function
 */

extern void hm_list_init(hm_list* list, hm_free free);
extern void hm_list_free(hm_list* list);

/**
 * Add functions
 */

extern hm_list_ret hm_list_insert_head(hm_list* list, void* val);
extern hm_list_ret hm_list_insert_tail(hm_list* list, void* val);
extern hm_list_ret hm_list_insert_index(hm_list* list, void* val, size_t index);



/**
 * Delete functions
 */

extern hm_list_ret hm_list_del_head(hm_list* list);
extern hm_list_ret hm_list_del_tail(hm_list* list);
extern hm_list_ret hm_list_del_index(hm_list* list, size_t index);

/**
 * Get functions
 */

extern void* hm_list_get(hm_list* list, size_t index);

/**
 * Iterator functions
 */

// These functions will be remove

extern void hm_list_iter_init(hm_list_iter* iter, hm_list* list);
extern bool hm_list_iter_has_next(hm_list_iter* iter);
extern void* hm_list_iter_next(hm_list_iter* iter);

// New function of iterator

extern void hm_list_iter_init_head(hm_list_iter* iter, hm_list* list);
extern void hm_list_iter_init_tail(hm_list_iter* iter, hm_list* list);
extern void hm_list_iter_init_index(hm_list_iter* iter, hm_list* list, size_t index);

extern bool hm_list_iter_has_cur(hm_list_iter* iter);
extern void* hm_list_iter_cur(hm_list_iter* iter);

extern void hm_list_iter_move_next(hm_list_iter* iter);
extern void hm_list_iter_move_prev(hm_list_iter* iter);

/**
 * Sort functions
 */

extern void hm_list_sort(hm_list* list, hm_cmp cmp);

#endif