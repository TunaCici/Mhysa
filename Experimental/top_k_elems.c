/**
 * Note: The returned array must be malloced, assume caller calls free().
 */

#include <stdlib.h>

typedef struct my_pair
{
    int value;
    int occur;
} my_pair;

int compare_int(const void *a, const void *b)
{
        return (*(int*) a) - (*(int*) b);
}

int compare_pair(const void *a, const void *b)
{
        return (*((my_pair**) b))->occur - (*((my_pair**) a))->occur;
}

int* topKFrequent(int* nums, int numsSize, int k, int* returnSize)
        {
        if (!nums || numsSize < 0) {
                *returnSize = 0;
                return NULL;
        }

        // option 1. hash with autosort feature (e.g., std::ordered_map)
        // option 1.1. crete the return array of K elements
        // option 1.2. initialize it with hash iterator (up to K)
        // option 1.3. return that array
        // option 2. sort the input
        // option 2.1. count all unique elements
        // option 2.2. create an array of 'pairs'
        // option 2.3. each pair is value and it's occurance
        // option 2.4. sort the newly created array using the occurance
        // option 2.5. create the return array of K elements
        // option 2.6. initialize it with the sorted array of 'pairs' (up to K)
        // option 2.7. return that array

        // go with option 2
        qsort(nums, numsSize, sizeof(int), compare_int);

        size_t unique_count = 1;
        int prev = 0;
        for (int i = 1; i < numsSize; i++) {
                if (nums[prev] != nums[i]) {
                        unique_count++;
                }
                prev = i;
        }

        // array of value-occur pairs
        my_pair **occur_arr = (my_pair**) malloc(unique_count * sizeof(my_pair*));
        int occur_iter = 0;

        int occur = 1;
        prev = 0;
        for (int i = 1; i < numsSize; i++) {
                if (nums[prev] != nums[i]) {
                        my_pair *pair = (my_pair*) malloc(sizeof(my_pair));
                        pair->value = nums[prev];
                        pair->occur = occur;
                        occur_arr[occur_iter++] = pair;

                        occur = 1;
                }

                occur++;
                prev = i;
        }

        // handle the last element
        if (occur_iter < unique_count) {
                my_pair *pair = (my_pair*) malloc(sizeof(my_pair));
                pair->value = nums[prev];
                pair->occur = occur;
                occur_arr[occur_iter++] = pair;
        }

        // sort the occurance array
        qsort(occur_arr, unique_count, sizeof(my_pair*), compare_pair);

        // return array
        int *top_k_arr = (int*) malloc(k * sizeof(int));
        for (int i = 0; i < k; i++) {
                top_k_arr[i] = occur_arr[i]->value;
        }
        *returnSize = k;

        // free the pair array
        for (int i = 0; i < unique_count; i++) {
                free(occur_arr[i]);
        }
        free(occur_arr);

        return top_k_arr;
}