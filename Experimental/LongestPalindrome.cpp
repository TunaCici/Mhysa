#include <iostream>
#include <unordered_set>

using namespace std;

class Solution {
public:
        std::pair<int, int> expandToSides(const string &s, int left, int right) {
                std::pair<int, int> retValue{}; /* size of the valid plaindrom 'string' */

                /* a. check idx bounds */
                /* b. check char equality */
                while (0 <= left && right < s.size() && s[left] == s[right]) {
                        left--;
                        right++;
                }
                
                retValue = std::make_pair(left, right);

                return retValue;
        }

        string longestPalindrome(string s) {
                std::string retValue{};

                if (s.size() <= 1) {
                        return s;
                }

                for (int i = 0; i < s.size(); i++) {
                        /* 1. Assume 'i' does not have a buddy */
                        int left = i;
                        int right = i;

                        auto pairs = expandToSides(s, left, right);
                        auto substrSize = pairs.second - pairs.first - 1;

                        if (retValue.size() < substrSize) {
                                retValue = s.substr(pairs.first + 1, substrSize);
                        }

                        /* 2. Assume 'i' does have a buddy */
                        left = i;
                        right = i + 1;
                       
                        pairs = expandToSides(s, left, right);
                        substrSize = pairs.second - pairs.first - 1;
                        
                        if (retValue.size() < substrSize) {
                                retValue = s.substr(pairs.first + 1, substrSize);
                        }
                }

                return retValue;
        }
};

int main ()
{
        Solution solution;

        std::string test = "xxracecarxx";

        std::cout << solution.longestPalindrome(test) << std::endl;

        return 0;
}
