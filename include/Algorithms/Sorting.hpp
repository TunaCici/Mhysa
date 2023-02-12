#ifndef SORTING_HPP
#define SORTING_HPP

#include <vector>

namespace sorting {

/* Utilities */
bool in_bound(const int& target, const int& left, const int& right);
bool is_ordered(const std::vector<int>& arr, bool ascending = true);

/* Sorting functions */
void bubble_sort(std::vector<int>& unsorted_arr, bool ascending = true);
void insertion_sort(std::vector<int>& unsorted_arr, bool ascending = true);
void merge_sort(std::vector<int>& unsorted_arr, bool ascending = true);
void heap_sort(std::vector<int>& unsorted_arr, bool ascending = true);
void quick_sort(std::vector<int>& unsorted_arr, bool ascending = true);

}

#endif