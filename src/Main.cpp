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
#include "DataStructures/BinaryTree.hpp"

int main(int argc, char** argv) {
    int nResult = EXIT_FAILURE;

    // Initialize logging
    #ifdef DEBUG
        google::InitGoogleLogging(argv[0]);
        fLB::FLAGS_logtostderr = true;
    #endif

    std::unique_ptr<data_struct::BinaryTree<int>> myBinaryTree;
    myBinaryTree = std::make_unique<data_struct::BinaryTree<int>>();

    std::vector<int> someValues = {8, 4, 12, 2, 6, 10, 14, 1, 3, 5, 7, 9, 15};

    for(std::size_t i = 0; i < someValues.size(); i++) {
        bool result = myBinaryTree->insert(someValues[i]);

        if (result) {
            DLOG(INFO) << *myBinaryTree << std::endl;
        }
        else {
            DLOG(WARNING) << "Oh no... Result: " << result << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds (250 ));
    }

    /* Remove every other value */
    for(std::size_t i = 0; i < someValues.size(); i ++) {
        bool result = myBinaryTree->remove(someValues[i]);

        if (result) {
            DLOG(INFO) << *myBinaryTree << std::endl;
        }
        else {
            DLOG(WARNING) << "Oh no... Result: " << result << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds (250 ));
    }

    nResult = EXIT_SUCCESS;
    return nResult;
}
