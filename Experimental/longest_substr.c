#include <stdlib.h>

void insert_hash(int *table, char key, int value)
{
        table[(int) key] = value;
}

int find_hash(int *table, char key)
{
        return table[(int) key];
}

int lengthOfLongestSubstring(char* s)
{
        // hash table for ASCII-256 characters
        // key: ascii character
        // value: int
        int *table = NULL;
        int table_capacity = 256;

        table = (int*) malloc(table_capacity * sizeof(int));
        for (int i = 0; i < table_capacity; i++) {
                table[i] = -1;
        }

        int start = 0;
        int end = 0;
        int longest = 0;

        for (; s[end] != '\0'; end++) {
                int res = find_hash(table, s[end]);

                if ((res != -1) && !(res < start)) {
                        start = res + 1;
                }
                insert_hash(table, s[end], end);

                int len = (end - start + 1);
                if (longest < len) {
                        longest = len;
                }
        }

        return longest;
}