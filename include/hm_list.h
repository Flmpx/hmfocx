#ifndef HM_LIST_H
#define HM_LIST_H
#include <stdio.h>
#include "../base.h"
#include <stdbool.h>


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

void hm_list_init(hm_list* list, hm_free free);
void hm_list_free(hm_list* list);

/**
 * add functions
 */

hm_info hm_list_insert_head(hm_list* list, void* val);
hm_info hm_list_insert_tail(hm_list* list, void* val);
hm_info hm_list_insert_index(hm_list* list, void* val, size_t index);



/**
 * delete functions
 */
hm_info hm_list_del_head(hm_list* list);
hm_info hm_list_del_tail(hm_list* list);
hm_info hm_list_del_index(hm_list* list, size_t index);

/**
 * get functions
 */

void* hm_list_get(hm_list* list, size_t index);

/**
 * iterator functions
 */

void hm_iter_list_init(hm_iter_list* iter, hm_list* list);
bool hm_iter_list_has_next(hm_iter_list* iter);
void* hm_iter_list_next(hm_iter_list* iter);


#endif