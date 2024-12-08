/*
 * Basic C implementation of a hash table using FNV1a as the hash function.
 * 
 * key: null-terminated ASCII string (max 7 length)
 * value: uint64_t
 *
 * Tested on macOS and Linux systems.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <time.h>

#define HASH_LL_KEY_SIZE 8u

typedef struct hash_ll
{
        struct hash_ll *next;
        struct hash_ll *prev;

        char key[HASH_LL_KEY_SIZE];
        uint64_t value;
} hash_ll;

/* meta-data */
static hash_ll **hash_table = NULL;
static size_t hash_table_size = 0;
static size_t hash_table_buckets = 32;
static int hash_table_lf = 0; // load factor percentage (%)

static const int hash_table_lf_limit = 75;
static const int hash_table_gf = 50; // growth factor percentage(%)

static const uint64_t fnv_prime = 0x00000100000001B3;
static const uint64_t fnv_offset_basis = 0xCBF29CE484222325;

int hash_insert(const char *key, int value);
int hash_search(const char *key, int *res);
int hash_delete(const char *key);

static hash_ll *hash_ll_find(hash_ll *list, const char *key)
{
        if (!list) {
                return NULL;
        }

        // linear search
        hash_ll *iter = list;

        while(iter) {
                if (!strcmp(iter->key, key)) {
                        break;
                }

                iter = iter->next;
        }

        return iter;
}

uint64_t hash_fnv1a(uint8_t *bytes, size_t bytes_len)
{
        uint64_t hashed = fnv_offset_basis;

        for (int i = 0; i < bytes_len; i++) {
                hashed = hashed ^ bytes[i];
                hashed = hashed * fnv_prime;
        }

        return hashed;
}

void hash_init()
{
        if (hash_table) {
                return;
        }

        hash_table = (hash_ll**) calloc(hash_table_buckets, sizeof(hash_ll*));
}

int hash_insert(const char *key, int value)
{
        if (!key || HASH_LL_KEY_SIZE < (strlen(key) + 1)) {
                return -1;
        }

        // get hash value
        uint8_t *bytes = (uint8_t*) key;
        size_t bytes_len = strlen(key);
        uint64_t hashed = hash_fnv1a(bytes, bytes_len);

        size_t bucket = hashed & (hash_table_buckets - 1);
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
                strncpy(new_pair->key, key, HASH_LL_KEY_SIZE);
                new_pair->value = value;

                if (hash_table[bucket]) {
                        hash_table[bucket]->prev = new_pair;
                }

                hash_table[bucket] = new_pair;
                hash_table_size++;
        }

        // maintain the table
        hash_table_lf = (hash_table_size * 100) / hash_table_buckets;

        if (hash_table_lf_limit < hash_table_lf) {
                printf("resize\n");
        }

        return 0;
}

int hash_search(const char *key, int *res)
{
        return 0;
}

int hash_delete(const char *key)
{
        return 0;
}

void try_to_insert(const char *key, uint64_t value)
{
        if (hash_insert(key, value)) {
                fprintf(stderr, "faled to insert\n");
        }
}

void print_hash_table()
{
        for (int i = 0; i < hash_table_buckets; i++) {
                printf("hash_table_[%d] ", i);

                hash_ll *iter = hash_table[i];
                while (iter) {
                        printf("%s: %llu ", iter->key, iter->value);
                        iter = iter->next;
                }

                printf("NULL\n");
        }

        printf("load factor: %d\n", hash_table_lf);
}

int main(int argc, char **argv)
{
        hash_init();

        char line[32];
        char key[HASH_LL_KEY_SIZE];
        uint64_t value;

        while (fgets(line, sizeof(line), stdin)) {
                line[strcspn(line, "\n")] = '\0';

                if (sscanf(line, "%7s %llu", key, &value) == 2) {
                        try_to_insert(key, value);
                } else {
                        fprintf(stderr, "malformed line: %s\n", line);
                }
        }

        print_hash_table();

        return EXIT_SUCCESS;
}