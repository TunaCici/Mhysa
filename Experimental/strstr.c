/* classic, but very few assumptions are made */

#include <stdlib.h>
#include <string.h>

int strStr(char* haystack, char* needle) {
    // assuming both haystack and needle is valid!

    size_t haystack_len = strlen(haystack);
    size_t needle_len = strlen(needle);

    for (int i = 0; i < haystack_len; i++) {
        // first character matches
        if (haystack[i] == needle[0]) {

            // deeper search
            int j = 0;
            for (; j < needle_len; j++) {
                if (haystack[i + j] != needle[j]) {
                    break;
                }
            }

            if (j == needle_len) {
                return i;
            }
        }
    }

    return -1;
}