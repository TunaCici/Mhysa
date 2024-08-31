//
// Created by Tuna Cici on 5.05.2023.
//

#include <iostream>

/* One of the most popular and essential algorithm: Coin Change */
/* Calculates the minimum number of banknotes needed to make up the target money */
unsigned int coin_change(
        const std::vector<unsigned short> &banknotes, const int &target);

static unsigned int coin_change_helper(
        std::vector<unsigned int> &cache, const std::vector<unsigned short> &banknotes, const int &target);

unsigned int coin_change(const std::vector<unsigned short> &banknotes, const int &target) {
    static std::vector<unsigned int> cache{};

    /* Cache size is static (no need for dynamic) */
    if (cache.capacity() == 0) {
        cache.resize(1024, std::numeric_limits<unsigned int>::max());
    }

    unsigned int ret_value = 0u;

    ret_value = coin_change_helper(cache, banknotes, target);

    return ret_value;
}

static unsigned int
coin_change_helper(std::vector<unsigned int> &cache, const std::vector<unsigned short> &banknotes, const int &target) {
    /* Termination cases */
    if (target == 0u) return 0u;
    else if (target < 0) return std::numeric_limits<unsigned int>::max() - 1;

    /* We got a cache hit! */
    if (cache[target] < std::numeric_limits<unsigned int>::max()) {
        return cache[target];
    }

    unsigned int ret_value = std::numeric_limits<unsigned int>::max();

    /* Try every banknote */
    for (unsigned int i: banknotes) {
        ret_value = std::min(ret_value, (coin_change_helper(cache, banknotes, target - i) + 1u));
    }

    /* Memoize */
    if (ret_value != std::numeric_limits<unsigned int>::max()) {
        cache[target] = ret_value;
    }

    return ret_value;
}

