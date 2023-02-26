#include <iostream>
#include <vector>
#include <cstdlib>
#include <memory>
#include <chrono>
#include <thread>

#include "glog/logging.h"

#include "Algorithms/Sorting.hpp"
#include "DataStructures/Stack.hpp"

int main(int argc, char** argv) {
    int n_result = EXIT_FAILURE;

    // Initialize logging
    google::InitGoogleLogging(argv[0]);
    #ifdef DEBUG
        fLB::FLAGS_logtostderr = true;
    #endif

    std::unique_ptr<data_struct::Stack<int>> myIntegerStack;
    myIntegerStack = std::make_unique<data_struct::Stack<int>>();

    for(std::size_t i = 1; i <= 16000; i++) {
        bool result = myIntegerStack->push((int) i);

        if (!result) {
            DLOG(FATAL) << "Oh no..." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds (5 ));
    }

    DLOG(INFO) << "Size: " << myIntegerStack->size() << " Stack: " << *myIntegerStack << std::endl;

    int temp = 0;
    for(std::size_t i = 1; i <= 16000; i++) {
        bool result = myIntegerStack->pop(temp);

        if (!result) {
            DLOG(FATAL) << "Oh no..." << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds (5));
    }

    DLOG(INFO) << *myIntegerStack << std::endl;

    n_result = EXIT_SUCCESS;
    return n_result;
}
