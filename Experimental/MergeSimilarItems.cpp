#include <iostream>
#include <vector>
#include <map>

using namespace std;

class Solution {
public:
        vector<vector<int>> mergeSimilarItems(vector<vector<int>>& items1, vector<vector<int>>& items2) {
                vector<vector<int>> retValue = {};
        
                std::map<int, int> intermediaryBuff = {};    

                for (auto v : items1) {
                        intermediaryBuff[v.front()] = v.back();
                }
                
                for (auto v : items2) {
                        if (intermediaryBuff.contains(v.front())) {
                                intermediaryBuff[v.front()] = intermediaryBuff[v.front()] + v.back();
                        } else {
                                intermediaryBuff[v.front()] = v.back();
                        }

                }

                retValue.reserve(intermediaryBuff.size());

                for (auto elem : intermediaryBuff) {
                        retValue.push_back(std::vector<int>{elem.first, elem.second});
                }

                return retValue;
        }
};

int main ()
{
        std::vector<vector<int>> items1 = {};
        std::vector<vector<int>> items2 = {};

        items1.push_back(std::vector<int>{1, 3});
        items1.push_back(std::vector<int>{2, 6});
        items1.push_back(std::vector<int>{3, 8});

        items2.push_back(std::vector<int>{1, 5});
        items2.push_back(std::vector<int>{2, 4});
        items2.push_back(std::vector<int>{3, 3});

        std::cout << "items1 (values): ";
        for (auto v : items1) {
                std::cout << v.front() << ", " << v.back() << "; ";
        }
        std::cout << std::endl;
        
        std::cout << "items2 (values): ";
        for (auto v : items2) {
                std::cout << v.front() << ", " << v.back() << "; ";
        }
        std::cout << std::endl;

        Solution solution;
        auto retValue = solution.mergeSimilarItems(items1, items2);

        std::cout << "mergeSimiliarItems: ";
        for (auto v : retValue) {
                std::cout << v.front() << ", " << v.back() << "; ";
        }
        std::cout << std::endl;

        return 0;
}
