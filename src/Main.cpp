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

#ifdef DEBUG
    /* Init Google Logging */
    google::InitGoogleLogging(argv[0]);
    fLB::FLAGS_logtostderr = true;
#endif

#ifdef EXPERIMENTAL
    /* Only active when building for target Mhysa_Experimental */

    /* Defined in src/Experimental/Fibonacci.cpp */
    extern unsigned int fib(const unsigned int &n);

    for (auto i = 0; i < 32; i++)
    {
        DLOG(INFO) << "fib(" << i << ") is " << fib(i);
    }

    /* Defined in src/Experimental/CoinChange.cpp */
    unsigned int coin_change(const std::vector<unsigned short> &banknotes, const int &target);

    const std::vector<unsigned short> banknotes = {1u, 2u, 5u, 10u, 20u, 50u, 100u};

    for (auto i = 0; i < 32; i++)
    {
        DLOG(INFO) << "coin_change(" << i * 10u << ") is " << coin_change(banknotes, i * 10u);
    }

#endif

    nResult = EXIT_SUCCESS;
    return nResult;
}
