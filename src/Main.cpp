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
#include "DataStructures/RedBlackTree.hpp"

int main(int argc, char** argv) {
    int nResult = EXIT_FAILURE;

    // Initialize logging
    #ifdef DEBUG
        google::InitGoogleLogging(argv[0]);
        fLB::FLAGS_logtostderr = true;
    #endif

    std::unique_ptr<data_struct::RedBlackTree<int>> myRedBlackTree;
    myRedBlackTree = std::make_unique<data_struct::RedBlackTree<int>>();

    std::vector<int> someValues = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 12, 14, 15};

    for(int someValue : someValues) {
        bool result = myRedBlackTree->insert(someValue);

        if (result) {
            DLOG(INFO) << *myRedBlackTree << std::endl;
        }
        else {
            DLOG(WARNING) << "Oh no... Result: " << result << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds (500 ));
    }

    for (int someValue : someValues) {
        bool result = myRedBlackTree->remove(someValue);

        if (result) {
            DLOG(INFO) << *myRedBlackTree << std::endl;
        }
        else {
            DLOG(WARNING) << "Oh no... Result: " << result << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds (500 ));
    }


    nResult = EXIT_SUCCESS;
    return nResult;
}
