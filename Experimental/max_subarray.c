int maxSubArray(int* nums, int numsSize)
{
        if (!nums || numsSize < 1) {
                return 0;
        }

        int max = nums[0];
        int curr_sum = 0;

        for (int i = 0; i < numsSize; i++) {
                if ((curr_sum + nums[i]) < nums[i]) {
                        curr_sum = 0;
                }

                curr_sum += nums[i];

                if (max < curr_sum) {
                        max = curr_sum;
                }
        }

        return max;
}