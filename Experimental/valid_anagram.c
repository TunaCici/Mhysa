#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static inline void ascii_hash_insert(int *table, char key, int value)
{
        table[(size_t) key] = value;
}

static inline int ascii_hash_find(int *table, char key)
{
        return table[(size_t) key];
}

bool isAnagram(char* s, char* t)
{
        // optimization: i can remove the second hash table

        size_t s_len = strlen(s);
        size_t t_len = strlen(t);

        if (s_len != t_len) {
            return false;
        }

        size_t ascii_hash_len = 256;
        int *s_table = (int*) calloc(ascii_hash_len, sizeof(int));
        int *t_table = (int*) calloc(ascii_hash_len, sizeof(int));

        // count all character occurances in s
        for (int i = 0; i < s_len; i++) {
                int val = ascii_hash_find(s_table, s[i]);
                ascii_hash_insert(s_table, s[i], val + 1);
        }
        
        // count all character occurances in t
        for (int i = 0; i < t_len; i++) {
                int val = ascii_hash_find(t_table, t[i]);
                ascii_hash_insert(t_table, t[i], val + 1);
        }
        
        // compare from s to t
        for (int i = 0; i < s_len; i++) {
                int s_occur = ascii_hash_find(s_table, s[i]);
                int t_occur = ascii_hash_find(t_table, s[i]);

                if (s_occur != t_occur) {
                        return false;
                }
        }

        return true;
}