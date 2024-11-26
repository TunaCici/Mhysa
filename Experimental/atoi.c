#include <limits.h>

int calc_w_overflow(int n, char digit, char sign)
{
    int res = 0;

    if (__builtin_mul_overflow(n, 10, &res)) {
        if (sign == '-') {
            return INT_MIN;
        }

        return INT_MAX;
    }

    n = res;
    
    if (sign == '-') {
        if (__builtin_sub_overflow(n, (digit - '0'), &res)) {
            return INT_MIN;
        }
    } else {
        if (__builtin_add_overflow(n, (digit - '0'), &res)) {
            return INT_MAX;
        }
    }

    n = res;

    return n;
}

int my_atoi(char* s) {
    int n = 0;

    unsigned int curr = 0;
    char sign = '+';

    if (!s) {
        return 0;
    }

    /* Skip any leading ws */
    while(s[curr] == ' ') {
        curr++;
    }

    if (s[curr] == '-' || s[curr] == '+') {
        sign = s[curr];
        curr++;
    }

    /* Skip any leading zeros (0) */
    while(s[curr] == '0') {
        curr++;
    }

    while (s[curr] != '\0') {
        if ('0' <= s[curr] && s[curr] <= '9') {
            n = calc_w_overflow(n, s[curr], sign);
        } else {
            break;
        }

        curr++;
    }

    return n;
}
