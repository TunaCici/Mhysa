#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();

/*
 * Complete the 'encryption' function below.
 *
 * The function is expected to return a STRING.
 * The function accepts STRING s as parameter.
 */

/*
 * To return the string from the function, you should either do static allocation or dynamic allocation
 *
 * For example,
 * char* return_string_using_static_allocation() {
 *     static char s[] = "static allocation of string";
 *
 *     return s;
 * }
 *
 * char* return_string_using_dynamic_allocation() {
 *     char* s = malloc(100 * sizeof(char));
 *
 *     s = "dynamic allocation of string";
 *
 *     return s;
 * }
 *
 */
char* encryption(char* s) {
    if (!s) {
        return "";
    }
    
    // length of the str w/out spaces
    int non_ws_len = 0;
    for (int i = 0; s[i] != '\0'; i++) {
        if ('a' <= s[i] && s[i] <= 'z') {
            non_ws_len++;
        }
    }
    
    // calculate grid row X col
    double sqrt_l = sqrt((double) non_ws_len);
    int sqrt_l_floor = (int) floor(sqrt_l);
    int sqrt_l_ceil = (int) ceil(sqrt_l);
    
    int row = sqrt_l_floor;
    int col = sqrt_l_ceil;
    
    if (row * col < non_ws_len) {
        if (row < col) {
            row = col;
        } else {
            col = row;
        }
    }
    
    // alloc space for encrypted string (1D) & grid (2D)
    char *encrypted_s = (char*) malloc(((row * col) + col) * sizeof(char));
    char **grid = (char**) malloc(row * sizeof(char*));
    for (int i = 0; i < row; i++) {
        grid[i] = (char*) malloc(col * sizeof(char));
    }
    
    int iter = 0;
    
    // generate the grid
    for (int i = 0; i < row; i++) {
        for (int j = 0; j < col; j++) {
            // skip ws chars
            while (iter < strlen(s) && !('a' <= s[iter] && s[iter] <= 'z')) {
                iter ++;
            }
            
            if (iter < strlen(s)) {
                grid[i][j] = s[iter++];
            } else {
                grid[i][j] = '\0';
            }
        }
    }
    
    iter = 0;
    
    // convert grid to encrypted string (1D)
    for (int i = 0; i < col; i++) {
        for (int j = 0; j < row; j++) {
            if (!(grid[j][i] == '\0')) {
                encrypted_s[iter++] = grid[j][i];
            }
        }
        
        encrypted_s[iter++] = ' ';
    }
    encrypted_s[iter] = '\0';
    
    return encrypted_s;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char* s = readline();

    char* result = encryption(s);

    fprintf(fptr, "%s\n", result);

    fclose(fptr);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!data) {
            data = '\0';

            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);

        if (!data) {
            data = '\0';
        }
    } else {
        data = realloc(data, data_length + 1);

        if (!data) {
            data = '\0';
        } else {
            data[data_length] = '\0';
        }
    }

    return data;
}
