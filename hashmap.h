#pragma once

#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashMap HashMap;
typedef size_t hash_func_t(void *, size_t);
typedef void *copy_key_func_t(void *);
typedef bool compare_keys_func_t(void *, void *);

size_t hash_int(void *key, size_t capacity);
size_t hash_string(void *key, size_t capacity);

void *copy_int_key(void *key);
void *copy_string_key(void *key);

bool compare_int_keys(void *a, void *b);
bool compare_string_keys(void *a, void *b);

HashMap *createMap(size_t capacity, hash_func_t *hash, compare_keys_func_t *compare, copy_key_func_t *copy);
HashMap *createStringMap(size_t capacity);
HashMap *createIntMap(size_t capacity);

void insertMap(HashMap *map, void *key, void *value);
void eraseMap(HashMap *map, void *key);
void *searchMap(HashMap *map, void *key);
void *firstMap(HashMap *map);
void *nextMap(HashMap *map);

void enlargeMap(HashMap *map);
int sizeOfMap(HashMap *map);
