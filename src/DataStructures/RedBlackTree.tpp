//
// Created by Tuna Cici on 23.03.2023.
//

#include "DataStructures/RedBlackTree.hpp"

namespace data_struct {
    template<typename T>
    RedBlackTree<T>::RedBlackTree() {
        this->m_pRoot = nullptr;

        this->m_uSize = 0u;
    }

    template<typename T>
    RedBlackTree<T>::~RedBlackTree() {
        /* std::unique_ptr releases data automatically */
        /* TODO: Possible recursion/stack-over-flow!! Manually release them */
        this->m_pRoot = nullptr;
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
    bool RedBlackTree<T>::transplant(RBTreeNode<T>* target, std::unique_ptr<RBTreeNode<T>> subtree) {

    }

    template<typename T>
    bool RedBlackTree<T>::insert_fixup(RBTreeNode<T>* target) {
        bool retValue = false;

        /* Target is nullptr (oops!) */
        if (target == nullptr) {
            return retValue;
        }

        /* We have 4 different cases */
        /* Case 0. target is m_pRoot: Just color it 'BLACK'  */
        /* Case 1. target's uncle (rawPtrUncle) is 'RED':  Apply recoloring */
        /* Case 2. target's uncle (rawPtrUncle) is 'BLACK' and target is 'right': Apply rotate_left() */
        /* Case 3. target's uncle (rawPtrUncle) is 'BLACK' and target is 'left': Apply rotate_right() */

        RBTreeNode<T>* rawPtrParent = RB_SENTINEL;
        RBTreeNode<T>* rawPtrGrandParent = RB_SENTINEL;
        RBTreeNode<T>* rawPtrUncle = RB_SENTINEL;

        /* Initial raw C pointers */
        rawPtrParent = target->parent;
        rawPtrGrandParent = rawPtrParent ? rawPtrParent->parent : RB_SENTINEL;

        while (rawPtrParent && rawPtrParent->color == RBColor::RED) {
            /* Parent is left child */
            if (rawPtrGrandParent && rawPtrParent == rawPtrGrandParent->left.get()) {
                rawPtrUncle = rawPtrGrandParent->right.get();

                /* Case 1. */
                if (rawPtrUncle && rawPtrUncle->color == RBColor::RED) {
                    rawPtrParent->color = RBColor::BLACK;
                    rawPtrUncle->color = RBColor::BLACK;
                    rawPtrGrandParent = RBColor::RED;

                    target = rawPtrGrandParent;
                }
                /* Case 2. */
                else if (target == rawPtrParent->right.get()) {
                    target = rawPtrParent;

                    this->rotate_left(target);
                }
                /* Case 3. */
                else if (target == rawPtrParent->left.get()) {
                    rawPtrParent->color = RBColor::BLACK;
                    rawPtrGrandParent->color = RBColor::RED;

                    this->rotate_right(rawPtrGrandParent);
                }
                /* You f'd up! */
                else {

                }

            }
            /* Parent is right child */
            else if (rawPtrGrandParent && rawPtrParent == rawPtrGrandParent->right.get()) {
                rawPtrUncle = rawPtrGrandParent->left.get();


            }
            /* Parent is the root */
            else {
                break;
            }

            /* Update raw C pointer */
            rawPtrParent = target->parent;
            rawPtrGrandParent = rawPtrParent ? rawPtrParent->parent : RB_SENTINEL;
        }

        /* Update 'm_pRoot' color */
        this->m_pRoot->color = RBColor::BLACK;

        return retValue;
    }

    template<typename T>
    bool RedBlackTree<T>::rotate_left(std::unique_ptr<RBTreeNode<T>> target) {

    }

    template<typename T>
    bool RedBlackTree<T>::rotate_right(std::unique_ptr<RBTreeNode<T>> target) {

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
        RBTreeNode<T>* rawPtrNewNode = newNode.get();

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

        /* Maintain the red-black tree properties */
        retValue &= this->insert_fixup(rawPtrNewNode);

        return retValue;
    }


    template<typename T>
    bool RedBlackTree<T>::remove(const T& target) {
        return false;
    }

    template<typename T>
    RBTreeNode<T>* RedBlackTree<T>::search(const T& target) {
        return nullptr;
    }

    template<typename T>
    RBTreeNode<T>* RedBlackTree<T>::successor(const RBTreeNode<T>* target) {
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
    RBTreeNode<T>* RedBlackTree<T>::predecessor(const RBTreeNode<T>* target) {
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
    RBTreeNode<T>* RedBlackTree<T>::min(const RBTreeNode<T>* target) {
        RBTreeNode<T>* retValue = this->m_pRoot.get();

        /* Start searching from the 'target' node */
        /* If 'target' is RB_SENTINEL, start searching from the root */
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
    RBTreeNode<T>* RedBlackTree<T>::max(const RBTreeNode<T>* target) {
        RBTreeNode<T>* retValue = this->m_pRoot.get();

        /* Start searching from the 'target' node */
        /* If 'target' is RB_SENTINEL, start searching from the root */
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
    std::size_t RedBlackTree<T>::size() {
        return this->m_uSize;
    }

    template<typename T>
    std::size_t RedBlackTree<T>::height_helper(const RBTreeNode<T>* target) {
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
    std::size_t RedBlackTree<T>::height(const RBTreeNode<T>* target) {
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