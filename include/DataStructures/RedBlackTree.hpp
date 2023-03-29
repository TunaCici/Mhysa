//
// Created by Tuna Cici on 23.03.2023.
//

#ifndef MHYSA_REDBLACKTREE_HPP
#define MHYSA_REDBLACKTREE_HPP

#include <memory>
#include <optional>

namespace data_struct {
    enum class RBColor {RED, BLACK};

    template<typename U>
    struct RBTreeNode{
        U key;
        RBColor color;

        std::unique_ptr<RBTreeNode<U>> left;
        std::unique_ptr<RBTreeNode<U>> right;
    };

    template<typename T>
    class RedBlackTree {
    private:
        std::unique_ptr<RBTreeNode<T>> m_pRoot{};
        std::size_t m_uSize{};

        std::unique_ptr<RBTreeNode<T>> create_node(const T& input);

        std::unique_ptr<RBTreeNode<T>> rotate_left(std::unique_ptr<RBTreeNode<T>> target);
        std::unique_ptr<RBTreeNode<T>> rotate_right(std::unique_ptr<RBTreeNode<T>> target);
        void flip_colors(std::unique_ptr<RBTreeNode<T>>& target);
        std::unique_ptr<RBTreeNode<T>> move_red_left(std::unique_ptr<RBTreeNode<T>> target);
        std::unique_ptr<RBTreeNode<T>> move_red_right(std::unique_ptr<RBTreeNode<T>> target);

        bool is_red(const RBTreeNode<T>* target) const noexcept;
        std::unique_ptr<RBTreeNode<T>> fix_up(std::unique_ptr<RBTreeNode<T>> target);

        std::unique_ptr<RBTreeNode<T>> insert_helper(std::unique_ptr<RBTreeNode<T>> curr_node, const T& input);
        std::unique_ptr<RBTreeNode<T>> remove_helper(std::unique_ptr<RBTreeNode<T>> curr_node, const T& target);
        std::unique_ptr<RBTreeNode<T>> remove_min(std::unique_ptr<RBTreeNode<T>> curr_node);

        RBTreeNode<T>* min(RBTreeNode<T>* curr_node) const noexcept;
        RBTreeNode<T>* max(RBTreeNode<T>* curr_node) const noexcept;

        std::size_t height_helper(const RBTreeNode<T>* target) const noexcept;
        void inorder_print(std::ostream& os, const RBTreeNode<T>* curr_node) const;
        void preorder_print( std::ostream& os, const RBTreeNode<T>* curr_node) const;
    public:
        RedBlackTree();
        ~RedBlackTree();

        RedBlackTree(RedBlackTree& other) = delete;
        RedBlackTree& operator=(const RedBlackTree& other) = delete;

        bool insert(const T& input);
        bool remove(const T& target);
        bool remove_min();
        std::optional<T> search(const T& target) const noexcept;

        std::size_t size() const noexcept;
        std::size_t height(const RBTreeNode<T>* target = nullptr) const noexcept;

        template<typename U>
        friend std::ostream& operator<<(std::ostream& os, const RedBlackTree<U>& obj);
    };
}

#include "DataStructures/RedBlackTree.tpp"

#endif
