#include <iostream>
#include <vector>
#include <map>

using namespace std;

 struct ListNode {
        int val;
        ListNode *next;
        
        ListNode() : val(0), next(nullptr) {}
        ListNode(int x) : val(x), next(nullptr) {}
        ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution {
public:
        ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
                struct ListNode *retList = nullptr;

                struct ListNode *iter1 = l1;
                struct ListNode *iter2 = l2;

                struct ListNode *iter3 = retList;

                int sum = 0;
                int carry = 0;

                int digit1 = 0;
                int digit2 = 0;

                while (iter1 || iter2) {
                        digit1 = iter1 != nullptr ? iter1->val : 0;
                        digit2 = iter2 !=nullptr ? iter2->val : 0;

                        sum = digit1 + digit2 + carry;
                        
                        carry = sum / 10;
                        sum = sum % 10;

                        if (retList == nullptr) {
                                iter3 = new struct ListNode(sum);
                                retList = iter3;
                        } else {
                                iter3->next = new struct ListNode(sum);
                                iter3 = iter3->next;
                        }

                        iter1 = iter1 != nullptr ? iter1->next : nullptr;
                        iter2 = iter2 != nullptr ? iter2->next : nullptr;
                }

                if (carry) {
                        iter3->next = new struct ListNode(carry);
                        iter3 = iter3->next;
                }

                return retList;
        }
};

int main ()
{
        struct ListNode *list1 = {};
        struct ListNode *list2 = {};

        list1 = new struct ListNode(2);
        list1->next = new struct ListNode(4);
        list1->next->next = new struct ListNode(3);

        list2 = new struct ListNode(5);
        list2->next = new struct ListNode(6);
        list2->next->next = new struct ListNode(4);

        Solution solution;
        struct ListNode *retValue = solution.addTwoNumbers(list1, list2);

        auto iter = retValue;

        while (iter) {
                std::cout << iter->val << ", "; 

                iter = iter->next;
        }
        std::cout << std::endl;

        return 0;
}
