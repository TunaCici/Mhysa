//
// Created by Tuna Cici on 20.03.2023.
//

#include <queue>
#include "DataStructures/BinaryTree.hpp"

namespace data_struct {
    template<typename T>
    BinaryTree<T>::BinaryTree() {
        this->m_pRoot = nullptr;

        this->m_uSize = 0u;
    }

    template<typename T>
    BinaryTree<T>::~BinaryTree() {
        /* std::unique_ptr releases data automatically */
        /* TODO: Possible recursion/stack-over-flow!! Manually release them */
        this->m_pRoot = nullptr;
    }

    template<typename T>
    bool BinaryTree<T>::create_node(const T& input, std::unique_ptr<BTreeNode<T>>& output) {
        bool retValue = false;

        /* Create a new node */
        try {
            output = std::make_unique<BTreeNode<T>>();

            output->key = input;
            output->parent = nullptr;
            output->left = nullptr;
            output->right = nullptr;

            this->m_uSize++;
            retValue = true;
        }
        catch (const std::exception& e) {
            /* TODO: Do nothing for now */
        }

        return retValue;
    }

    template<typename T>
    bool BinaryTree<T>::transplant(BTreeNode<T>* target, std::unique_ptr<BTreeNode<T>> subtree) {
        bool retValue = false;

        /* Target does not exist */
        if (target == nullptr) {
            return retValue;
        }

        /* Temp variables */
        BTreeNode<T>* parent = target->parent;
        BTreeNode<T>* rawSubtree = subtree.get();

        /* Target node is the left child */
        if (target->parent && target == target->parent->left.get()) {
            target->parent->left = std::move(subtree);
        }
        /* Target node is the right child */
        else if (target->parent && target == target->parent->right.get()) {
            target->parent->right = std::move(subtree);
        }
        /* Target node is the root */
        else {
            this->m_pRoot = std::move(subtree);
        }

        /* Update the parent values */
        if (rawSubtree) {
            rawSubtree->parent = parent;

            if (rawSubtree->left) {
                rawSubtree->left->parent = rawSubtree;
            }
            if (rawSubtree->right) {
                rawSubtree->right->parent = rawSubtree;
            }
        }

        retValue = true;

        return retValue;
    }

    template<typename T>
    bool BinaryTree<T>::insert(const T& input) {
        bool retValue = false;

        std::unique_ptr<BTreeNode<T>> newNode{};
        retValue = this->create_node(input, newNode);

        /* Failed to create a new node */
        if (!retValue) {
            return retValue;
        }

        BTreeNode<T>* parent = nullptr;
        BTreeNode<T>* iter = this->m_pRoot.get();

        /* Find where to insert according to the binary tree property */
        while (iter != nullptr) {
            parent = iter;

            if (input < iter->key) {
                iter = iter->left.get();
            }
            else {
                iter = iter->right.get();
            }
        }

        /* Add to the root */
        if (parent == nullptr) {
            this->m_pRoot = std::move(newNode);
        }
        /* Add to left */
        else if (input < parent->key) {
            newNode->parent = parent;
            parent->left = std::move(newNode);
        }
        /* Add to right (default) */
        else {
            newNode->parent = parent;
            parent->right = std::move(newNode);
        }

        return retValue;
    }

    template<typename T>
    bool BinaryTree<T>::remove(const T& target) {
        bool retValue = false;

        BTreeNode<T>* targetNode = this->search(target);

        /* a) If target has no children, then we simply remove it via its parent */
        /* b) If target has one children, then we simply swap target with it's children via its parent */
        /* c) If target has two children, we find it's successor */
        /*  c.1) If the successor is target's children, then we swap target with it's successor via its parent */
        /*  c.2) If the successor is not target's children, the */

        /* Target does not exist */
        if (targetNode == nullptr) {
            return retValue;
        }

        /* Target does not have a left child (Case a | b) */
        if (targetNode->left.get() == nullptr) {
            retValue = this->transplant(targetNode, std::move(targetNode->right));
        }
        /* Target does not have a right child (Case a | b) */
        else if (targetNode->right.get() == nullptr) {
            retValue = this->transplant(targetNode, std::move(targetNode->left));
        }
        /* Target does have both of it's children (Case c) */
        else {
            BTreeNode<T>* successor = this->successor(targetNode);

            /* Case c.1 */
            if (successor == targetNode->right.get()) {
                successor->left = std::move(targetNode->left);
                successor->left->parent = successor;

                retValue = this->transplant(targetNode, std::move(targetNode->right));
            }
            /* Case c.2 */
            else {
                /* Temporarily hold the successor */
                std::unique_ptr<BTreeNode<T>> temp = std::move(successor->parent->left);

                temp->parent->left = std::move(temp->right);

                temp->left = std::move(targetNode->left);
                temp->right = std::move(targetNode->right);

                retValue = this->transplant(targetNode, std::move(temp));
            }
        }

        if (retValue && this->m_uSize != 0u) {
            this->m_uSize--;
        }

        return retValue;
    }

