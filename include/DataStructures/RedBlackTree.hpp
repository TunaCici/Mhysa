//
// Created by Tuna Cici on 23.03.2023.
//

#ifndef MHYSA_REDBLACKTREE_HPP
#define MHYSA_REDBLACKTREE_HPP

#include <memory>
#include <optional>

/* Virtual RBTree sentinel 'node' */
/* Note: Normally sentinel nodes are actual nodes (such as RBTreeNode) */
/* Note: But since my  left and right pointers are std::unique_ptr, I can't use a real sentinel node */
/* Note: Doing so requires me to create many sentinel nodes for each new node (thus wasting memoryß) */
/* Note: Instead, I can use a C++ macro to mimic a sentinel node */
#define RB_SENTINEL nullptr

namespace data_struct {
    enum class RBColor {RED, BLACK};

    template<typename U>
    struct RBTreeNode{
        U key;
        RBColor color;

        RBTreeNode<U>* parent;
        std::unique_ptr<RBTreeNode<U>> left;
        std::unique_ptr<RBTreeNode<U>> right;
    };

    template<typename T>
    class RedBlackTree {
    private:
        std::unique_ptr<RBTreeNode<T>> m_pRoot{};
        std::size_t m_uSize{};

        bool create_node(const T& input, std::unique_ptr<RBTreeNode<T>>& output);
        void transplant(RBTreeNode<T>* target, std::unique_ptr<RBTreeNode<T>> subtree);

        void insert_fixup(std::unique_ptr<RBTreeNode<T>>& target);
        void remove_fixup(std::unique_ptr<RBTreeNode<T>>& target);
        void rotate_left(std::unique_ptr<RBTreeNode<T>>& target);
        void rotate_right(std::unique_ptr<RBTreeNode<T>>& target);

        RBTreeNode<T>* search_node(const T& target) const noexcept;
        RBTreeNode<T>* successor(const RBTreeNode<T>* target = nullptr) const noexcept;
        RBTreeNode<T>* predecessor(const RBTreeNode<T>* target = nullptr) const noexcept;

        RBTreeNode<T>* min(const RBTreeNode<T>* target = nullptr) const noexcept;
        RBTreeNode<T>* max(const RBTreeNode<T>* target = nullptr) const noexcept;

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
        std::optional<T> search(const T& target) const noexcept;

        std::size_t size() const noexcept;
        std::size_t height(const RBTreeNode<T>* target = nullptr) const noexcept;

        template<typename U>
        friend std::ostream& operator<<(std::ostream& os, const RedBlackTree<U>& obj);
    };
}

#include "DataStructures/RedBlackTree.tpp"

#endif
