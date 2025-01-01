/**
 * Definition for a binary tree node.
 * struct TreeNode {
 *     int val;
 *     struct TreeNode *left;
 *     struct TreeNode *right;
 * };
 */

 struct TreeNode {
        int val;
        struct TreeNode *left;
        struct TreeNode *right;
 };

#include <stdlib.h>


int tree_height(struct TreeNode *root)
{
        if (!root) {
                return 0;
        }

        int mh = 0;
        int lh = 0;
        int rh = 0;

        lh = tree_height(root->left);
        rh = tree_height(root->right);
        
        mh = (lh < rh) ? rh : lh;

        return mh + 1;
}

int dfs_path_search(struct TreeNode *root, struct TreeNode* target, struct TreeNode *path[], size_t path_idx)
{
        if (!root || !target) {
                return 0;
        }

        if (root->val == target->val) {
                path[path_idx] = root;
                return 1;
        }

        int res = 0;

        res = dfs_path_search(root->left, target, path, path_idx + 1);
        if (res) {
                path[path_idx] = root;
                return 1;
        }

        res = dfs_path_search(root->right, target, path, path_idx + 1);
        if (res) {
                path[path_idx] = root;
                return 1;
        }

        return 0;
}

struct TreeNode* lowestCommonAncestor(struct TreeNode* root, struct TreeNode* p, struct TreeNode* q)
{
        struct TreeNode **path_p = NULL;
        struct TreeNode **path_q = NULL;

        size_t path_size = tree_height(root) + 1;

        path_p = (struct TreeNode**) calloc(path_size, sizeof(struct TreeNode*));
        path_q = (struct TreeNode**) calloc(path_size, sizeof(struct TreeNode*));

        int res = 0;
        res = dfs_path_search(root, p, path_p, 0);
        if (!res) {
                return NULL;
        }

        res = dfs_path_search(root, q, path_q, 0);
        if (!res) {
                return NULL;
        }

        struct TreeNode *lca = NULL;
        for (int i = 0; i < path_size; i++) {
                if (!path_p[i] || !path_q[i]) {
                        break;
                }

                if (path_p[i]->val != path_q[i]->val) {
                        break;
                }

                lca = path_p[i];
        }

        return lca;
}