/* The hash_table is taken from Mhys/DataStructures/hash_fnv1a.c */
/* So, this code is basically duck taped together */
/* Shouldn't be used anywhere! I just did it to 'pass' the problem */
/* It's ugly. Instead check out the reference code: hash_fnv1a.c */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

typedef struct hash_ll
{
        struct hash_ll *next;
        struct hash_ll *prev;

        uint64_t key;
        uint64_t value;
} hash_ll;

/* meta-data */
static hash_ll **hash_table = NULL;
static hash_ll **hash_table_tmp = NULL;
static size_t hash_table_size = 0;
static size_t hash_table_buckets = 0;
static int hash_table_lf = 0; // load factor percentage (%)

static const int hash_table_min_buckets = 64;
static const int hash_table_lf_lower_limit = 25;
static const int hash_table_lf_upper_limit = 75;

static const int hash_table_gf = 100; // growth factor percentage(%)

static const uint64_t fnv_prime = 0x00000100000001B3;
static const uint64_t fnv_offset_basis = 0xCBF29CE484222325;

int hash_insert(uint64_t key, uint64_t value);
int hash_search(uint64_t key, uint64_t *res);
int hash_delete(uint64_t key);

void hash_init();
void hash_maintain();

static hash_ll *hash_ll_find(hash_ll *list, uint64_t key)
{
        if (!list) {
                return NULL;
        }

        // linear search
        hash_ll *iter = list;

        while(iter) {
                if (iter->key == key) {
                        break;
                }

                iter = iter->next;
        }

        return iter;
}

uint64_t hash_fnv1a(uint64_t key)
{
    uint64_t hashed_key = fnv_offset_basis;

    for (int i = 0; i < sizeof(key); i++) {
        hashed_key = hashed_key ^ (key & 0xFF);
        hashed_key = hashed_key * fnv_prime;
        key = key >> 8;
    }

    return hashed_key;
}

size_t hash_bucket(uint64_t key)
{
        // get hash value
        uint64_t hashed = hash_fnv1a(key);

        // use simple modulo
        size_t bucket = hashed % hash_table_buckets;

        return bucket;
}

void hash_init()
{
        if (hash_table) {
                return;
        }

        hash_table_buckets = hash_table_min_buckets;
        hash_table = (hash_ll**) calloc(hash_table_buckets, sizeof(hash_ll*));
}

void hash_maintain()
{
        if (hash_table_tmp) {
                return;
        }

        hash_table_lf = (hash_table_size * 100) / hash_table_buckets;

        size_t new_buckets = hash_table_buckets;
        size_t change = (hash_table_buckets * hash_table_gf) / 100;

        if (hash_table_lf_upper_limit < hash_table_lf) {
                new_buckets = hash_table_buckets + change;
        } else if (hash_table_lf < hash_table_lf_lower_limit) {
                new_buckets = hash_table_buckets - change;
        } else {
                return;
        }

        if (new_buckets < hash_table_min_buckets) {
                return;
        }

        if (new_buckets == hash_table_buckets) {
                return;
        }

        // save the old table
        hash_table_tmp = hash_table;
        size_t hash_table_tmp_buckets = hash_table_buckets;

        // resize and reset
        hash_table = (hash_ll**) calloc(new_buckets, sizeof(hash_ll*));
        hash_table_size = 0;
        hash_table_buckets = new_buckets;

        // rehashing
        for (size_t i = 0; i < hash_table_tmp_buckets; i++) {
                hash_ll *iter = hash_table_tmp[i];

                while (iter) {
                        hash_insert(iter->key, iter->value);
                        iter = iter->next;
                }
        }

        hash_table_lf = (hash_table_size * 100) / hash_table_buckets;

        // free the old table
        for (int i = 0; i < hash_table_tmp_buckets; i++) {
                hash_ll *iter = hash_table_tmp[i];

                while (iter) {
                        hash_ll *to_be_freed = iter;
                        iter = iter->next;

                        free(to_be_freed);
                }
        }
        free(hash_table_tmp);
        hash_table_tmp = NULL;
}

int hash_insert(uint64_t key, uint64_t value)
{
        size_t bucket = hash_bucket(key);
        hash_ll *found = hash_ll_find(hash_table[bucket], key);

        if (found) {
                found->value = value;
        } else {
                hash_ll *new_pair = (hash_ll*) malloc(sizeof(hash_ll));

                if (!new_pair) {
                        fprintf(stderr, "malloc(%lu)\n", sizeof(hash_ll));
                        return -1;
                }

                new_pair->next = hash_table[bucket];
                new_pair->prev = NULL;
                new_pair->key = key;
                new_pair->value = value;

                if (hash_table[bucket]) {
                        hash_table[bucket]->prev = new_pair;
                }

                hash_table[bucket] = new_pair;
                hash_table_size++;
        }

        hash_maintain();

        return 0;
}

int hash_search(uint64_t key, uint64_t *value)
{
        size_t bucket = hash_bucket(key);
        hash_ll *found = hash_ll_find(hash_table[bucket], key);

        if (!found) {
                return -1;
        }

        if (value) {
                *value = found->value;
        }

        return 0;
}

int hash_delete(uint64_t key)
{
        size_t bucket = hash_bucket(key);
        hash_ll *found = hash_ll_find(hash_table[bucket], key);

        if (!found) {
                return -1;
        }

        // check if head
        if (hash_table[bucket] == found) {
                hash_table[bucket] = found->next;
        }

        if (found->prev) {
                found->prev->next = found->next;
        }

        if (found->next) {
                found->next->prev = found->prev;
        }

        if (hash_table_size) {
                hash_table_size--;
        }

        free(found);

        hash_maintain();

        return 0;
}

/* Below functions are LeetCode specific */

typedef struct MyHashMap {
        hash_ll **table;
} MyHashMap;

MyHashMap* myHashMapCreate()
{
        hash_init();

        MyHashMap *ret = (MyHashMap*) malloc(sizeof(MyHashMap*));
        ret->table = hash_table;

        return ret;
}

void myHashMapPut(MyHashMap* obj, int key, int value)
{
        hash_insert(key, value);
}

int myHashMapGet(MyHashMap* obj, int key)
{
        int res = 0;
        uint64_t value = 0;

        res = hash_search(key, &value);
        if (res == -1) {
                return res;
        }

        return (int) value;
}

void myHashMapRemove(MyHashMap* obj, int key) {
        hash_delete(key);
}

void myHashMapFree(MyHashMap* obj) {
        if (!hash_table) {
                return;
        }

        for (size_t i = 0; i < hash_table_buckets; i++) {
                hash_ll *iter = hash_table[i];

                while (iter) {
                        hash_ll *to_be_freed = iter;
                        iter = iter->next;
                        free(to_be_freed);
                }
        }

        free(hash_table);

        hash_table = NULL;
        hash_table_buckets = 0;
        hash_table_size = 0;
        hash_table_lf = 0;

        free(obj);
}

/**
 * Your MyHashMap struct will be instantiated and called as such:
 * MyHashMap* obj = myHashMapCreate();
 * myHashMapPut(obj, key, value);
 
 * int param_2 = myHashMapGet(obj, key);
 
 * myHashMapRemove(obj, key);
 
 * myHashMapFree(obj);
*/