#include <stdio.h>
#include <stdlib.h>


struct ListNode {
        int val;
        struct ListNode *next;
};

inline int my_max(int x, int y)
{
        return ((x < y) ? y : x); 
}

struct ListNode* mergeTwoLists(struct ListNode* list1, struct ListNode* list2) {
        if (!list1 && !list2) {
                return NULL;
        }

        struct ListNode *l1_iter = list1;
        struct ListNode *l2_iter = list2;

        // Merged
        struct ListNode *l3_head = NULL;
        struct ListNode *l3_iter = NULL;

        while (l1_iter && l2_iter) {
                struct ListNode *tmp = (struct ListNode*) malloc(sizeof(struct ListNode));
                tmp->val = my_max(l1_iter->val, l2_iter->val);
                tmp->next = NULL;

                // Chooce which list to take from
                if (l1_iter->val < l2_iter->val) {
                        tmp->val = l1_iter->val;
                        l1_iter = l1_iter->next;
                } else {
                        tmp->val = l2_iter->val;
                        l2_iter = l2_iter->next;
                }

                // Maintain the list
                if (l3_head) {
                        l3_iter->next = tmp;
                } else {
                        l3_head = tmp;
                }

                l3_iter = tmp;
        }

        // See if any list is left
        while (l1_iter) {
                struct ListNode *tmp =
                        (struct ListNode*) malloc(sizeof(struct ListNode));
                tmp->val = l1_iter->val;
                tmp->next = NULL;

                // Maintain the list
                if (l3_head) {
                        l3_iter->next = tmp;
                } else {
                        l3_head = tmp;
                }

                l1_iter = l1_iter->next;
                l3_iter = tmp;
                }

                while (l2_iter) {
                struct ListNode *tmp =
                        (struct ListNode*) malloc(sizeof(struct ListNode));
                tmp->val = l2_iter->val;
                tmp->next = NULL;

                // Maintain the list
                if (l3_head) {
                        l3_iter->next = tmp;
                } else {
                        l3_head = tmp;
                }

                l2_iter = l2_iter->next;
                l3_iter = tmp;
        }

        return l3_head;
}
