/*
 * Basic C implementation of a hash table using FNV1a as the hash function.
 * 
 * Keys are null-terminated ASCII strings
 * Values are simple integers
 *
 * Tested on macOS and Linux systems.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <time.h>

typedef struct hash_ll
{
        struct hash_ll *next;
        struct hash_ll *prev;
        int value;
} hash_ll;

/* meta-data */
static hash_ll *hash_table = NULL;
static int hash_table_size = 0;
static int hash_table_capacity = 0;

static const uint64_t fnv_prime = 0x00000100000001B3;
static const uint64_t fnv_offset_basis = 0xCBF29CE484222325;

uint64_t hash_fnv1a(uint8_t *bytes, size_t bytes_len)
{
        uint64_t hashed = fnv_offset_basis;

        for (int i = 0; i < bytes_len; i++) {
                hashed = hashed ^ bytes[i];
                hashed = hashed * fnv_prime;
        }

        return hashed;
}

int hash_insert(char *key, int value)
{
        return 0;
}

int hash_search(char *key, int *res)
{
        return 0;
}

int hash_delete(char *key)
{
        return 0;
}


int main(int argc, char **argv)
{
        printf("hash_fnv1a\n");
        for (int i = 1; i < argc; i++) {
                uint8_t *bytes = (uint8_t*) (argv[i]);
                size_t bytes_len = strlen(argv[i]);

                printf("%s: 0x%llX\n", argv[i], hash_fnv1a(bytes, bytes_len));
        }

        return EXIT_SUCCESS;
}