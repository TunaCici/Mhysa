#include <stdlib.h>
#include <string.h>

int expand(char *s, size_t s_len, int mid_left, int mid_right)
{
    int radius = 0;

    while (0 <= mid_left && mid_right < s_len) {
        if (s[mid_left] != s[mid_right]) {
            return radius;
        }

        mid_left--;
        mid_right++;

        radius++;
    }

    return radius;
}

char* longestPalindrome(char* s)
{
        if (!s) {
                return NULL;
        }

        // [x] option 1. just expand from each center
        // [ ] option 2. manacher's algorithm

        size_t s_len = strlen(s);

        char *longest = NULL;
        size_t longest_len = 0;

        for (int i = 0; i < s_len; i++) {
                // odd-length
                int mid_left = i;
                int mid_right = i;

                int radius = expand(s, s_len, mid_left, mid_right);
                int curr_len = 2 * radius - 1;
                if (longest_len < curr_len) {
                        longest = &s[i - (radius - 1)];
                        longest_len = curr_len;
                }

                // even-length
                mid_left = i - 1;
                mid_right = i;

                radius = expand(s, s_len, mid_left, mid_right);
                curr_len = 2 * radius;
                if (longest_len < curr_len) {
                        longest = &s[i - (radius)];
                        longest_len = curr_len;
                }
        }

        // create space for the result
        char *result = (char*) malloc((longest_len + 1) * sizeof(char));
        strncpy(result, longest, longest_len);
        result[longest_len] = '\0';

        return result;
}
