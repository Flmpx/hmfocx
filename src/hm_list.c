#include "../include/hm_list.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>



/**
 * Initialize a list, this list can be freed with hm_list_free()
 * if you want this list don't have the power of free this value,
 * let the hm_free function pointer to be NULL
 */
void hm_list_init(hm_list* list, hm_free free) {
    list->head = list->tail = NULL;
    list->size = 0;
    list->free = free;
}





/**
 * Free a list
 * if this list don't have the power of free value,
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
 * Insert value in the head of list
 * this function will return `hm_list_ret_error` when malloc failed
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
 * this function will return `hm_list_ret_error` when malloc failed
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
 * the range of index must be >= `0`, and <= `the size of list`
 * 
 * this function will return `hm_list_ret_warn` when `index` is invalid
 * 
 * Also, this function will return `hm_list_ret_error` when malloc failed
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


    hm_listnode* cur = list->head;


    while (index-- && cur) {
        cur = cur->next;
    }

    new_node->next = cur;
    new_node->prev = cur->prev;

    cur->prev->next = new_node;
    cur->prev = new_node;


    list->size++;

    return hm_list_ret_suc;
}


/**
 * Del hm_listnode with value at head in list
 * this function will return `hm_list_ret_none` when list is empty
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
 * this function will return `hm_list_ret_none` when list is empty
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
 * the range of index must be >= `0`, and < `the size of list`
 * 
 * this function will return `hm_list_ret_none` when `index` is invalid
 * 
 */
hm_list_ret hm_list_del_index(hm_list* list, size_t index) {
    if (index >= list->size) {
        return hm_list_ret_none;
    }

    if (index == 0) return hm_list_del_head(list);
    if (index == list->size - 1) return hm_list_del_tail(list);

    hm_listnode* cur = list->head;
    while (index-- && cur) {
        cur = cur->next;
    }

    cur->prev->next = cur->next;
    cur->next->prev = cur->prev;

    list->size--;

    return hm_list_ret_suc;

}

/**
 * Get pointer of value by index that you given in a list
 * this function will return nullptr when index is invalid
 */
void* hm_list_get(hm_list* list, size_t index) {
    if (index >= list->size) return NULL;

    hm_listnode* cur = list->head;
    
    while (index-- && cur) {
        cur = cur->next;
    }

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
 * Return true if iterator has next
 */
bool hm_iter_list_has_next(hm_iter_list* iter) {
    return iter->next != NULL;
}

/**
 * Get next value of list
 * Please use function of `hm_iter_list_has_next` to check if list has next value
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


