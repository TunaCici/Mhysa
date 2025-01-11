struct ListNode {
        int val;
        struct ListNode *next;
};

#include <stdlib.h>

int compare_node(const void *a, const void *b)
{
        return (*((struct ListNode**) a))->val
                - (*((struct ListNode**) b))->val;
}

struct ListNode* mergeKLists(struct ListNode** lists, int listsSize)
{
        if (!lists || listsSize <= 0) {
                return NULL;
        }

        // count all nodes O(n)
        size_t nodes_count = 0;
        for (int i = 0; i < listsSize; i++) {
                struct ListNode *iter = lists[i];

                while (iter) {
                        nodes_count++;
                        iter = iter->next;
                }
        }

        if (nodes_count == 0) {
                return NULL;
        }

        // represent them in an array
        struct ListNode **nodes_arr = (struct ListNode**)
                malloc(nodes_count * sizeof(struct ListNode*));
        size_t curr_idx = 0;

        for (int i = 0; i < listsSize; i++) {
                struct ListNode *iter = lists[i];

                while (iter) {
                nodes_arr[curr_idx++] = iter;
                iter = iter->next;
                }
        }

        // sort them
        qsort(nodes_arr, nodes_count, sizeof(struct ListNode*), compare_node);

        // fix the links
        struct ListNode *merged_head = nodes_arr[0];
        struct ListNode *prev = merged_head;
        for (int i = 1; i < nodes_count; i++) {
                prev->next = nodes_arr[i];
                prev = nodes_arr[i];
        }

        // tail node
        prev->next = NULL;

        return merged_head;
}