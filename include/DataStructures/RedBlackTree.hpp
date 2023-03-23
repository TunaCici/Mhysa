//
// Created by Tuna Cici on 23.03.2023.
//

#ifndef MHYSA_REDBLACKTREE_HPP
#define MHYSA_REDBLACKTREE_HPP

#include <memory>

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
        bool transplant(RBTreeNode<T>* target, std::unique_ptr<RBTreeNode<T>> subtree);
        bool insert_fixup(RBTreeNode<T>* target);

        bool rotate_left(std::unique_ptr<RBTreeNode<T>> target);
        bool rotate_right(std::unique_ptr<RBTreeNode<T>> target);

        std::size_t height_helper(const RBTreeNode<T>* target);
        void inorder_print(std::ostream& os, const RBTreeNode<T>* curr_node) const;
        void preorder_print( std::ostream& os, const RBTreeNode<T>* curr_node) const;
    public:
        RedBlackTree();
        ~RedBlackTree();

        RedBlackTree(RedBlackTree& other) = delete;
        RedBlackTree& operator=(const RedBlackTree& other) = delete;

        bool insert(const T& input);
        bool remove(const T& target);

        RBTreeNode<T>* search(const T& target);
        RBTreeNode<T>* successor(const RBTreeNode<T>* target);
        RBTreeNode<T>* predecessor(const RBTreeNode<T>* target);

        RBTreeNode<T>* min(const RBTreeNode<T>* target = nullptr);
        RBTreeNode<T>* max(const RBTreeNode<T>* target = nullptr);

        std::size_t size();
        std::size_t height(const RBTreeNode<T>* target = nullptr);

        template<typename U>
        friend std::ostream& operator<<(std::ostream& os, const RedBlackTree<U>& obj);
    };
}

#include "DataStructures/RedBlackTree.tpp"

#endif
