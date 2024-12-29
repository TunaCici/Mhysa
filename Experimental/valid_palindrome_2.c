#include <string.h>
#include <stdbool.h>

bool shrink(char *s, size_t s_len, int *left, int *right)
{
        while (*left < *right) {
                if (s[*left] != s[*right]) {
                return false;
                }

                (*left)++;
                (*right)--;
        }

        return true;
}

bool validPalindrome(char* s)
{
        size_t s_len = strlen(s);

        int left = 0;
        int right = (s_len - 1);

        // try if palindrome
        bool res = shrink(s, s_len, &left, &right);
        if (res) {
                return true;
        }

        // skip from left
        int test_left = left + 1;
        int test_right = right;
        res = shrink(s, s_len, &test_left, &test_right);
        if (res) {
                return true;
        }

        // skip from right
        test_left = left;
        test_right = right - 1;
        res = shrink(s, s_len, &test_left, &test_right);
        if (res) {
                return true;
        }

        return false;
}