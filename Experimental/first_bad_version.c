// The API isBadVersion is defined for you.
// bool isBadVersion(int version);

#include <stdbool.h>

extern bool isBadVersion(int version);

static inline unsigned my_avg(int a, int b)
{
    return (a / 2) + (b / 2) + (a & b & 1);
}

int firstBadVersion(int n)
{
        // g g g g g b b b
        // 0 1 2 3 4 5 6 7
        //           ^   

        // just do a binary search

        int left = 0;
        int right = n;
        
        while (left < right) {
                int mid = my_avg(left, right);

                // 1st occurence must be on the left
                if (isBadVersion(mid) == true) {
                        if (isBadVersion(mid - 1) == false) {
                                return mid;
                        }

                        right = mid;
                } else {
                        if (isBadVersion(mid + 1) == true) {
                                return mid + 1;
                        }

                        left = mid;
                }
        }

        return -1;
}
