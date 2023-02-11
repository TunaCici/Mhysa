#include "Algorithms/Sorting.hpp"

namespace sorting {

bool in_bound(const int& target, const int& left, const int& right) {
    return left < target && target < right;
}

bool is_ordered(const std::vector<int>& arr, const bool ascending) {
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
        return;
    }

    /* Control values */
    int start_idx = (ascending == true ? 1 : unsorted_arr.size() - 1);
    int move_by_one = (ascending == true ? 1 : - 1);
    const int arr_size = unsorted_arr.size();

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

void bubble_sort(std::vector<int>& unsorted_arr, bool ascending) {
    if (unsorted_arr.size() <= 1) {
        /* No need to sort */
        return;
    }

    for (int i = unsorted_arr.size() - 1; 0 <= i; i--) {
        for (int j = 1; j <= i; j++) {
            bool left_bigger = unsorted_arr[j] < unsorted_arr[j - 1];
            bool left_smaller = unsorted_arr[j - 1] < unsorted_arr[j];

            if(ascending && left_bigger) {
                std::swap(unsorted_arr[j], unsorted_arr[j - 1]);
            }
            else if (!ascending && left_smaller){
                std::swap(unsorted_arr[j], unsorted_arr[j - 1]);
            }
        }
    }
}

static void merge_helper(
        std::vector<int>& left_arr,
        std::vector<int>& right_arr,
        std::vector<int>& unsorted_arr,
        bool ascending) {
    unsigned int arr_idx = 0;
    unsigned int left_idx = 0;
    unsigned int right_idx = 0;

    unsigned int left_size = left_arr.size();
    unsigned int right_size = right_arr.size();

    /* Merge two arrays into one while also sorting them */
    while(left_idx < left_size && right_idx < right_size) {
        bool left_bigger = right_arr[right_idx] < left_arr[left_idx];
        bool left_smaller = left_arr[left_idx] < right_arr[right_idx];

        if (ascending && left_smaller) {
            unsorted_arr[arr_idx] = left_arr[left_idx];
            left_idx++;
        }
        else if (!ascending && left_bigger) {
            unsorted_arr[arr_idx] = left_arr[left_idx];
            left_idx++;
        }
        else {
            unsorted_arr[arr_idx] = right_arr[right_idx];
            right_idx++;
        }
        arr_idx++;
    }

    /* Lastly, add the leftover elements (if any) */
    while(left_idx < left_size) {
        unsorted_arr[arr_idx] = left_arr[left_idx];
        left_idx++;
        arr_idx++;
    }
    while(right_idx < right_size) {
        unsorted_arr[arr_idx] = right_arr[right_idx];
        right_idx++;
        arr_idx++;
    }
}

void merge_sort(std::vector<int>& unsorted_arr, bool ascending) {
    if (unsorted_arr.size() <= 1) {
        /* No need to sort */
        return;
    }

    /* Divide the array into 2 sub arrays */
    unsigned int middle_idx = unsorted_arr.size() / 2;
    std::vector<int> left_arr = {unsorted_arr.begin(), unsorted_arr.begin() + middle_idx};
    std::vector<int> right_arr = {unsorted_arr.begin() + middle_idx, unsorted_arr.end()};

    merge_sort(left_arr, ascending);
    merge_sort(right_arr, ascending);
    merge_helper(left_arr, right_arr, unsorted_arr, ascending);
}

void quick_sort(std::vector<int>& unsorted_arr, bool ascending) {
    if (unsorted_arr.size() <= 1) {
        /* No need to sort */
        return;
    }


}


}
