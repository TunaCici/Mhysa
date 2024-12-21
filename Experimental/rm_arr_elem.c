/* not a bad solution, but it's  O(n2) :( */

int removeElement(int* nums, int numsSize, int val) {
        if (!nums || numsSize == 0) {
                return 0;
        }

        for (int i = 0; i < numsSize; i++) {
                if (nums[i] == val) {
                        // find the first occr of not 'val'
                        int j = i;
                        while (j < numsSize && nums[j] == val) {
                                j++;
                        }

                        // rest of the values are not 'val'
                        if (j == numsSize) {
                                return i;
                        }

                        // swap 'val' with the next not 'val'
                        int tmp = nums[j];
                        nums[j] = nums[i];
                        nums[i] = tmp;
                }
        }

        return numsSize;
}