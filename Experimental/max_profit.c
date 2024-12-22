#include <stdlib.h>

int maxProfit(int* prices, int pricesSize)
{
        // find two points in 'prices' with the biggest positive diff
        size_t best_buy_idx = 0;
        size_t best_sell_idx = 0;
        int best_profit = 0;

        size_t greedy_buy_idx = 0;
        size_t greedy_sell_idx = 0;
        int greedy_profit = 0;

        for (int i = 0; i < pricesSize; i++) {
                int ptnl_profit = prices[i] - prices[greedy_buy_idx];

                // sell as soon as there is a bigger profit
                if (greedy_profit < ptnl_profit) {
                greedy_sell_idx = i;
                greedy_profit = ptnl_profit;
                }

                // was my greedy decision worth it?
                if (best_profit < greedy_profit) {
                best_buy_idx = greedy_buy_idx;
                best_sell_idx = greedy_sell_idx;
                best_profit = greedy_profit;
                }

                // new lowest point found
                if (prices[i] < prices[greedy_buy_idx]) {
                greedy_buy_idx = i;
                }
        }

        return best_profit;
}
