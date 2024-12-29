void moveZeroes(int* nums, int numsSize)
{
        // assume all inputs are valid

        int non_zeroes_len = 0;
        int zeroes_len = 0;

        for (int i = 0; i < numsSize; i++) {
                if (nums[i] != 0) {
                nums[non_zeroes_len++] = nums[i];
                }
        }

        // fill in the rest with zeroes
        for (int i = non_zeroes_len; i < numsSize; i++) {
                nums[i] = 0;
        }
}