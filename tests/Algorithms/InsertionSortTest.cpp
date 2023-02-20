//
// Created by Tuna Cici on 20.01.2023.
//

#include <random>
#include <vector>
#include <ctime>
#include <limits>

#include "gtest/gtest.h"

#include "Algorithms/Sorting.hpp"


/* Random int generator mt19937 */
static std::mt19937 mt_machine(time(nullptr));
static std::uniform_int_distribution uni_dist(
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max()
);

TEST(Insertion_Sort, Edge_Cases) {
    /*Vectors with size 0 */
    const std::vector<int> org_vec_no_size;
    std::vector<int> srtd_vec_no_size;

    /* Vectors with size 1 */
    const std::vector<int> org_vec_one_size = {42};
    std::vector<int> srtd_vec_one_size = {42};

    /* Vectors should not change */
    sorting::insertion_sort(srtd_vec_no_size);
    EXPECT_EQ(org_vec_no_size == srtd_vec_no_size, true);

    sorting::insertion_sort(srtd_vec_one_size);
    EXPECT_EQ(org_vec_one_size, srtd_vec_one_size);
}

TEST(Insertion_Sort, Sorting_Size_1K) {
    std::vector<int> vec_thousand = {};
    for (long i = 0; i < 1000; i++) { vec_thousand.push_back(uni_dist(mt_machine));}

    /* Ascending order */
    sorting::insertion_sort(vec_thousand);
    EXPECT_EQ(sorting::is_ordered(vec_thousand), true);

    /* Descending order */
    sorting::insertion_sort(vec_thousand, false);
    EXPECT_EQ(sorting::is_ordered(vec_thousand, false), true);
}
TEST(Insertion_Sort, Sorting_Size_10K) {
    std::vector<int> vec_ten_thousand = {};
    for (long i = 0; i < 10000; i++) { vec_ten_thousand.push_back(uni_dist(mt_machine));}

    /* Ascending order*/
    sorting::insertion_sort(vec_ten_thousand);
    EXPECT_EQ(sorting::is_ordered(vec_ten_thousand), true);

    /* Descending order*/
    sorting::insertion_sort(vec_ten_thousand, false);
    EXPECT_EQ(sorting::is_ordered(vec_ten_thousand, false), true);
}
TEST(Insertion_Sort, Sorting_Size_50K) {
    std::vector<int> vec_fifty_thousand = {};
    for (long i = 0; i < 50000; i++) { vec_fifty_thousand.push_back(uni_dist(mt_machine));}

    /* Ascending order*/
    sorting::insertion_sort(vec_fifty_thousand);
    EXPECT_EQ(sorting::is_ordered(vec_fifty_thousand), true);

    /* Descending order*/
    sorting::insertion_sort(vec_fifty_thousand, false);
    EXPECT_EQ(sorting::is_ordered(vec_fifty_thousand, false), true);
}