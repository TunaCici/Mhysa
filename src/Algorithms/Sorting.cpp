#include "Algorithms/Sorting.hpp"

namespace sorting {

bool in_bound(const int& target, const int& left, const int& right) {
    return left < target && target < right;
}

void insertion_sort(std::vector<int>& unsorted_arr, bool ascending) {
    if (unsorted_arr.size() <= 1) {
        /* No need to sort */
    }
    else {
        /* Control values */
        int start_idx = (ascending == true ? 1 : unsorted_arr.size() - 1);
        int move_by_one = (ascending == true ? 1 : - 1);
        int arr_size = unsorted_arr.size();

        /*
        * Depending on the sorting order start either from left or right
        * Depending on the sorting order move_by_one to either right or left
        */
        for (int i = start_idx; in_bound(i, -1, arr_size); i+=move_by_one) {
            int on_hand = unsorted_arr[i];

            int j = i - move_by_one;
            while (in_bound(j, -1, arr_size) && on_hand < unsorted_arr[j]) {
                unsorted_arr[j + move_by_one] = unsorted_arr[j];
                j -= move_by_one;
            }
            unsorted_arr[j + move_by_one] = on_hand;
        }        
    }
}



}
