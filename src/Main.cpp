#include <iostream>
#include <vector>
#include <cstdlib>

#include "glog/logging.h"

#include "Algorithms/Sorting.hpp"

int main(int argc, char** argv) {
    int n_result = EXIT_FAILURE;

    // Initialize logging
    google::InitGoogleLogging(argv[0]);
    #ifdef DEBUG
        fLB::FLAGS_logtostderr = true;
    #endif

    std::vector<int> my_arr = {0, 45, 234, 23, 34, 5, -1, -4, 6, -55, -4};

    sorting::quick_sort(my_arr, false);
    for (int i : my_arr) {
        DLOG(INFO) << i;
    }

    DLOG(INFO) << "is_ordered: " << sorting::is_ordered(my_arr);

    n_result = EXIT_SUCCESS;
    return n_result;
}
