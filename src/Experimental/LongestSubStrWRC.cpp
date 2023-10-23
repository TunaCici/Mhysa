#include <iostream>
#include <unordered_map>

using namespace std;

class Solution {
public:
        int lengthOfLongestSubstring(string s) {
                int maxLength = 0; /* return value */

                std::unordered_map<char, int> lastEncounters = {};

                int left = 0;
                int right = 0;

                for (; right < s.length(); right++) {
                        if (lastEncounters.contains(s[right]) && 
                                left <= lastEncounters[s[right]]) {
                                left = lastEncounters[s[right]] + 1;
                        }

                        lastEncounters[s[right]] = right;

                        maxLength = std::max(maxLength, right - left + 1);
                }

                return maxLength;
        }
};

int main ()
{
        std::string str1 = "pwwkew";

        Solution solution;

        std::cout << solution.lengthOfLongestSubstring(str1) << std::endl;

        return 0;
}
