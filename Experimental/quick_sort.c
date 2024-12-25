#include <stdlib.h>
#include <stdio.h>

int partition(int *numbers, int p, int r)
{
        int x = numbers[r];
        int i = p - 1;

        for (int j = p; j < r; j++) {
                if (numbers[j] <= x) {
                        i++;
                        int tmp = numbers[j];
                        numbers[j] = numbers[i];
                        numbers[i] = tmp;
                }
        }

        i++;
        int tmp = numbers[r];
        numbers[r] = numbers[i];
        numbers[i] = tmp;

        return i;
}

// p: start index (inclusive)
// r: end index (inclusive)
void quick_sort(int *numbers, int p, int r)
{
        if (p < r) {
                int q = partition(numbers, p, r);
                quick_sort(numbers, p, q - 1);
                quick_sort(numbers, q + 1, r);
        }
}

int main()
{
        int numbers[] = {2, 8, 7, 1,3, 5, 6, 4};
        int numbers_len = 8;

        printf("before:\n");
        for (int i = 0; i < numbers_len; i++) {
                printf("%d ", numbers[i]);
        }
        printf("\n");

        quick_sort(numbers, 0, (numbers_len - 1));

        printf("after:\n");
        for (int i = 0; i < numbers_len; i++) {
                printf("%d ", numbers[i]);
        }
        printf("\n");

        return EXIT_SUCCESS;
}