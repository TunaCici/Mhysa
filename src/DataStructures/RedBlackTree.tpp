//
// Created by Tuna Cici on 23.03.2023.
//

#include "DataStructures/RedBlackTree.hpp"

namespace data_struct {
    template<typename T>
    RedBlackTree<T>::RedBlackTree() {
        this->m_uSize = 0u;
    }

    template<typename T>
    RedBlackTree<T>::~RedBlackTree() {
        /* std::unique_ptr releases data automatically */
        /* TODO: Possible recursion/stack-over-flow!! Manually release them */
        this->m_pRoot.release();
    }

    template<typename T>
    bool RedBlackTree<T>::create_node(const T& input, std::unique_ptr<RBTreeNode<T>>& output) {
        bool retValue = false;

        /* Create a new node */
        try {
            output = std::make_unique<RBTreeNode<T>>();

            output->key = input;
            output->color = RBColor::RED;

            output->parent = RB_SENTINEL;
            output->left = RB_SENTINEL;
            output->right = RB_SENTINEL;

            this->m_uSize++;
            retValue = true;
        }
        catch (const std::exception& e) {
            /* TODO: Do nothing for now */
        }

        return retValue;
    }

    template<typename T>
    void RedBlackTree<T>::transplant(RBTreeNode<T>* target, std::unique_ptr<RBTreeNode<T>> subtree) {
        /* Temp variables */
        RBTreeNode<T>* parent = target->parent;
        RBTreeNode<T>* rawSubtree = subtree.get();

        /* Target does not exist */
        if (target == nullptr) {
            return;
        }

        /* Target is the root */
        if (target->parent == RB_SENTINEL) {
            this->m_pRoot = std::move(subtree);
        }
        /* Target node is a left child */
        else if (target == target->parent->left.get()) {
            target->parent->left = std::move(subtree);
        }
        /* Target node is a right child */
        else {
            target->parent->right = std::move(subtree);
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
    }

    template<typename T>
    void RedBlackTree<T>::rotate_left(std::unique_ptr<RBTreeNode<T>>& target) {
        /* Right child does not exist */
        if (!target->right) {
            return;
        }

        RBTreeNode<T>* parent = target->parent;
        std::unique_ptr<RBTreeNode<T>> rightChild = std::move(target->right);

        /* Update target's right child */
        target->right = std::move(rightChild->left);
        if (target->right) {
            target->right->parent = target.get();
        }

        /* Target is the root */
        if (parent == RB_SENTINEL) {
            rightChild->left = std::move(this->m_pRoot);
            rightChild->left->parent = rightChild.get();

            this->m_pRoot = std::move(rightChild);
            this->m_pRoot->parent = RB_SENTINEL;
        }
        else {
            bool isLeftChild = (target == parent->left);

            /* Move the target to rightChild's left */
            rightChild->left = std::move(target);
            rightChild->left->parent = rightChild.get();

            /* Target was the left child */
            if (isLeftChild) {
                parent->left = std::move(rightChild);
                parent->left->parent = parent;
            }
            /* Target was the right child */
            else {
                parent->right = std::move(rightChild);
                parent->right->parent = parent;
            }
        }
    }

    template<typename T>
    void RedBlackTree<T>::rotate_right(std::unique_ptr<RBTreeNode<T>>& target) {
        /* Left child does not exist */
        if (!target->left) {
            return;
        }

        RBTreeNode<T>* parent = target->parent;
        std::unique_ptr<RBTreeNode<T>> leftChild = std::move(target->left);

        /* Update target's left child */
        target->left = std::move(leftChild->right);
        if (target->left) {
            target->left->parent = target.get();
        }

        /* Target is the root */
        if (parent == RB_SENTINEL) {
            leftChild->right = std::move(this->m_pRoot);
            leftChild->right->parent = leftChild.get();

            this->m_pRoot = std::move(leftChild);
            this->m_pRoot->parent = RB_SENTINEL;
        }
        else {
            bool isLeftChild = (target == parent->left);

            /* Move the target to leftChild's right */
            leftChild->right = std::move(target);
            leftChild->right ->parent = leftChild.get();

            /* Target was the left child */
            if (isLeftChild) {
                parent->left = std::move(leftChild);
                parent->left->parent = parent;
            }
            /* Target was the right child */
            else {
                parent->right = std::move(leftChild);
                parent->right->parent = parent;
            }
        }
    }

    template<typename T>
    void RedBlackTree<T>::insert_fixup(std::unique_ptr<RBTreeNode<T>>& target) {
        /* We have 4 different cases */
        /* Case 0. target is m_pRoot: Just color it 'BLACK'  */
        /* Case 1. parent's sibling is 'RED':  Apply recoloring */
        /* Case 2. parent's sibling is 'BLACK' and target is 'right': Apply rotate_left() */
        /* Case 3. parent's sibling is 'BLACK' and target is 'left': Apply rotate_right() */

        RBTreeNode<T>* parent = RB_SENTINEL;
        RBTreeNode<T>* parentSibling = RB_SENTINEL;
        RBTreeNode<T>* grandParent = RB_SENTINEL;

        parent = target->parent;

        while (parent && parent->color == RBColor::RED) {
            grandParent = parent->parent;

            /* Parent is the left child */
            if (grandParent && parent == grandParent->left.get()) {
                parentSibling = grandParent->right.get();

                /* Case 1. */
                if (parentSibling && parentSibling->color == RBColor::RED) {
                    parent->color = RBColor::BLACK;
                    parentSibling->color = RBColor::BLACK;
                    grandParent->color = RBColor::RED;

                    parent = grandParent->parent;
                }
                else {
                    /* Case 2. */
                    if (target == parent->right) {
                        this->rotate_left(grandParent->left);

                        parent = grandParent->left.get();
                    }

                    /* Case 3. */
                    parent->color = RBColor::BLACK;
                    grandParent->color = RBColor::RED;

                    /* Some smart pointer bs */
                    auto grandGrandParent = grandParent->parent;
                    if (grandGrandParent == RB_SENTINEL) {
                        this->rotate_right(this->m_pRoot);
                    }
                    else if (grandParent == grandGrandParent->left.get()) {
                        this->rotate_right(grandGrandParent->left);
                    }
                    else {
                        this->rotate_right(grandGrandParent->right);
                    }
                }
            }
            /* Parent is the right child */
            else {
                parentSibling = grandParent->left.get();

                /* Case 1. */
                if (parentSibling && parentSibling->color == RBColor::RED) {
                    parent->color = RBColor::BLACK;
                    parentSibling->color = RBColor::BLACK;
                    grandParent->color = RBColor::RED;

                    parent = grandParent->parent;
                }
                else {
                    /* Case 2. */
                    if (target == parent->left) {
                        this->rotate_right(grandParent->right);

                        parent = grandParent->right.get();
                    }

                    /* Case 3. */
                    parent->color = RBColor::BLACK;
                    grandParent->color = RBColor::RED;

                    /* Some smart pointer bs */
                    /* TODO: Find a better way to do this */
                    auto grandGrandParent = grandParent->parent;
                    if (grandGrandParent == RB_SENTINEL) {
                        this->rotate_left(this->m_pRoot);
                    }
                    else if (grandParent == grandGrandParent->left.get()) {
                        this->rotate_left(grandGrandParent->left);
                    }
                    else {
                        this->rotate_left(grandGrandParent->right);
                    }
                }
            }
        }

        /* Update 'm_pRoot' color */
        this->m_pRoot->color = RBColor::BLACK;
    }

    template<typename T>
    bool RedBlackTree<T>::insert(const T& input) {
        bool retValue = false;

        std::unique_ptr<RBTreeNode<T>> newNode{};
        retValue = this->create_node(input, newNode);

        /* Failed to create a new node */
        if (!retValue) {
            return retValue;
        }

        RBTreeNode<T>* parent = RB_SENTINEL;
        RBTreeNode<T>* iter = this->m_pRoot.get();

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
        if (parent == RB_SENTINEL) {
            this->m_pRoot = std::move(newNode);

            this->insert_fixup(this->m_pRoot);
        }
        /* Add to left */
        else if (input < parent->key) {
            newNode->parent = parent;
            parent->left = std::move(newNode);

            this->insert_fixup(parent->left);
        }
        /* Add to right (default) */
        else {
            newNode->parent = parent;
            parent->right = std::move(newNode);

            this->insert_fixup(parent->right);
        }

        return retValue;
    }

    template<typename T>
    void RedBlackTree<T>::remove_fixup(std::unique_ptr<RBTreeNode<T>>& target) {

    }

    template<typename T>
    bool RedBlackTree<T>::remove(const T& target) {
        bool retValue = false;

        RBTreeNode<T>* targetNode = this->search_node(target);

        /* We have 4 different cases */


        /* Target does not exist */
        if (!targetNode) {
            return retValue;
        }

        /* The 'changedNode' is either removed or moved within the tree */
        /* The 'replacedNode' is the node that moves into the place of 'changedNode' */
        RBTreeNode<T>* changedNode = targetNode;
        RBTreeNode<T>* replacedNode = RB_SENTINEL;
        RBColor originalColor = targetNode->color;

        /* Target does not have a left child (Case a | b) */
        if (targetNode->left.get() == RB_SENTINEL) {
            replacedNode = targetNode->right.get();
            this->transplant(targetNode, std::move(targetNode->right));
        }
        /* Target does not have a right child (Case a | b) */
        else if (targetNode->right.get() == RB_SENTINEL) {
            replacedNode = targetNode->left.get();
            this->transplant(targetNode, std::move(targetNode->left));
        }
        /* Target does have both of it's children (Case c) */
        else {
            changedNode = this->successor(targetNode);
            originalColor = changedNode->color;

            /* TODO: Left here... as of 26 March 2023 - 10:03 PM */

            /* Case c.1 */
            if (changedNode == targetNode->right.get()) {
                changedNode->left = std::move(targetNode->left);

                 this->transplant(targetNode, std::move(targetNode->right));
            }
            /* Case c.2 */
            else {
                /* Temporarily hold the successor */
                std::unique_ptr<RBTreeNode<T>> temp = std::move(changedNode->parent->left);

                /* Update the successor's parent's left child */
                if (temp->right) {
                    temp->right->parent = temp->parent;
                    temp->parent->left = std::move(temp->right);
                }

                /* Take ownership of the target's children */
                temp->left = std::move(targetNode->left);
                temp->right = std::move(targetNode->right);

                this->transplant(targetNode, std::move(temp));
            }
        }

        if (originalColor == RBColor::BLACK) {
            /* Some smart pointer bs */
            /* TODO: Find a better way to do this */

            auto parent = replacedNode->parent;
            if (parent == RB_SENTINEL) {
                this->remove_fixup(this->m_pRoot);
            }
            else if (replacedNode == parent->left.get()) {
                this->remove_fixup(replacedNode->parent->left);
            }
            else {
                this->remove_fixup(parent->right);
            }
        }

        if (this->m_uSize != 0u) {
            retValue = true;
            this->m_uSize--;
        }

        return retValue;
    }

    template<typename T>
    RBTreeNode<T>* RedBlackTree<T>::search_node(const T& target) const noexcept {
        RBTreeNode<T>* retValue = nullptr;
        RBTreeNode<T>* iter = this->m_pRoot.get();

        /* Search for the target */
        while (iter && target != retValue->key) {
            if (target < retValue->key) {
                iter = iter->left.get();
            } else {
                iter = iter->right.get();
            }
        }

        /* Found the target */
        if (iter) {
            retValue = iter;
        }

        return retValue;
    }

    template<typename T>
    std::optional<T> RedBlackTree<T>::search(const T& target) const noexcept {
        std::optional<T> retValue;
        RBTreeNode<T>* iter = this->m_pRoot.get();

        /* Search for the target */
        while (iter && target != retValue->key) {
            if (target < retValue->key) {
                iter = iter->left.get();
            } else {
                iter = iter->right.get();
            }
        }

        /* Found the target */
        if (iter) {
            retValue = iter->key;
        }

        return retValue;
    }

    template<typename T>
    RBTreeNode<T>* RedBlackTree<T>::successor(const RBTreeNode<T>* target) const noexcept {
        RBTreeNode<T>* retValue = nullptr;

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
    RBTreeNode<T>* RedBlackTree<T>::predecessor(const RBTreeNode<T>* target) const noexcept {
        RBTreeNode<T>* retValue = nullptr;

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
    RBTreeNode<T>* RedBlackTree<T>::min(const RBTreeNode<T>* target) const noexcept {
        RBTreeNode<T>* retValue = this->m_pRoot.get();

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
    RBTreeNode<T>* RedBlackTree<T>::max(const RBTreeNode<T>* target) const noexcept {
        RBTreeNode<T>* retValue = this->m_pRoot.get();

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
    std::size_t RedBlackTree<T>::size() const noexcept {
        return this->m_uSize;
    }

    template<typename T>
    std::size_t RedBlackTree<T>::height_helper(const RBTreeNode<T>* target) const noexcept {
        std::size_t retValue = 0u;

        if (target != RB_SENTINEL) {
            retValue = 1u + std::max(
                    this->height_helper(target->left.get()),
                    this->height_helper(target->right.get())
            ) ;
        }

        return retValue;
    }
    template<typename T>
    std::size_t RedBlackTree<T>::height(const RBTreeNode<T>* target) const noexcept {
        std::size_t retValue = 0u;

        /* If target is not specified, then start from the root */
        if (target == nullptr) {
            target = this->m_pRoot.get();
        }

        retValue = this->height_helper(target);

        return retValue;
    }

    template<typename T>
    void RedBlackTree<T>::inorder_print(std::ostream& os, const RBTreeNode<T>* curr_node) const {
        if (curr_node == RB_SENTINEL) {
            os << "_";
        }
        else {
            os << "[";
            this->inorder_print(os, curr_node->left.get());
            if (curr_node->color == RBColor::RED) {
                os << " " << curr_node->key << "(R) ";
            }
            else {
                os << " " << curr_node->key << "(B) ";
            }
            this->inorder_print(os, curr_node->right.get());
            os << "]";
        }
    }

    template<typename T>
    void RedBlackTree<T>::preorder_print(std::ostream& os, const RBTreeNode<T>* curr_node) const {
        if (curr_node == RB_SENTINEL) {
            os << "_";
        }
        else {
            if (curr_node->color == RBColor::RED) {
                os << "[" << curr_node->key << "(R) ";
            }
            else {
                os << "[" << curr_node->key << "(B) ";
            }
            this->preorder_print(os, curr_node->left.get());
            os << " ";
            this->preorder_print(os, curr_node->right.get());
            os << "]";
        }
    }

    template<typename U>
    std::ostream& operator<<(std::ostream& os, const RedBlackTree<U>& obj) {
        obj.preorder_print(os, obj.m_pRoot.get());

        return os;
    }
}