    template<typename T>
    BTreeNode<T>* BinaryTree<T>::search(const T& target) {
        BTreeNode<T>* retValue = this->m_pRoot.get();

        while (retValue && target != retValue->key) {
            if (target < retValue->key) {
                retValue = retValue->left.get();
            } else {
                retValue = retValue->right.get();
            }
        }

        return retValue;
    }

    template<typename T>
    BTreeNode<T>* BinaryTree<T>::successor(const BTreeNode<T>* target) {
        BTreeNode<T>* retValue = nullptr;

        if (target == nullptr) {
            return retValue;
        }

        if (target->right) {
            return this->min(target->right.get());
        }

        retValue = target->parent;

        while (retValue && target == retValue->right.get()) {
            target = retValue;
            retValue = retValue->parent;
        }

        return retValue;
    }

    template<typename T>
    BTreeNode<T>* BinaryTree<T>::predecessor(const BTreeNode<T>* target) {
        BTreeNode<T>* retValue = nullptr;

        if (target == nullptr) {
            return retValue;
        }

        if (target->left) {
            return this->max(target->left.get());
        }

        retValue = target->parent;

        while (retValue && target == retValue->left.get()) {
            target = retValue;
            retValue = retValue->parent;
        }

        return retValue;
    }

    template<typename T>
    BTreeNode<T>* BinaryTree<T>::min(const BTreeNode<T>* target) {
        BTreeNode<T>* retValue = this->m_pRoot.get();

        /* Start searching from the 'target' node */
        /* If 'target' is nullptr, start searching from the root */
        while (target && retValue != target) {
            if (target->key < retValue->key) {
                retValue = retValue->left.get();
            } else {
                retValue = retValue->right.get();
            }
        }

        /* Iterate to the leftest child */
        while (retValue && retValue->left) {
            retValue = retValue->left.get();
        }

        return retValue;
    }

    template<typename T>
    BTreeNode<T>* BinaryTree<T>::max(const BTreeNode<T>* target) {
        BTreeNode<T>* retValue = this->m_pRoot.get();

        /* Start searching from the 'target' node */
        /* If 'target' is nullptr, start searching from the root */
        while (target && retValue != target) {
            if (target->key < retValue->key) {
                retValue = retValue->left.get();
            } else {
                retValue = retValue->right.get();
            }
        }

        /* Iterate to the rightest child */
        while (retValue && retValue->right) {
            retValue = retValue->right.get();
        }

        return retValue;
    }

    template<typename T>
    std::size_t BinaryTree<T>::size() {
        return this->m_uSize;
    }

    template<typename T>
    std::size_t BinaryTree<T>::height_helper(const BTreeNode<T>* target) {
        std::size_t retValue = 0u;

        if (target != nullptr) {
            retValue = 1u + std::max(
                    this->height_helper(target->left.get()),
                    this->height_helper(target->right.get())
            ) ;
        }

        return retValue;
    }

    template<typename T>
    std::size_t BinaryTree<T>::height(const BTreeNode<T>* target) {
        std::size_t retValue = 0u;

        /* If target is not specified, then start from the root */
        if (target == nullptr) {
            target = this->m_pRoot.get();
        }

        retValue = this->height_helper(target);

        return retValue;
    }

    template<typename T>
    void BinaryTree<T>::inorder_print(std::ostream& os,  const BTreeNode<T>* curr_node) const {
        if (curr_node == nullptr) {
            os << "_";
        }
        else {
            os << "[";
            this->inorder_print(os, curr_node->left.get());
            os << " " << curr_node->key << " ";
            this->inorder_print(os, curr_node->right.get());
            os << "]";
        }
    }

    template<typename T>
    void BinaryTree<T>::preorder_print(std::ostream& os, const BTreeNode<T>* curr_node) const {
        if (curr_node == nullptr) {
            os << "_";
        }
        else {
            os << "[" << curr_node->key << " ";
            this->preorder_print(os, curr_node->left.get());
            os << " ";
            this->preorder_print(os, curr_node->right.get());
            os << "]";
        }
    }

    template<typename U>
    std::ostream& operator<<(std::ostream& os, const BinaryTree<U>& obj) {
        const std::size_t maxAllowedSize = 8u;

        if (obj.m_uSize <= maxAllowedSize){
            obj.preorder_print(os, obj.m_pRoot.get());
        }
        else {
            obj.preorder_print(os, obj.m_pRoot.get());
        }

        return os;
    }
}