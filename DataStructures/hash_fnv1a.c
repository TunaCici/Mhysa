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
static hash_ll **hash_table_tmp = NULL;
static size_t hash_table_size = 0;
static size_t hash_table_buckets = 0;
static int hash_table_lf = 0; // load factor percentage (%)

static const int hash_table_min_buckets = 8;
static const int hash_table_lf_lower_limit = 25;
static const int hash_table_lf_upper_limit = 75;

static const int hash_table_gf = 100; // growth factor percentage(%)

static const uint64_t fnv_prime = 0x00000100000001B3;
static const uint64_t fnv_offset_basis = 0xCBF29CE484222325;

int hash_insert(const char *key, uint64_t value);
int hash_search(const char *key, uint64_t *res);
int hash_delete(const char *key);

void hash_init();
void hash_maintain();

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

size_t hash_bucket(const char *key)
{
        // get hash value
        uint8_t *bytes = (uint8_t*) key;
        size_t bytes_len = strlen(key);
        uint64_t hashed = hash_fnv1a(bytes, bytes_len);

        // use simple modulo
        size_t bucket = hashed & (hash_table_buckets - 1);

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
        for (int i = 0; i < hash_table_tmp_buckets; i++) {
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

int hash_insert(const char *key, uint64_t value)
{
        if (!key || HASH_LL_KEY_SIZE < (strlen(key) + 1)) {
                return -1;
        }

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
                strncpy(new_pair->key, key, HASH_LL_KEY_SIZE);
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

int hash_search(const char *key, uint64_t *res)
{
        if (!key) {
                return -1;
        }

        size_t bucket = hash_bucket(key);
        hash_ll *found = hash_ll_find(hash_table[bucket], key);

        if (!found) {
                return -1;
        }

        if (res) {
                *res = found->value;
        }

        return 0;
}

int hash_delete(const char *key)
{
        if (!key) {
                return -1;
        }

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

void print_hash_table()
{
        for (int i = 0; i < hash_table_buckets; i++) {
                printf("[%d] ", i);

                hash_ll *iter = hash_table[i];
                while (iter) {
                        printf("%s: %llu ", iter->key, iter->value);
                        iter = iter->next;
                }

                printf("NULL\n");
        }
        
        printf(
                "\n"
                "size: %lu\n"
                "buckets: %lu\n"
                "load factor: %d\n",
                hash_table_size, hash_table_buckets, hash_table_lf
        );
}

inline uint64_t clock_gettime_nsec()
{
#if defined(__APPLE__) || defined (__MACH__)
        return clock_gettime_nsec_np(CLOCK_UPTIME_RAW);
#elif defined(__linux__) 
        struct timespec t1;
        clock_gettime(CLOCK_MONOTONIC, &t1);
        return t1.tv_nsec;
#else
        #error "unsupported platform"
#endif
}

void handle_insert(const char *line)
{
        char key[HASH_LL_KEY_SIZE];
        uint64_t value;

        if (sscanf(line, "%7s %llu", key, &value) == 2) {
                uint64_t t1 = clock_gettime_nsec();
                int res = hash_insert(key, value);
                uint64_t t2 = clock_gettime_nsec();

                if (res) {
                        fprintf(stderr, "failed to insert\n");
                } else {
                        printf("took %0.4f usecs\n", ((t2 - t1) / 1e4));
                }
        } else {
                fprintf(stderr, "malformed line: %s\n", line);
        }
}

void handle_delete(const char *line)
{
        char key[HASH_LL_KEY_SIZE];

        if (sscanf(line, "%7s", key) == 1) {
                uint64_t t1 = clock_gettime_nsec();
                int res = hash_delete(key);
                uint64_t t2 = clock_gettime_nsec();
                
                if (res) {
                        fprintf(stderr, "failed to delete\n");
                } else {
                        printf("took %0.4f usecs\n", ((t2 - t1) / 1e4));
                }
        } else {
                fprintf(stderr, "malformed line: %s\n", line);
        }
}

void handle_search(const char *line)
{
        char key[HASH_LL_KEY_SIZE];
        uint64_t value = 0;

        if (sscanf(line, "%7s", key) == 1) {
                uint64_t t1 = clock_gettime_nsec();
                int res = hash_search(key, &value);
                uint64_t t2 = clock_gettime_nsec();

                if (res) {
                        fprintf(stderr, "\"%s\" does not exist\n", key);
                } else {
                        printf("\"%s\": %llu\n", key, value);
                        printf("took %0.4f usecs\n", ((t2 - t1) / 1e4));
                }
        } else {
                fprintf(stderr, "malformed line: %s\n", line);
        }
}

void handle_help()
{
        printf(
                "available commands:\n"
                "  insert <char[7]> <uint64_t> - insert an item\n"
                "  delete <char[7]>            - delete an item\n"
                "  search <char[7]>            - search for an item\n"
                "  help                        - show this help message\n"
                "  exit                        - exit the program\n"
        );
}

void handle_print()
{
        print_hash_table();
}

int main(int argc, char **argv)
{
        hash_init();

        char line[32];

        printf("(%s) ", __FILE_NAME__);
        while (fgets(line, sizeof(line), stdin)) {
                line[strcspn(line, "\n")] = '\0';

                if (strstr(line, "insert")) {
                        handle_insert(line + strlen("insert"));
                } else if (strstr(line, "delete")) {
                        handle_delete(line + strlen("delete"));
                } else if (strstr(line, "search")) {
                        handle_search(line + strlen("search"));
                } else if (strstr(line, "print")) {
                        handle_print();
                } else if (strstr(line, "help")) {
                        handle_help();
                } else if (strstr(line, "exit")) {
                        return EXIT_SUCCESS;
                } else {
                        fprintf(stderr, "unkown command: %s\n", line);
                }

                printf("(%s) ", __FILE_NAME__);
        }

        return EXIT_SUCCESS;
}