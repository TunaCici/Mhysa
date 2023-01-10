#include <iostream>
#include <vector>
#include <random>
#include <string>

#include "Spdlog/spdlog.h"
#include "Algorithms/Sorting.hpp"
#include "Formatting/NumberFormatting.hpp"

int main(int argc, char** argv) {
    int n_result = -1;

    // Set debug level
    spdlog::set_level(spdlog::level::debug);

    const std::string arr1 = "10";
    const std::string arr2 = "11";

    std::string result;
    
    formatting::binary_add(result, arr1, arr2);

    spdlog::info("arr1:\t\t{}", arr1);
    spdlog::info("arr2:\t\t{}", arr2);
    spdlog::info("result:\t{}", result);

    return n_result;
}  