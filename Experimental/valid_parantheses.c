#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

bool is_valid(char* s) {
    char *stack_base = NULL;
    char *stack_ptr = NULL;
    char *stack_lim = NULL;

    // Max strlen(s) is 10^4
    stack_base = (char*) calloc(10000, sizeof(char));
    stack_ptr = stack_base;
    stack_lim = (stack_base + (10000 * sizeof(char)));

    for (int i = 0; i < strlen(s); i++) {
        if (s[i] == '(' || s[i] == '[' || s[i] == '{' ) {
            (stack_ptr < stack_lim) && ((*(++stack_ptr)) = s[i]);
        } else if (s[i] == ')') {
            if (*stack_ptr == '(') {
                *stack_ptr = '\0';
                ((stack_base + 1) <= stack_ptr) && (--stack_ptr);
            } else {
                return false;
            }
        } else if (s[i] == ']') {
            if (*stack_ptr == '[') {
                *stack_ptr = '\0';
                ((stack_base + 1) <= stack_ptr) && (--stack_ptr);
            } else {
                return false;
            }
        } else if (s[i] == '}') {
            if (*stack_ptr == '{') {
                *stack_ptr = '\0';
                ((stack_base + 1) <= stack_ptr) && (--stack_ptr);
            } else {
                return false;
            }
        }
    }

    if (stack_ptr == stack_base) {
        return true;
    }

    return false;
}
