#include "uthash.h"

typedef struct my_pair
{
        int key;
        int value;
        UT_hash_handle hh;
} my_pair;

void insert_prefix(my_pair **table, int key, int value)
{
        my_pair *pair = NULL;

        pair = (my_pair*) malloc(sizeof(my_pair));
        pair->key = key;
        pair->value = value;

        HASH_ADD_INT(*table, key, pair);
}

my_pair *find_prefix(my_pair **table, int key)
{
        my_pair *found = NULL;

        HASH_FIND_INT(*table, &key, found);

        return found;
}

int subarraySum(int* nums, int numsSize, int k)
{
        if (!nums || numsSize == 0) {
                return 0;
        }
        
        int total = 0;

        my_pair *table = NULL;

        // empty subarray
        insert_prefix(&table, 0, 1);

        int prev = 0;
        for (int i = 0; i < numsSize; i++) {
                int prefix_sum = nums[i] + prev;
                prev = prefix_sum;

                // need 'req' amount of sum
                int req = prefix_sum - k;

                my_pair *found = find_prefix(&table, req);
                if (found) {
                        total += found->value;
                }
                
                found = find_prefix(&table, prefix_sum);
                if (found) {
                        found->value++;
                } else {
                        insert_prefix(&table, prefix_sum, 1);
                }   
        }

        return total;
}