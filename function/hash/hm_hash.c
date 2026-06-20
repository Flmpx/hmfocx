/*
 * Copyright (c) 2026 Flmpx
 * Licensed under MIT (see LICENSE).
 */
#include "hm_hash.h"

/**
 * hash type of `int`
 */
size_t hash_int_1(void* ptr) {
    unsigned int key = (unsigned int)(*(int*)ptr);
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = ((key >> 16) ^ key) * 0x45d9f3b;
    key = (key >> 16) ^ key;
    return (size_t)key;
}

static const size_t mod = 1000000009;
static const size_t base = 131;

/**
 * hash type of `string`
 */
size_t hash_str_1(void* ptr) {
    char* key = (char*)ptr;
    size_t hash_res = 0;
    for (size_t i = 0; key[i]; i++) {
        hash_res = (hash_res * base + key[i] + 199) % mod;
    }
    return hash_res;
}