/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */
#include "../include/hm_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



/**
 * Initialize a list
 * @note - This list can be freed with `hm_list_free()`
 * @note - If you want this list don't have the power of free this value,
 * let the `hm_free` function pointer to be `NULL`
 */
void hm_list_init(hm_list* list, hm_free free) {
    list->head = list->tail = NULL;
    list->size = 0;
    list->free = free;
}





/**
 * Free a list
 * @note - If this list don't have the power of free value,
 * this function can't free the val in hm_listnode
 */
void hm_list_free(hm_list* list) {
    hm_listnode* cur = list->head;
    while (cur) {
        hm_listnode* tmp = cur;
        cur = cur->next;
        if (list->free) {
            list->free(tmp->val);
        }
        free(tmp);
    }
    hm_list_init(list, list->free);
}

/**
 * Get the Node by the `index` that passed in
 * @warning - You have to make sure the `index` is valid because the function don't check this variable
 */
static hm_listnode* hm_list_get_node(hm_list* list, size_t index) {

    hm_listnode* cur = NULL;
    size_t cnt = 0;

    // perf the find logic of target index
    if (index > list->size / 2) {
        cur = list->tail;
        cnt = list->size - index - 1;
        
        while (cnt-- && cur) {
            cur = cur->prev;
        }
        
    } else {
        cur = list->head;
        cnt = index;

        while (cnt-- && cur) {
            cur = cur->next;
        }

    }
    return cur;
}


/**
 * Insert value in the head of list
 * @note - This function will return `hm_list_ret_error` when insert failed
 * 
 */
hm_list_ret hm_list_insert_head(hm_list* list, void* val) {

    hm_listnode* new_node = (hm_listnode*)malloc(sizeof(hm_listnode));
    if (new_node == NULL) {
        return hm_list_ret_error;
    }
    new_node->next = list->head;
    new_node->prev = NULL;
    new_node->val = val;

    if (list->size == 0) {
        list->head = list->tail = new_node;
    } else {
        list->head->prev = new_node;
        list->head = new_node;
    }

    list->size++;
    return hm_list_ret_suc;
}



/**
 * Insert value in the tail of list
 * @note - This function will return `hm_list_ret_error` when insert failed
 */
hm_list_ret hm_list_insert_tail(hm_list* list, void* val) {
    hm_listnode* new_node = (hm_listnode*)malloc(sizeof(hm_listnode));
    if (new_node == NULL) {
        return hm_list_ret_error;
    }

    new_node->next = NULL;
    new_node->prev = list->tail;
    new_node->val = val;

    if (list->size == 0) {
        list->head = list->tail = new_node;
    } else {
        list->tail->next = new_node;
        list->tail = new_node;
    }

    list->size++;
    return hm_list_ret_suc;
}


/**
 * Insert value at the index that you given in this list
 * @note - The range of index must be >= `0`, and <= `the size of list`
 * 
 * @note - This function will return `hm_list_ret_warn` when `index` is invalid
 * 
 * @note - Also, this function will return `hm_list_ret_error` when insert failed
 */
hm_list_ret hm_list_insert_index(hm_list* list, void* val, size_t index) {

    if (index > list->size) {
        return hm_list_ret_warn;
    }

    if (index == 0) return hm_list_insert_head(list, val);
    if (index == list->size) return hm_list_insert_tail(list, val);

    hm_listnode* new_node = (hm_listnode*)malloc(sizeof(hm_listnode));
    if (new_node == NULL) {
        return hm_list_ret_error;
    }
    new_node->val = val;

    // the index is valid when run there
    hm_listnode* cur = hm_list_get_node(list, index);


    new_node->next = cur;
    new_node->prev = cur->prev;

    cur->prev->next = new_node;
    cur->prev = new_node;


    list->size++;

    return hm_list_ret_suc;
}


/**
 * Del hm_listnode with value at head in list
 * @note - This function will return `hm_list_ret_none` when list is empty
 */
hm_list_ret hm_list_del_head(hm_list* list) {
    if (list->size == 0) {
        return hm_list_ret_none;
    }

    hm_listnode* del_node = list->head;

    if (list->free) {
        list->free(del_node->val);
    }

    if (list->size == 1) {
        list->head = list->tail = NULL;
    } else {
        list->head = list->head->next;
        list->head->prev = NULL;
    }

    free(del_node);

    list->size--;

    return hm_list_ret_suc;
}

/**
 * Del hm_listnode with value at end in list
 * @note - This function will return `hm_list_ret_none` when list is empty
 */
