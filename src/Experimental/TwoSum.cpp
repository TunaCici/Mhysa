#include <iostream>
#include <vector>
#include <map>

using namespace std;

class Solution {
public:
        vector<int> twoSum(vector<int>& nums, int target) {   
                std::vector<int> retValue = {};

                std::map<int, int> history = {};

                for (int i = 0; i < nums.size(); i++) {
                        int diff = target - nums[i];

                        if (history.contains(diff)) {
                                retValue.push_back(history[diff]);
                                retValue.push_back(i);

                                break;
                        }

                        history[nums[i]] = i;
                }

                return retValue;
        }
};

int main ()
{
        std::vector<int> numbers = {2, 7, 11, 15};
        int target = 9;

        Solution solution;

        std::cout << "numbers: ";
        for (auto v : numbers) {
                std::cout << v << ", ";
        }
        std::cout << std::endl;

        auto retValue = solution.twoSum(numbers, target);

        std::cout << "twoSum: ";
        for (auto v : retValue) {
                std:: cout << v << ", ";
        }
        std::cout << std::endl;

        return 0;
}
