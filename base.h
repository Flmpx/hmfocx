#ifndef BASE_H
#define BASE_H


/// @brief result of compare
typedef enum hm_cmp_res {
    less = -1,
    same,
    more
} hm_cmp_res;



/// @brief return information of functions
typedef enum hm_return_info {
    hm_error = 2,
    hm_warning,
    hm_none,
    hm_success
} hm_return_info;



#endif