#include <stdlib.h>

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* plusOne(int* digits, int digitsSize, int* returnSize)
{
        // assume all inputs are valid!

        int *result_arr = NULL;
        size_t result_arr_size = digitsSize;

        int carry = 0;
        int inter = 0;

        int is_carry = 0;

        // add 1 to the first least significant
        inter = digits[digitsSize - 1] + 1;
        digits[digitsSize - 1] = (inter % 10);
        inter /= 10;
        carry += inter;

        // start adding from the second least significant
        for (int i = (digitsSize - 2); 0 <= i; i--) {
                inter = digits[i] + (carry % 10);
                carry /= 10;

                digits[i] = (inter % 10);
                inter /= 10;
                carry += inter;
        }

        // reserve space for carry
        if (carry) {
                result_arr_size += 1;
                is_carry = 1;
        }

        result_arr = (int*) calloc(result_arr_size, sizeof(int));

        // add the last carry
        if (carry) {
                result_arr[0] = carry;
        }

        // copy the remaining digits
        for (int i = 0; i < digitsSize; i++) {
                result_arr[i + is_carry] = digits[i];
        }

        *returnSize = result_arr_size;

        return result_arr;
}
