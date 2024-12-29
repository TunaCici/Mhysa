#include <stdlib.h>
#include <string.h>

static inline int my_max(int a, int b)
{
        return (a < b) ? b : a;
}

char* addBinary(char* a, char* b)
{
        if (!a || !b) {
                return NULL;
        }


        int a_len = strlen(a);
        int b_len = strlen(b);

        // no input has leading zeroes
        // but: edge case: '0' + '0'
        if (a[0] == '0' && b[0] == '0') {
                return "0";
        }

        // result size can't be greate than my_max(a_len, b_len) + 1
        // i also allocate space for the null-terminator
        char *result = NULL;
        size_t result_len = my_max(a_len, b_len) + 1 + 1;

        result = (char*) malloc((result_len) * sizeof(char));
        for (int i = 0; i < result_len; i++) {
                result[i] = '0';
        }

        int carry = 0;
        for (int i = 0; i < result_len; i++) {
                int digit_a = 0;
                int digit_b = 0;

                if (0 <= (a_len - 1 - i)) {
                        digit_a = a[a_len - 1 - i] - '0';
                }

                if (0 <= (b_len - 1 - i)) {
                        digit_b = b[b_len - 1 - i] - '0';
                }

                int res = digit_a + digit_b + carry;

                result[i] = (char) ((res & 0b1) + '0'); // the first bit is stored
                carry = (res >> 1); // the second bit is carried
        }

        // skip the leading zeroes in result
        int start = result_len - 1;
        while (0 <= start && result[start] == '0') {
                start--;
        }

        // reverse the result
        int left = 0;
        int right = start;
        while (left < right) {
                char tmp = result[right];
                result[right] = result[left];
                result[left] = tmp;

                left++;
                right--;
        }

        // put the null-terminator
        result[start + 1] = '\0';

        return result;
}