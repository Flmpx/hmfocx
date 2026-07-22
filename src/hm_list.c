/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */
#include "../include/hm_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


size_t hm_list_size(hm_list* list) {
    return list->size;
}


/**
 * Initialize a list
 * @note - Use `hm_list_free()` to free the list
 * @note - If you do `NOT` want the list to free its values,
 * set the `hm_free` function pointer to `NULL`
 */
void hm_list_init(hm_list* list, hm_free free) {
    list->head = list->tail = NULL;
    list->size = 0;
    list->free = free;
}





/**
 * Free a list
 * @note - If the list was initialized without freeing capacity, 
 * this function can't free the values stored in the Nodes
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
 * Get a pointer to the Node at the given `index`
 * @warning - You must ensure the `index` is valid because the function does not check this variable
 */
static hm_listnode* hm_list_get_node(hm_list* list, size_t index) {

    hm_listnode* cur = NULL;
    size_t cnt = 0;

    // perf the logic about finding target Node
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
 * Insert a value at the head of the list
 * @note - This function will return `hm_list_ret_error` when insertion fails
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
 * Insert a value at the tail of the list
 * @note - This function will return `hm_list_ret_error` when insertion fails
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
 * Insert a value at the given `index`
 * @note - `Index` must be >= `0`, and <= `the size of list`
 * 
 * @note - This function will return `hm_list_ret_warn` when the `index` is out of bounds
 * 
 * @note - Also, this function will return `hm_list_ret_error` when insertion fails
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
 * Delete the Node at the head of the list
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
 * Delete the Node at the tail of the list
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
 * Delete the Node at the specified `index`
 * @note - `Index` must be >= `0`, and < `the size of list`
 * 
 * @note - This function will return `hm_list_ret_none` when the `index` is out of bounds
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
 * Get a pointer to the value at the given `index`
 * @note - `Index` must be >= `0`, and < `the size of list`
 * @note - This function will return nullptr when the `index ` is out of bounds
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
void hm_list_iter_init(hm_list_iter* iter, hm_list* list) {
    iter->cur = list->head;
}



/**
 * Check if the iterator has a next element
 * @note - Return true if iterator has next
 */
bool hm_list_iter_has_next(hm_list_iter* iter) {
    return iter->cur != NULL;
}

/**
 * Get next value of list
 * @note - Use `hm_list_iter_has_next()` to check before calling `hm_list_iter_next()`
 */
void* hm_list_iter_next(hm_list_iter* iter) {
    hm_listnode* cur = iter->cur;

    if (cur == NULL) {
        return NULL;
    }

    void* val = cur->val;

    iter->cur = cur->next;

    return val;
}

/**
 * Initialize iterator of list
 * @note - Let the iterator point to the head of the list
 */
void hm_list_iter_init_head(hm_list_iter* iter, hm_list* list) {
    iter->cur = list->head;
}


/**
 * Initialize iterator of list
 * @note - Let the iterator point to the tail of the list
 */
void hm_list_iter_init_tail(hm_list_iter* iter, hm_list* list) {
    iter->cur = list->tail;
}


/**
 * Initialize iterator of list
 * @note - Let the iterator point to the specified `index` of the list
 * @note - Iterator will point to `NULL` if `index` is out of bounds
 */
void hm_list_iter_init_index(hm_list_iter* iter, hm_list* list, size_t index) {
    if (index >= list->size) {
        iter->cur = NULL;
    } else {
        iter->cur = hm_list_get_node(list, index);
    }
}


/**
 * Check if the iterator's current pointer is valid
 */
bool hm_list_iter_has_cur(hm_list_iter* iter) {
    return iter->cur != NULL;
}

/**
 * Get the current value of the iterator
 * Use `hm_list_iter_has_cur()` to check before calling `hm_list_iter_cur()`
 */
void* hm_list_iter_cur(hm_list_iter* iter) {
    hm_listnode* cur = iter->cur;
    if (cur) {
        return cur->val;
    } else {
        return NULL;
    }
}

/**
 * Move the iterator's pointer to next
 */
void hm_list_iter_move_next(hm_list_iter* iter) {
    hm_listnode* cur = iter->cur;

    if (cur == NULL) {
        return;
    }
    
    iter->cur = cur->next;
}

/**
 * Move the iterator's pointer to prev
 */
void hm_list_iter_move_prev(hm_list_iter* iter) {
    hm_listnode* cur = iter->cur;

    if (cur == NULL) {
        return;
    }

    iter->cur = cur->prev;
}


/**
 * Take the first `n` Node from the head and form a separate sub-list
 * @note - It will return the pointer to head Node of the remaining part
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
 * @note - Return the pointer to last Node of the merged list
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
 * @note - Pass a comparison function to this function
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
