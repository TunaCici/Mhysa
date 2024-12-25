#include <stdlib.h>

/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */

int partition(int **intervals, int p, int r)
{
        int x = intervals[r][0];
        int i = p - 1;

        for (int j = p; j < r; j++) {
                if (intervals[j][0] <= x) {
                i++;

                // swap values only (becuse they are small)
                int tmp = intervals[j][0];
                intervals[j][0] = intervals[i][0];
                intervals[i][0] = tmp;

                tmp = intervals[j][1];
                intervals[j][1] = intervals[i][1];
                intervals[i][1] = tmp;
                }
        }

        i++;

        int tmp = intervals[r][0];
        intervals[r][0] = intervals[i][0];
        intervals[i][0] = tmp;

        tmp = intervals[r][1];
        intervals[r][1] = intervals[i][1];
        intervals[i][1] = tmp;

        return i;
}

void quick_sort(int **intervals, int p, int r)
{
        if (p < r) {
                int q = partition(intervals, p, r);
                quick_sort(intervals, p, q - 1);
                quick_sort(intervals, q + 1, r);
        }
}

int** merge(int** intervals, int intervalsSize, int* intervalsColSize, int* returnSize, int** returnColumnSizes)
{
        // assume all inputs are valid

        // sort the 'intervals' by their 'start' points
        quick_sort(intervals, 0, (intervalsSize - 1));

        // first interval is taken as starting point
        int last_start = intervals[0][0];
        int last_end = intervals[0][1];

        // assume the worst for return array of arrays
        int **merged = (int**) malloc(intervalsSize * sizeof(int*));
        int merged_size = 0;

        for (int i = 1; i < intervalsSize; i++) {
                int start = intervals[i][0];
                int end = intervals[i][1];

                // partial
                if ((start <= last_end) && (last_end <= end)) {
                last_end = end;
                }
                // fully
                else if ((end <= last_end)) {
                continue;
                }
                // none
                else {
                // create a new interval from 'last'
                int *interval = (int*) malloc(2 * sizeof(int));
                interval[0] = last_start;
                interval[1] = last_end;

                merged[merged_size++] = interval;

                // update the 'last' interval
                last_start = start;
                last_end = end;
                }
        }

        // create the remaining interval
        int *interval = (int*) malloc(2 * sizeof(int));
        interval[0] = last_start;
        interval[1] = last_end;

        merged[merged_size++] = interval;

        // prepare the return values
        int *merged_col_sizes = (int*) malloc(merged_size * sizeof(int));
        for (int i = 0; i < merged_size; i++) {
                merged_col_sizes[i] = 2;
        }

        (*returnColumnSizes) = merged_col_sizes;
        (*returnSize) = merged_size;

        return merged;
}