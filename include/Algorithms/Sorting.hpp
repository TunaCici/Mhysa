#ifndef SORTING_HPP
#define SORTING_HPP

#include <vector>

namespace sorting {

/* Utilities */
bool in_bound(const int& target, const int& left, const int& right);
bool is_ordered(std::vector<int>& arr, bool ascending = true);

/* Sorting functions */
void insertion_sort(std::vector<int>& unsorted_arr, bool ascending = true);

}

#endif