hm_list_ret hm_list_del_tail(hm_list* list) {
    if (list->size == 0) {
        return hm_list_ret_none;
    }

    hm_listnode* del_node = list->tail;
    if (list->free) {
        list->free(del_node->val);
    }

    if (list->size == 1) {
        list->head = list->tail = NULL;
    } else {
        list->tail = list->tail->prev;
        list->tail->next = NULL;
    }

    free(del_node);

    list->size--;
    
    return hm_list_ret_suc;
}


/**
 * Del hm_listnode with value at the index that you given in this list
 * @note - The range of index must be >= `0`, and < `the size of list`
 * 
 * @note - This function will return `hm_list_ret_none` when `index` is invalid
 * 
 */
hm_list_ret hm_list_del_index(hm_list* list, size_t index) {
    if (index >= list->size) {
        return hm_list_ret_none;
    }

    if (index == 0) return hm_list_del_head(list);
    if (index == list->size - 1) return hm_list_del_tail(list);

    // the index is valid when run there
    hm_listnode* cur = hm_list_get_node(list, index);

    if (list->free) {
        list->free(cur->val);
    }

    cur->prev->next = cur->next;
    cur->next->prev = cur->prev;

    free(cur);
    
    list->size--;

    return hm_list_ret_suc;

}

/**
 * Get pointer of value by index that you given in a list
 * @note - This function will return nullptr when index is invalid
 */
void* hm_list_get(hm_list* list, size_t index) {
    if (index >= list->size) return NULL;

    hm_listnode* cur = hm_list_get_node(list, index);

    return cur->val;
}


/**
 * Initialize iterator of list
 * 
 */
void hm_iter_list_init(hm_iter_list* iter, hm_list* list) {
    iter->next = list->head;
}



/**
 * Check if the iterator of list has next value
 * @note - Return true if iterator has next
 */
bool hm_iter_list_has_next(hm_iter_list* iter) {
    return iter->next != NULL;
}

/**
 * Get next value of list
 * @note - Please use function of `hm_iter_list_has_next` to check if list has next value
 */
void* hm_iter_list_next(hm_iter_list* iter) {
    hm_listnode* cur = iter->next;

    if (cur == NULL) {
        return NULL;
    }

    void* val = cur->val;

    iter->next = cur->next;

    return val;
}


/**
 * Take the first `n` listnode from the head and form a separate sub-list
 * @note - It will return the head listnode of the remaining part
 */
static hm_listnode* split(hm_listnode* head, size_t n) {
    if (!head) return NULL;
    hm_listnode* cur = head;
    for (size_t i = 1; i < n && cur->next; i++) {
        cur = cur->next;
    }
    hm_listnode* right = cur->next;

    if (right) {
        right->prev = NULL;
    }

    cur->next = NULL;
    head->prev = NULL;

    return right;
}


/**
 * Merge two lists and attach them to the tail 
 * @note - Return the last listnode of the merged list
 */
static hm_listnode* merge(hm_listnode* left, hm_listnode* right, hm_listnode* tail, hm_cmp cmp) {
    hm_listnode* l = left;
    hm_listnode* r = right;

    while (l && r) {
        if (cmp(l->val, r->val) <= 0) {
            tail->next = l;
            l->prev = tail;
            l = l->next;
        } else {
            tail->next = r;
            r->prev = tail;
            r = r->next;
        }
        tail = tail->next;
    }

    if (l) {
        tail->next = l;
        l->prev = tail;
        while (tail->next) tail = tail->next;
    } else if (r) {
        tail->next = r;
        r->prev = tail;
        while (tail->next) tail = tail->next;
    }
    return tail;
}


/**
 * Sort list
 * @note - Pass a comparation function to this function
 */
void hm_list_sort(hm_list* list, hm_cmp cmp) {
    
    hm_listnode* head = list->head;

    if (!head || !head->next) return;

    size_t s = list->size;

    hm_listnode dummy;
    dummy.next = head;
    dummy.prev = NULL;

    head->prev = &dummy;

    hm_listnode* tail;
    hm_listnode* cur;
    for (size_t step = 1; step < s; step <<= 1) {
        tail = &dummy;
        cur = dummy.next;

        while (cur) {
            hm_listnode* left = cur;
            hm_listnode* right = split(left, step);
            cur = split(right, step);

            tail = merge(left, right, tail, cmp);
        }
    }

    dummy.next->prev = NULL;

    list->head = dummy.next;
    list->tail = tail;
}
