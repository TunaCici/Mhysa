int fib_cache(int n)
{
    static int cache[4096] = {0};

    if (n == 0 || n == 1) {
        return n;
    }

    if (cache[n] != 0) {
        return cache[n];
    }

    int res_2 = fib_cache(n - 2);
    int res_1 = fib_cache(n - 1);

    cache[n] = res_2 + res_1;

    return cache[n];
}

int fib(int n)
{
    int res = fib_cache(n);

    return res;
}