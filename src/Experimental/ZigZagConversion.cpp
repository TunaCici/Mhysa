#include <iostream>

using namespace std;

class Solution {
public:
        string convert(string s, int numRows) {
                std::string retValue{};

                /* Buffer (TODO: make it dynamic the next time) */
                char buff[1024][1024] = {0};

                bool isGoingUp = false;

                int targetX = 0;
                int targetY = 0;

                /* edge case when numRows is 1 */
                if (numRows == 1) {
                        return s;
                }

                /* print it onto the 2D buffer */
                for (auto i = 0; i < s.length(); i++) {
                        buff[targetY][targetX] = s[i];
                        std::cout << "put " << s[i] << " @ " << targetY << ", " << targetX << std::endl;

                        /* should reflect? */
                        if (!isGoingUp && numRows <= (targetY + 1)) {
                                isGoingUp = true;
                        } else if (isGoingUp && (targetY - 1) < 0) {
                                isGoingUp = false;
                        }

                        /* determine next target idx */ 
                        if (isGoingUp) {
                                targetY--;
                                targetX++;
                        } else {
                                targetY++;
                        }
                }

                /* debug */
                for (auto i = 0; i < numRows; i++) {
                        for (auto j = 0; j <= targetX; j++) {
                                if (buff[i][j] != 0) {
                                        std::cout << buff[i][j];
                                } else {
                                        std::cout << ' ';
                                }
                        }

                        std::cout << std::endl;
                }

                /* finally, convert to 1D array */
                for (auto i = 0; i < numRows; i++) {
                        for (auto j = 0; j <= targetX; j++) {
                                if (buff[i][j] != 0) {
                                        retValue.push_back(buff[i][j]);
                                }
                        }
                }

                return retValue;
        }
};

int main ()
{
        Solution solution;

        std::string testThis = "SAYGOODBYE";
        std::string result = solution.convert(testThis, 3);

        std::cout << result << std::endl;

        return 0;
}
