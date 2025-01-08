#include <stdlib.h>

int hammingWeight(int n)
{
        size_t one_bits = 0;

        while (n != 0) {
                n = n & (n - 1); // remove the lsb bit that is '1'
                one_bits++;
        }

        return one_bits;
}