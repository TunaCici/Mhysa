#include <iostream>
#include <vector>
#include <random>

#include "Spdlog/spdlog.h"
#include "Algorithms/Sorting.hpp"

int main(int argc, char** argv) {
    int n_result = -1;

    // Set debug level
    spdlog::set_level(spdlog::level::debug);

    // Generate random ints
    std::random_device                  rand_dev;
    std::mt19937                        generator(rand_dev());
    std::uniform_int_distribution<int> dist(1, 50);

    std::vector<int> my_arr;
    for (auto i = 0; i < 16; i++) {
        my_arr.push_back(dist(generator));
    }

    sorting::insertion_sort(my_arr, false);

    for (auto i = 0; i < my_arr.size(); i++) {
        std::cout << my_arr[i] << ' ';
    }

    std::cout << std::endl;

    return n_result;
} 