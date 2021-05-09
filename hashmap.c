#include "hashmap.h"

typedef struct Pair Pair;
typedef struct HashMap HashMap;

struct Pair
{
    void *key;
    void *value;
};

struct HashMap
{
    Pair **buckets;
    size_t size;
    size_t capacity;
    size_t current;
    hash_func_t *hash_func;
    copy_key_func_t *copy_key_func;
    compare_keys_func_t *compare_keys_func;
};

Pair *createPair(HashMap *map, void *key, void *value)
{
    Pair *new_pair = (Pair *)malloc(sizeof(Pair));
    new_pair->key = map->copy_key_func(key); //clave
    new_pair->value = value;                 //valor asociado
    return (new_pair);
}

HashMap *createMap(size_t capacity, hash_func_t *hash, compare_keys_func_t *compare, copy_key_func_t *copy)
{
    HashMap *map = malloc(sizeof(HashMap));
    map->buckets = (Pair **)calloc(capacity, sizeof(Pair));
    map->capacity = capacity;
    map->current = -1;
    map->size = 0;
    map->hash_func = hash;
    map->copy_key_func = copy;
    map->compare_keys_func = compare;

    return map;
}

HashMap *createStringMap(size_t capacity)
{
    HashMap *result = createMap(capacity, hash_string, compare_string_keys, copy_string_key);
    return (result);
}

HashMap *createIntMap(size_t capacity)
{
    HashMap *result = createMap(capacity, hash_int, compare_int_keys, copy_int_key);
    return (result);
}

void insertMap(HashMap *map, void *key, void *value)
{
    size_t index = map->hash_func(key, map->capacity);

    size_t steps = 0;
    while (map->buckets[index] != NULL && steps < map->capacity)
    {
        index++;
        if (map->capacity <= index)
        {
            index = 0;
        }
        ++steps;
    }

    /*
    if (steps >= map->capacity)
    {
        enlargeMap(map);
    }
    */

    Pair *element = createPair(map, key, value);
    map->buckets[index] = element;
    map->size++;

    /*
    if (((float)map->size) / ((float)map->capacity) >= 0.7f)
    {
        enlargeMap(map);
    }
    */
}

void eraseMap(HashMap *map, void *key)
{
    size_t index = map->hash_func(key, map->capacity);
    while (!map->compare_keys_func(map->buckets[index]->key, key))
    {
        index++;
        if (map->buckets[index] == NULL)
        {
            return;
        }
        if (map->capacity <= index)
        {
            index = 0;
        }
    }

    free(map->buckets[index]->key);
    map->buckets[index]->key = NULL;
    map->size--;
}

void *searchMap(HashMap *map, void *key)
{
    size_t index = map->hash_func(key, map->capacity);
    if (map->buckets[index] == NULL)
    {
        return (NULL);
    }

    while (!map->compare_keys_func(map->buckets[index]->key, key))
    {
        index++;
        if (map->buckets[index] == NULL)
        {
            return NULL;
        }
        if (map->capacity <= index)
        {
            index = 0;
        }
    }

    map->current = index;
    return map->buckets[index]->value;
}

void *firstMap(HashMap *map)
{
    size_t index;
    for (index = 0; index < map->capacity; index++)
    {
        if (map->buckets[index] != NULL)
        {
            if (map->buckets[index]->key != NULL)
            {
                map->current = index;
                return map->buckets[index]->value;
            }
        }
    }
    return NULL;
}

void *nextMap(HashMap *map)
{
    size_t index;
    int cont = 0;
    for (index = map->current; index < map->capacity; index++)
    {
        if (map->buckets[index] != NULL)
        {
            if (map->buckets[index]->key != NULL)
            {
                map->current = index;
                cont++;
                if (cont == 2)
                {
                    return map->buckets[index]->value;
                }
            }
        }
    }
    return NULL;
}

void enlargeMap(HashMap *map)
{
    int capacidad_antigua = map->capacity;
    Pair **old_buckets = map->buckets;
    map->capacity *= 2;
    map->buckets = (Pair **)calloc(map->capacity, sizeof(Pair));
    map->size = 0;

    for (int i = 0; i < capacidad_antigua; i++)
    {
        if (old_buckets[i] != NULL)
        {
            if (old_buckets[i]->key != NULL)
            {
                map->current = i;
                insertMap(map, old_buckets[i]->key, old_buckets[i]->value);
            }
        }
    }

    free(old_buckets);
}

int sizeOfMap(HashMap *map)
{
    return (map->size);
}

// Sample hash functions.
size_t hash_int(void *key, size_t capacity)
{
    int key_int = *(int *)key;
    return ((size_t)key_int % capacity);
}

size_t hash_string(void *key, size_t capacity)
{
    char *key_str = (char *)key;

    size_t hash = 0;
    char *ptr;
    for (ptr = key_str; *ptr != '\0'; ptr++)
    {
        hash += hash * 32 + *ptr;
    }
    return (hash % capacity);
}

// Sample copy functions
void *copy_int_key(void *key)
{
    int *result = malloc(sizeof(int));
    *result = *(int *)key;
    return (result);
}

void *copy_string_key(void *key)
{
    char *result = strdup((const char *)key);
    return (result);
}

// Sample compare functions
bool compare_int_keys(void *a, void *b)
{
    int int_a = *(int *)a;
    int int_b = *(int *)b;
    bool result = int_a == int_b;
    return (result);
}

bool compare_string_keys(void *a, void *b)
{
    char *str_a = (char *)a;
    char *str_b = (char *)b;
    return (strcmp(str_a, str_b) == 0);
}
