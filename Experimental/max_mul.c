#define NO_OF_TERMS 3u

int compare_int(const void *a, const void *b)
{
        return (*(int *)a - *(int *)b);
}


int maximumProduct(int* nums, int numsSize)
{
        if (!nums || numsSize < NO_OF_TERMS) {
                return 0;
        }

        qsort(nums, numsSize, sizeof(int), compare_int);

        int max_product = 1;

        // try all partitions of 'i' smallest and 'NO_OF_TERMS - i' largest
        for (int i = 0; i <= NO_OF_TERMS; i++) {
                if (i > numsSize || (NO_OF_TERMS - i) > numsSize) {
                        continue;
                }

                int product = 1;

                // 'i' smallest numbers
                for (int j = 0; j < i; j++) {
                        product *= nums[j];
                }

                // 'NO_OF_TERMS - i' largest numbers
                for (int j = 0; j < (NO_OF_TERMS - i); j++) {
                        product *= nums[numsSize - 1 - j];
                }

                if (i == 0 || product > max_product) {
                        max_product = product;
                }
        }

        return max_product;
}