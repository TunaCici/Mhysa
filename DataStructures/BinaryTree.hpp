//
// Created by Tuna Cici on 20.03.2023.
//

#ifndef MHYSA_BINARYTREE_HPP
#define MHYSA_BINARYTREE_HPP

#include <ostream>

namespace data_struct {
    template<typename U>
    struct BTreeNode{
        U key;
        BTreeNode<U>* parent;
        std::unique_ptr<BTreeNode<U>> left;
        std::unique_ptr<BTreeNode<U>> right;
    };

    template<typename T>
    class BinaryTree {
    private:
        std::unique_ptr<BTreeNode<T>> m_pRoot{};
        std::size_t m_uSize{};

        bool create_node(const T& input, std::unique_ptr<BTreeNode<T>>& output);
        bool transplant(BTreeNode<T>* target, std::unique_ptr<BTreeNode<T>> subtree);

        std::size_t height_helper(const BTreeNode<T>* target);
        void inorder_print(std::ostream& os, const BTreeNode<T>* curr_node) const;
        void preorder_print( std::ostream& os, const BTreeNode<T>* curr_node) const;
    public:
        BinaryTree();
        ~BinaryTree();

        BinaryTree(BinaryTree& other) = delete;
        BinaryTree& operator=(const BinaryTree& other) = delete;

        bool insert(const T& input);
        bool remove(const T& target);

        BTreeNode<T>* search(const T& target);
        BTreeNode<T>* successor(const BTreeNode<T>* target);
        BTreeNode<T>* predecessor(const BTreeNode<T>* target);

        BTreeNode<T>* min(const BTreeNode<T>* target = nullptr);
        BTreeNode<T>* max(const BTreeNode<T>* target = nullptr);

        std::size_t size();
        std::size_t height(const BTreeNode<T>* target = nullptr);

        template<typename U>
        friend std::ostream& operator<<(std::ostream& os, const BinaryTree<U>& obj);
    };
}

#include "DataStructures/BinaryTree.tpp"

#endif
