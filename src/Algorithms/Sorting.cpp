#include "Algorithms/Sorting.hpp"

namespace sorting {

bool in_bound(const int& target, const int& left, const int& right) {
    return left < target && target < right;
}

bool is_ordered(std::vector<int>& arr, bool ascending) {
    bool ret_value = false;

    if (arr.size() <= 1) {
        ret_value = true;
    }
    else {
        unsigned int idx;
        for (idx = 1; idx < arr.size(); idx++) {
            /* Values should increase */
            if (ascending && arr[idx] < arr[idx - 1]) {
                break;
            }
            /* Values should decrease */
            else if (!ascending && arr[idx - 1] < arr[idx]) {
                break;
            }
        }

        /* Traversed all the elements? */
        if (idx == arr.size()) {
            ret_value = true;
        }
    }

    return ret_value;
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
