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

typedef struct hm_iter_list {
    hm_listnode* cur;
} hm_iter_list;


/**
 * Some simple functions
 */

#define hm_list_size(l) ((l)->size)


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

extern void hm_iter_list_init(hm_iter_list* iter, hm_list* list);
extern bool hm_iter_list_has_next(hm_iter_list* iter);
extern void* hm_iter_list_next(hm_iter_list* iter);

// New function of iterator

extern void hm_iter_list_init_head(hm_iter_list* iter, hm_list* list);
extern void hm_iter_list_init_tail(hm_iter_list* iter, hm_list* list);
extern void hm_iter_list_init_index(hm_iter_list* iter, hm_list* list, size_t index);

extern bool hm_iter_list_has_cur(hm_iter_list* iter);
extern void* hm_iter_list_cur(hm_iter_list* iter);

extern void hm_iter_list_move_next(hm_iter_list* iter);
extern void hm_iter_list_move_prev(hm_iter_list* iter);

/**
 * Sort functions
 */

extern void hm_list_sort(hm_list* list, hm_cmp cmp);

#endif