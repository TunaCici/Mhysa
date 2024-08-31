#include <iostream>
#include <limits>
#include <cstdint>

using namespace std;

class Solution {
public:
        int reverse(int x) {   
                int retValue = {};

                while (x != 0) {
                        /* check for int bounds */
                        std::int64_t temp = ((std::int64_t)retValue * 10) + (x % 10);
                        if (std::numeric_limits<int>::max() < temp) {
                                return 0;
                        } else if (temp < std::numeric_limits<int>::min()) {
                                return 0;
                        }

                        retValue = (retValue * 10) + (x % 10);

                        x /= 10; 
                }

                return retValue;
        }
};

int main ()
{

        Solution solution;

        auto retValue = solution.reverse(-1234689123);

        std::cout << "reverse: " << retValue << std::endl;

        return 0;
}
