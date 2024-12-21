/* think backwards */

#include <stdlib.h>
#include <string.h>

int lengthOfLastWord(char* s) {
        // assuming s is valid

        size_t len = strlen(s);

        // find the last word
        size_t last_word = (len - 1);
        while (s[last_word] == ' ') {
                last_word--;
        }

        // get it's strlen (backwards)
        int iter = last_word;
        while (0 <= iter && s[iter] != ' ') {
                iter--;
        }

        return (last_word - iter);
}