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
    extern unsigned int coin_change(const std::vector<unsigned short> &banknotes, const int &target);

    const std::vector<unsigned short> banknotes = {1u, 2u, 5u, 10u, 20u, 50u, 100u};

    for (auto i = 0; i < 32; i++)
    {
        DLOG(INFO) << "coin_change(" << i * 10u << ") is " << coin_change(banknotes, i * 10u);
    }

    /* Defined in src/Experimental/RodCutting.cpp */
    extern unsigned int cut_rod(const std::vector<unsigned int> &prices, const int &n);

    const std::vector<unsigned int> prices = {0, 1, 5, 8, 9, 10, 17, 17, 20, 24, 30};

    for (auto i = 0; i < 11; i++)
    {
        DLOG(INFO) << "cut_rod(" << i << ") is " << cut_rod(prices, i);
    }

    /* Defined in src/Experimental/BasicVirtualATS.cpp */
    extern std::tuple<std::uint64_t, std::uint64_t> translate_64(const std::uint64_t &virt_addr);
    auto [p, d] = translate_64(0x00000000A35610A7);

    DLOG(INFO) << std::hex << "p: 0x" << p << " d: 0x" << d;
#endif

    nResult = EXIT_SUCCESS;
    return nResult;
}
