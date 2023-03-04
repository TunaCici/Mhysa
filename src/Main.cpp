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

int main(int argc, char** argv) {
    int nResult = EXIT_FAILURE;

    // Initialize logging
    #ifdef DEBUG
        google::InitGoogleLogging(argv[0]);
        fLB::FLAGS_logtostderr = true;
    #endif

    std::unique_ptr<data_struct::Queue<int>> myIntegerQueue;
    myIntegerQueue = std::make_unique<data_struct::Queue<int>>(64, true);

    for(std::size_t i = 1; i <= 128; i++) {
        bool result = myIntegerQueue->enqueue((int) i);
        DLOG(INFO) << "Container usage: " << myIntegerQueue->usage() << "%" << std::endl;

        if (!result) {
            DLOG(WARNING) << "Oh no... Result: " << result << std::endl;
        }
        else {
            DLOG(INFO) << "Enqueued: " << i << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds (250 ));
    }

    DLOG(INFO) << "Size: " << myIntegerQueue->size() << " Queue: " << *myIntegerQueue << std::endl;

    int temp = 0;
    for(std::size_t i = 1; i <= 128; i++) {
        bool result = myIntegerQueue->dequeue(temp);
        DLOG(INFO) << "Container usage: " << myIntegerQueue->usage() << "%" << std::endl;

        if (!result) {
            DLOG(WARNING) << "Oh no... Result: " << result << std::endl;
        }
        else {
            DLOG(INFO) << "Dequeued: " << i << std::endl;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds (250));
    }

    DLOG(INFO) << "Size: " << myIntegerQueue->size() << " Queue: " << *myIntegerQueue << std::endl;

    nResult = EXIT_SUCCESS;
    return nResult;
}
