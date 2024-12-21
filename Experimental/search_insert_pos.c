/* classic binary search with a trich */

#include <stdlib.h>

int searchInsert(int* nums, int numsSize, int target) {
        // assuming all params are valid!

        size_t left = 0;
        size_t right = numsSize;
        size_t pivot = 0;

        while (left < right) {
                // perform a binary search
                pivot = (left + right) / 2;

                // if 'pivot' hasn't changed, then 'target' doesn't exist
                if (pivot == left || pivot == right) {
                        break;
                }

                if (nums[pivot] == target) {
                        return pivot;
                } else if (nums[pivot] < target) {
                        left = pivot;
                } else {
                        right = pivot;
                }
        }

        // return where 'target' would be if it were to be inserted
        size_t place = 0;
        if (nums[pivot] < target) {
                place = right;
        } else {
                place = left;
        }

        return place;
}