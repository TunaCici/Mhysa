//
// Created by Tuna Cici on 7.05.2023.
//

#include <_types/_uint32_t.h>
#include <sys/_types/_int32_t.h>
#include <vector>

/* A very popular dynamic programming problem: Rod cutting */
/* Calculates the 'maximum' amount of money that can earned by cutting the target rod */
unsigned int cut_rod(const std::vector<unsigned int> &prices, const int &n);

unsigned int cut_rod_helper(std::vector<unsigned int> &cache, const std::vector<unsigned int> &prices, const int &n);

unsigned int cut_rod(const std::vector<unsigned int> &prices, const int &n) {
    static std::vector<unsigned int> cache{};

    /* Cache size is static (no need for dynamic) */
    if (cache.capacity() == 0) {
        cache.resize(1024, 0u);
    }

    unsigned int ret_value = 0u;

    ret_value = cut_rod_helper(cache, prices, n);

    return ret_value;
}

unsigned int cut_rod_helper(std::vector<unsigned int> &cache, const std::vector<unsigned int> &prices, const int &n) {
    /* Termination case */
    if (n <= 0) return 0u;

    /* Cache hit !!! */
    if (cache[n] != 0u) {
        return cache[n];
    }

    unsigned int ret_value = 0u;

    /* Try every price */
    for (auto i = 1; i < prices.size(); i++) {
        if (0 <= n - i) {
            ret_value = std::max(ret_value, prices[i] + cut_rod_helper(cache, prices, n - i));
        }
    }

    /* Memoize */
    cache[n] = ret_value;

    return ret_value;
}