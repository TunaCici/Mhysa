#include <stdlib.h>

int removeDuplicates(int* nums, int numsSize) {
        if (!nums || numsSize == 0) {
                return 0;
        }

        int k = 0;

        size_t first_occr = 0;
        size_t last_occr = 0;

        for (int i = 1; i < numsSize; i++) {
                if (nums[first_occr] == nums[i]) {
                        last_occr = i;
                } else {
                        // compress
                        nums[k++] = nums[first_occr];

                        // move on to the next unique int
                        first_occr = i;
                }
        }

        // compress the last unique int
        nums[k++] = nums[first_occr];

        return k;
}