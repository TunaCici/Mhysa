#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int lexicographic_sort(const char* a, const char* b) {
    if (strcmp(a, b) < 0) {
        return 1;
    } else {
        return 0;
    }
}

int lexicographic_sort_reverse(const char* a, const char* b) {
    if (0 < strcmp(a, b)) {
        return 1;
    } else {
        return 0;
    }
}

int sort_by_number_of_distinct_characters(const char* a, const char* b) {
    // assuming only lower-case english letters
    int letter_count = 'z' - 'a' + 1; 
    int *letters = (int*) calloc(letter_count, sizeof(int));

    int a_unique_count = 0;
    int b_unique_count = 0;

    for (int i = 0; a[i] != '\0'; i++) {
        int idx = a[i] - 'a'; 
        letters[idx]++;
    }

    for (int i = 0; i < letter_count; i++) {
        if (letters[i]) {
            a_unique_count++;
        }
    }

    // reset
    memset(letters, 0, letter_count * sizeof(letters[0]));

    for (int i = 0; b[i] != '\0'; i++) {
        int idx = b[i] - 'a'; 
        letters[idx]++;
    }

    for (int i = 0; i < letter_count; i++) {
        if (letters[i]) {
            b_unique_count++;
        }
    }

    if (a_unique_count < b_unique_count) {
        return 1;
    } else if (b_unique_count < a_unique_count) {
        return 0;
    }

    // if both length is equal than use lexical sort
    return lexicographic_sort(a, b);
}

int sort_by_length(const char* a, const char* b) {
    int a_len = strlen(a);
    int b_len = strlen(b);

    if (a_len < b_len) {
        return 1;
    } else if (b_len < a_len) {
        return 0;
    }

    // if both length is equal than use lexical sort
    return lexicographic_sort(a, b);
}

void string_sort(char** arr,const int len,int (*cmp_func)(const char* a, const char* b)){
    if (!arr || len == 0 || !cmp_func) {
        return;
    }

    // Since len <= 50 I can use a simple bubble-sort algorithm
    for (int i = 0; i < len; i++) {
        for (int j = i + 1; j < len; j++) {

            // if cmp_func is non-zero, it means 'swap' is needed
            if (cmp_func(arr[j], arr[i])) {
                char *tmp = arr[j];
                arr[j] = arr[i];
                arr[i] = tmp;
            }
        }
    }
}


int main() 
{
    int n;
    scanf("%d", &n);
  
    char** arr;
	arr = (char**)malloc(n * sizeof(char*));
  
    for(int i = 0; i < n; i++){
        *(arr + i) = malloc(1024 * sizeof(char));
        scanf("%s", *(arr + i));
        *(arr + i) = realloc(*(arr + i), strlen(*(arr + i)) + 1);
    }
  
    string_sort(arr, n, lexicographic_sort);
    for(int i = 0; i < n; i++)
        printf("%s\n", arr[i]);
    printf("\n");

    string_sort(arr, n, lexicographic_sort_reverse);
    for(int i = 0; i < n; i++)
        printf("%s\n", arr[i]); 
    printf("\n");

    string_sort(arr, n, sort_by_length);
    for(int i = 0; i < n; i++)
        printf("%s\n", arr[i]);    
    printf("\n");

    string_sort(arr, n, sort_by_number_of_distinct_characters);
    for(int i = 0; i < n; i++)
        printf("%s\n", arr[i]); 
    printf("\n");
}