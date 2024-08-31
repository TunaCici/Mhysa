//
// Created by Tuna Cici on 25.04.2023.
//

#include <vector>

/* Calculates fibonacci(n) using memoization */
unsigned int fib(const unsigned int &n);

static unsigned int fib_helper(std::vector<unsigned int> &cache, const unsigned int &n);

/* Implementation */
unsigned int fib(const unsigned int &n) {
    /* The first two values [0] [1] are 1 by definition */
    static std::vector<unsigned int> cache = {1u, 1u};

    /* Does this really necessary? I'm too lazy to check */
    if (n == 0u) return 0u;
    else if (n == 1u or n == 2u) return 1u;

    /* Increase cache size (this is stupid but nvm) */
    if (cache.size() < n) {
        cache.reserve(n * 2u);
    }

    unsigned int ret_value = 0u;

    ret_value = fib_helper(cache, n - 1u) + fib_helper(cache, n - 2u);

    return ret_value;
}

static unsigned int fib_helper(std::vector<unsigned int> &cache, const unsigned int &n) {
    /* Termination cases */
    if (n == 0u) return 0u;
    else if (n == 1u or n == 2u) return 1u;

    /* Check if exist in vec? (cache hit) */
    if (cache[n] != 0u) {
        return cache[n];
    }

    unsigned int ret_value = 0u;

    ret_value = fib_helper(cache, n - 1u) + fib_helper(cache, n - 2u);

    /* Memoize */
    cache[n] = ret_value;

    return ret_value;
}
