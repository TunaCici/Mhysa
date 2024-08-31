#include <iostream>
#include <vector>
#include <map>

using namespace std;

class Solution {
public:
        bool isPalindrome(int x) {
                if (x < 0) {
                        return false;
                }

                int number = x;
                int digit_count = 0;
                uint64_t left_mask = 1;
                uint64_t right_mask = 10;

                /* Count digits */
                for (int i = x; i != 0; i/=10) {
                        digit_count++;
                        left_mask *= 10;
                }

                /* Off-by-one correction */
                left_mask /= 10;

                // std::cout << "digit_count: " << digit_count << std::endl;
                // std::cout << "left_mask: " << left_mask << std::endl;
                
                while (0 < digit_count) {
                        int right_digit = number ? (number % right_mask) : 0;
                        int left_digit = number ? (number / left_mask) : 0;

                        if (left_digit != right_digit) {
                                return false;
                        }

                        // std::cout << number << ": " << left_digit << ", " << right_digit << std::endl;

                        /* Remove left digit and right digit (in order) */
                        number = number ? (number % left_mask) : 0;
                        number = number ? (number / 10) : 0;

                        /* We just removed 2 digits */
                        left_mask /= 100;
                        digit_count -= 2;
                }

                return true;
        }
};

int main ()
{
        std::vector<int> numbers = {121, 1234321, 7667, 45, 135, 1000000001};

        Solution solution;

        for (auto i : numbers) {
                auto result = solution.isPalindrome(i);
                std::cout << result << std::endl;

        }

        return 0;
}
