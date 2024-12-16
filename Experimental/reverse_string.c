#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void reverse_string(char *str)
{
    if (!str || strlen(str) == 0) {
        return;
    }

    size_t left = 0;
    size_t right = strlen(str) - 1;

    while (left < right) {
        char tmp = str[right];
        str[right] = str[left];
        str[left] = tmp;

        left++;
        right--;
    }
}

int main()
{
    char *str = "hello world";
    size_t str_len = strlen(str);

    char reversed[32];

    size_t res = strlcpy(reversed, str, (str_len + 1));

    if (res != str_len) {
        fprintf(stderr, "failed to strlcpy\n");
        return EXIT_FAILURE;
    }

    reverse_string(reversed);

    printf("%s\n", str);
    printf("%s\n", reversed);

    return EXIT_SUCCESS;
}