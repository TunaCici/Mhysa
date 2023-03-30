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
    std::unique_ptr<RBTreeNode<T>> RedBlackTree<T>::create_node(const T& input) {
        std::unique_ptr<RBTreeNode<T>> output{};

        /* Create a new node */
        try {
            output = std::make_unique<RBTreeNode<T>>();

            output->key = input;
            output->color = RBColor::RED;

            output->left = {};
            output->right = {};

            this->m_uSize++;
        }
        catch (const std::exception& e) {
            std::cerr << "Failed to create a new node! " << e.what() << std::endl;
            throw;
        }

        return output;
    }

    template<typename T>
    std::unique_ptr<RBTreeNode<T>> RedBlackTree<T>::rotate_left(std::unique_ptr<RBTreeNode<T>> target) {
        /* Right child does not exist */
        if (!target->right) {
            throw std::runtime_error("Attempted to rotate left with a node that has no right child!");
        }

        /* Hold the right child */
        auto rightChild = std::move(target->right);

        target->right = std::move(rightChild->left);
        rightChild->left = std::move(target);

        rightChild->color = rightChild->left->color;
        rightChild->left->color = RBColor::RED;

        return std::move(rightChild);
    }

    template<typename T>
    std::unique_ptr<RBTreeNode<T>> RedBlackTree<T>::rotate_right(std::unique_ptr<RBTreeNode<T>> target) {
        /* Left child does not exist */
        if (!target->left) {
            throw std::runtime_error("Attempted to rotate right with a node that has no left child!");
        }

        /* Hold the left child */
        auto leftChild = std::move(target->left);

        target->left = std::move(leftChild->right);
        leftChild->right = std::move(target);

        leftChild->color = leftChild->right->color;
        leftChild->right->color = RBColor::RED;

        return std::move(leftChild);
    }

    template<typename T>
    void RedBlackTree<T>::flip_colors(std::unique_ptr<RBTreeNode<T>>& target) {
        /* Target */
        if (target->color == RBColor::RED) {
            target->color = RBColor::BLACK;
        }
        else {
            target->color = RBColor::RED;
        }

        /* Left child */
        if (target->left) {
            if (target->left->color == RBColor::RED) {
                target->left->color = RBColor::BLACK;
            }
            else {
                target->left->color = RBColor::RED;
            }
        }

        /* Right child */
        if (target->right) {
            if (target->right->color == RBColor::RED) {
                target->right->color = RBColor::BLACK;
            }
            else {
                target->right->color = RBColor::RED;
            }
        }
    }

    template<typename T>
    std::unique_ptr<RBTreeNode<T>> RedBlackTree<T>::move_red_left(std::unique_ptr<RBTreeNode<T>> target) {
        this->flip_colors(target);

        if (target->right and this->is_red(target->right->left.get())) {
            target->right = this->rotate_right(std::move(target->right));
            target = this->rotate_left(std::move(target));
            this->flip_colors(target);
        }

        return std::move(target);
    }

    template<typename T>
    std::unique_ptr<RBTreeNode<T>> RedBlackTree<T>::move_red_right(std::unique_ptr<RBTreeNode<T>> target) {
        this->flip_colors(target);

        if (target->left and this->is_red(target->left->left.get())) {
            target = this->rotate_right(std::move(target));
            this->flip_colors(target);
        }

        return std::move(target);
    }

    template<typename T>
    bool RedBlackTree<T>::is_red(const RBTreeNode<T>* target) const noexcept {
        if (target) {
            return (target->color == RBColor::RED);
        }

        return false;
    }

    template<typename T>
    std::unique_ptr<RBTreeNode<T>> RedBlackTree<T>::fix_up(std::unique_ptr<RBTreeNode<T>> target) {
        if (this->is_red(target->right.get()) and !this->is_red(target->left.get())) {
            target = this->rotate_left(std::move(target));
        }

        if (this->is_red(target->left.get()) and this->is_red(target->left->left.get())) {
            target = this->rotate_right(std::move(target));
        }

        if(this->is_red(target->left.get()) and this->is_red(target->right.get())) {
            this->flip_colors(target);
        }

        return std::move(target);
    }

    template<typename T>
    std::unique_ptr<RBTreeNode<T>> RedBlackTree<T>::insert_helper(std::unique_ptr<RBTreeNode<T>> curr_node, const T& input) {
        /* Create a new node */
        if (!curr_node) {
            return this->create_node(input);
        }

        /* Insert the node */
        if (input == curr_node->key) {
            curr_node->key = input;
        }
        else if (input < curr_node->key) {
            curr_node->left = this->insert_helper(std::move(curr_node->left), input);
        }
        else {
            curr_node->right = this->insert_helper(std::move(curr_node->right), input);
        }

        /* Balance the tree using 'fix_up()' */
        curr_node = this->fix_up(std::move(curr_node));

        return std::move(curr_node);
    }

    template<typename T>
    bool RedBlackTree<T>::insert(const T& input) {
        bool retValue = false;

        auto previousSize = this->m_uSize;

        /* Recursively insert the node */
        this->m_pRoot = this->insert_helper(std::move(this->m_pRoot), input);
        this->m_pRoot->color = RBColor::BLACK;

        auto newSize = this->m_uSize;

        if (newSize > previousSize) {
            retValue = true;
        }

        return retValue;
    }

    template<typename T>
    std::unique_ptr<RBTreeNode<T>> RedBlackTree<T>::remove_min(std::unique_ptr<RBTreeNode<T>> curr_node) {
        if (!curr_node->left) {
            this->m_uSize--;

            return nullptr;
        }

        if (!this->is_red(curr_node->left.get()) and !this->is_red(curr_node->left->left.get())) {
            curr_node = this->move_red_left(std::move(curr_node));
        }

        curr_node->left = this->remove_min(std::move(curr_node->left));

        return this->fix_up(std::move(curr_node));
    }

    template<typename T>
    std::unique_ptr<RBTreeNode<T>> RedBlackTree<T>::remove_helper(std::unique_ptr<RBTreeNode<T>> curr_node, const T& target) {
        /* Current node is 'nullptr' */
        if (!curr_node) {
            if (0u < this->m_uSize) {
                this->m_uSize--;
            }

            return nullptr;
        }

        /* Target is less than the current node -> go left */
        if (target < curr_node->key) {
            if(!this->is_red(curr_node->left.get())) {
                /* Do not forget to if the curr_node->left is nullptr */
                if (curr_node->left and !this->is_red(curr_node->left->left.get())) {
                    curr_node = this->move_red_left(std::move(curr_node));
                }
            }

            curr_node->left = this->remove_helper(std::move(curr_node->left), target);
        }
        else {
            if (this->is_red(curr_node->left.get())) {
                curr_node = this->rotate_right(std::move(curr_node));
            }

            if (target == curr_node->key and !curr_node->right) {
                this->m_uSize--;

                return nullptr;
            }

            if (!this->is_red(curr_node->right.get())) {
                /* Do not forget to if the curr_node->right is nullptr */
                if (curr_node->right and !this->is_red(curr_node->right->left.get())) {
                    curr_node = this->move_red_right(std::move(curr_node));
                }
            }

            if (target == curr_node->key) {
                auto min = this->min(curr_node->right.get());
                curr_node->key = min->key;
                curr_node->right = this->remove_min(std::move(curr_node->right));
            }
            else {
                curr_node->right = this->remove_helper(std::move(curr_node->right), target);
            }

        }

        return std::move(curr_node);
    }

    template<typename T>
    bool RedBlackTree<T>::remove_min() {
        bool retValue = false;

        auto previousSize = this->m_uSize;

        this->m_pRoot = this->remove_min(std::move(this->m_pRoot));
        if (this->m_pRoot) {
            this->m_pRoot->color = RBColor::BLACK;
        }

        auto newSize = this->m_uSize;

        if (newSize < previousSize) {
            retValue = true;
        }

        return retValue;
    }

    template<typename T>
    bool RedBlackTree<T>::remove(const T& target) {
        bool retValue = false;

        auto previousSize = this->m_uSize;

        this->m_pRoot = this->remove_helper(std::move(this->m_pRoot), target);
        if (this->m_pRoot) {
            this->m_pRoot->color = RBColor::BLACK;
        }

        auto newSize = this->m_uSize;

        if (newSize < previousSize) {
            retValue = true;
        }

        return retValue;
    }

    template<typename T>
    RBTreeNode<T>* RedBlackTree<T>::min(RBTreeNode<T>* curr_node) const noexcept {
        RBTreeNode<T>* retValue = nullptr;

        while (curr_node) {
            retValue = curr_node;
            curr_node = curr_node->left.get();
        }

        return retValue;
    }

    template<typename T>
    RBTreeNode<T>* RedBlackTree<T>::max(RBTreeNode<T>* curr_node) const noexcept {
        RBTreeNode<T>* retValue = nullptr;

        while (curr_node) {
            retValue = curr_node;
            curr_node = curr_node->right.get();
        }

        return retValue;
    }

    template<typename T>
    const T* RedBlackTree<T>::search(const T& target) const noexcept {
        const T* retValue = nullptr;
        RBTreeNode<T>* iter = this->m_pRoot.get();

        /* Search for the target */
        while (iter && target != iter->key) {
            if (target < iter->key) {
                iter = iter->left.get();
            } else {
                iter = iter->right.get();
            }
        }

        /* Found the target */
        if (iter) {
            retValue = &iter->key;
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

        if (target) {
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
        if (!curr_node) {
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
        if (!curr_node) {
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
        const auto maxAllowedSize = 32u;

        if (obj.m_uSize <= maxAllowedSize) {
            obj.preorder_print(os, obj.m_pRoot.get());
        }
        else {
            os << "Tree is too large to print" << std::endl;
        }

        return os;
    }
}