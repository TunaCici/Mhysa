#include <iostream>
#include <vector>
#include <cstdlib>
#include <memory>
#include <chrono>
#include <thread>

#include "glog/logging.h"

#include "Algorithms/Sorting.hpp"
#include "DataStructures/Stack.hpp"
#include "DataStructures/Queue.hpp"
#include "DataStructures/LinkedList.hpp"

int main(int argc, char** argv) {
    int nResult = EXIT_FAILURE;

    // Initialize logging
    #ifdef DEBUG
        google::InitGoogleLogging(argv[0]);
        fLB::FLAGS_logtostderr = true;
    #endif

    std::unique_ptr<data_struct::LinkedList<int>> myLinkedList;
    myLinkedList = std::make_unique<data_struct::LinkedList<int>>(data_struct::double_link);

    for(std::size_t i = 8; 0 < i; i--) {
        bool result = myLinkedList->enqueue(i * 3);

        if (result) {
            DLOG(INFO) << *myLinkedList << std::endl;
        }
        else {
            DLOG(WARNING) << "Oh no... Result: " << result << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds (250 ));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds (1000 ));

    bool result = myLinkedList->sort();
    if (result) {
        DLOG(INFO) << *myLinkedList << std::endl;
    }
    else {
        DLOG(WARNING) << "Oh no... Result: " << result << std::endl;
    }

    result = myLinkedList->reverse();
    if (result) {
        DLOG(INFO) << *myLinkedList << std::endl;
    }
    else {
        DLOG(WARNING) << "Oh no... Result: " << result << std::endl;
    }

    nResult = EXIT_SUCCESS;
    return nResult;
}
