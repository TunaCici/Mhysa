/**
 * Note: The returned array must be malloced, assume caller calls free().
 */

#include "uthash.h"

typedef struct my_pair
{
        int key;
        int value;
        UT_hash_handle hh;
} my_pair;

void add_pair(my_pair **table, int key, int value)
{
        my_pair *tmp = NULL;

        tmp = (my_pair*) malloc(sizeof(my_pair));

        tmp->key = key;
        tmp->value = value;

        HASH_ADD_INT(*table, key, tmp);
}

my_pair *find_pair(my_pair **table, int key)
{
        my_pair *result = NULL;

        HASH_FIND_INT(*table, &key, result);

        return result;
}

int* twoSum(int* nums, int numsSize, int target, int* returnSize)
{
        if (!nums || !returnSize) {
                return NULL;
        }

        my_pair *table = NULL;
        int *result = NULL;

        result = (int*) malloc(2 * sizeof(int));
        result[0] = -1;
        result[1] = -1;
        *returnSize = 2;

        for (int i = 0; i < numsSize; i++) {
                int required = target - nums[i];

                my_pair *found = find_pair(&table, required);

                if (found) {
                        result[0] = i;
                        result[1] = found->value;

                        return result;
                }

                add_pair(&table, nums[i], i);
        }

        my_pair *iter;
        my_pair *tmp;

        HASH_ITER(hh, table, iter, tmp) {
                free(iter);
        }

        return result;
}