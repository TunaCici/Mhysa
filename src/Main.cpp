#include <iostream>
#include <vector>
#include <cstdlib>

#include "glog/logging.h"

#include "Formatting/NumberFormatting.hpp"

int main(int argc, char** argv) {
    int n_result = EXIT_FAILURE;

    // Initialize logging
    google::InitGoogleLogging(argv[0]);
    #ifdef DEBUG
        fLB::FLAGS_logtostderr = true;
    #endif

    const std::string arr1 = "1011";
    const std::string arr2 = "1100";

    std::string result;
    
    formatting::binary_add(result, arr1, arr2);

    DLOG(INFO) << "arr1:\t" << arr1;
    DLOG(INFO) << "arr2:\t" << arr2;
    DLOG(INFO) << "result:\t" << result;

    n_result = EXIT_SUCCESS;
    return n_result;
}
