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

static void max_heapify(std::vector<int>& heap_arr, unsigned int idx) {
    unsigned int left_idx = 2*idx + 1; // Left subtree
    unsigned int right_idx = 2*idx + 2; // Right subtree

    unsigned int biggest_idx = idx;

    /* Check for the max-heap property */
    if (left_idx < heap_arr.size() && heap_arr[idx] < heap_arr[left_idx]) {
        biggest_idx = left_idx;
    }
    if (right_idx < heap_arr.size() && heap_arr[biggest_idx] < heap_arr[right_idx]) {
        biggest_idx = right_idx;
    }

    /* Float down the heap_arr[idx] */
    if (biggest_idx != idx) {
        std::swap(heap_arr[idx], heap_arr[biggest_idx]);
        max_heapify(heap_arr, biggest_idx);
    }
}

static void min_heapify(std::vector<int>& heap_arr, unsigned int idx) {
    unsigned int left_idx = 2*idx + 1; // Left subtree
    unsigned int right_idx = 2*idx + 2; // Right subtree

    unsigned int smallest_idx = idx;

    /* Check for the min-heap property */
    if (left_idx < heap_arr.size() && heap_arr[left_idx] < heap_arr[idx]) {
        smallest_idx = left_idx;
    }
    if (right_idx < heap_arr.size() && heap_arr[right_idx] < heap_arr[smallest_idx]) {
        smallest_idx = right_idx;
    }

    /* Float down the heap_arr[idx] */
    if (smallest_idx != idx) {
        std::swap(heap_arr[idx], heap_arr[smallest_idx]);
        min_heapify(heap_arr, smallest_idx);
    }
}

static void build_max_heap(std::vector<int>& target_arr) {
    unsigned int first_leaf_idx = target_arr.size() / 2;

    /* TODO: Try using for_each here */
    for (int i = first_leaf_idx - 1; 0 <= i; i--) {
        max_heapify(target_arr, i);
    }
}

static void build_min_heap(std::vector<int>& target_arr) {
    unsigned int first_leaf_idx = target_arr.size() / 2;

    /* TODO: Try using for_each here */
    for (int i = first_leaf_idx - 1; 0 <= i; i--) {
        min_heapify(target_arr, i);
    }
}

void heap_sort(std::vector<int>& unsorted_arr, bool ascending) {
    if (unsorted_arr.size() <= 1) {
        /* No need to sort */
        return;
    }

    std::vector<int> sorted_array(unsorted_arr.size());

    if (ascending) {
        build_max_heap(unsorted_arr);
    }
    else {
        build_min_heap(unsorted_arr);
    }


    /* TODO: Try using for_each here */
    for (int i = unsorted_arr.size(); 0 < i; i--) {
        sorted_array[i - 1] = unsorted_arr[0];

        /* TODO: Maybe implement a better heap data structure? */
        /* Remove the biggest/smallest element that is at the root */
        std::swap(unsorted_arr[0], unsorted_arr[unsorted_arr.size() - 1]);
        unsorted_arr.pop_back();

        if (ascending) {
            max_heapify(unsorted_arr, 0);
        }
        else {
            min_heapify(unsorted_arr, 0);
        }
    }

    /* Replace the unsorted_arr with sorted_arr */
    unsorted_arr.swap(sorted_array);
}

static int partition(std::vector<int>& arr, int start_idx, int end_idx, bool ascending) {
    int pivot = arr[end_idx]; // TODO: How to choose pivot? Median-of-3 or etc.
    int first_idx = start_idx - 1;

    /* Compare each variable with the pivot */
    /* Put the variable either in the 1st partition(first_idx) or the 2nd partition (second_idx) */
    for (int second_idx = start_idx; second_idx <= end_idx -1; second_idx++) {
        if (ascending && arr[second_idx] < pivot) {
            first_idx++;
            std::swap(arr[first_idx], arr[second_idx]);
        }
        else if (!ascending && pivot < arr[second_idx]){
            first_idx++;
            std::swap(arr[first_idx], arr[second_idx]);
        }
    }

    /* Swap the pivot so that it lies between the two partitions */
    first_idx++;
    std::swap(arr[first_idx], arr[end_idx]);

    return first_idx;
}

static void quick_sort_helper(std::vector<int>& unsorted_arr, int start_idx, int end_idx, bool ascending) {
    if (start_idx < end_idx) {
        int pivot = partition(unsorted_arr, start_idx, end_idx, ascending);

        /* Sort pivot's both left and right sub-array */
        quick_sort_helper(unsorted_arr, start_idx, pivot - 1, ascending);
        quick_sort_helper(unsorted_arr, pivot + 1, end_idx, ascending);
    }
}

void quick_sort(std::vector<int>& unsorted_arr, bool ascending) {
    if (unsorted_arr.size() <= 1) {
        /* No need to sort */
        return;
    }

    quick_sort_helper(unsorted_arr, 0, unsorted_arr.size() - 1, ascending);
}


}
