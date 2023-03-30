//
// Created by Tuna Cici on 23.03.2023.
//

//
// Red Black Tree Implementation in C++20 using std::unique_ptr
//
// Based on the Robert Sedgewick's Left Leaning Red Black Tree (A.K.A. LLRB Tree)
// URL: https://sedgewick.io/wp-content/themes/sedgewick/papers/2008LLRB.pdf
//

#ifndef MHYSA_REDBLACKTREE_HPP
#define MHYSA_REDBLACKTREE_HPP

#include <memory>

namespace data_struct {
    enum class RBColor {RED, BLACK};

    template<typename U>
    struct RBTreeNode{
        U key;
        RBColor color;

        std::unique_ptr<RBTreeNode<U>> left;
        std::unique_ptr<RBTreeNode<U>> right;
    };

    /**
     * @brief Left Leaning Red Black Tree Implementatio
     *
     * @tparam T Type of the key to be used in the tree
     */
    template<typename T>
    class RedBlackTree {
    private:
        std::unique_ptr<RBTreeNode<T>> m_pRoot{};
        std::size_t m_uSize{};

        std::unique_ptr<RBTreeNode<T>> create_node(const T& input);

        /* Helper functions used when balancing the tree */
        bool is_red(const RBTreeNode<T>* target) const noexcept;
        void flip_colors(std::unique_ptr<RBTreeNode<T>>& target);
        std::unique_ptr<RBTreeNode<T>> rotate_left(std::unique_ptr<RBTreeNode<T>> target) ;
        std::unique_ptr<RBTreeNode<T>> rotate_right(std::unique_ptr<RBTreeNode<T>> target);
        std::unique_ptr<RBTreeNode<T>> move_red_left(std::unique_ptr<RBTreeNode<T>> target);
        std::unique_ptr<RBTreeNode<T>> move_red_right(std::unique_ptr<RBTreeNode<T>> target);
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

        /**
         * @brief Inserts a new key into the LLRB tree
         * @details If the key already exists, the insertion fails.
         *          Time Complexity (Average): O(log n)
         *
         * @param input The key to be inserted
         *
         * @return true if the insertion was successful
         * @return false if the insertion failed
         * */
        bool insert(const T& input);

        /**
         * @brief Removes a node from the LLRB tree with the given key
         * @details If the key does not exist, the removal fails
         *          Time Complexity (Average): O(log n)
         *
         * @param target The key to be removed
         *
         * @return true if the removal was successful
         * @return false if the removal failed
         * */
        bool remove(const T& target);

        /**
         * @brief Searches for a key in the LLRB tree
         * @details Time Complexity (Average): O(log n)
         *
         * @param target The key to be searched
         *
         * @return const T* A pointer to the key/value if it is found
         * @return nullptr if the key was not found
         * */
        const T* search(const T& target) const noexcept;

        /**
         * @brief Returns the size of the LLRB tree
         *
         * @return std::size_t The size of the LLRB tree
         * */
        std::size_t size() const noexcept;

        /**
         * @brief Returns the height of the LLRB tree
         * @details Counting starts from 1.
         *          Empty tree has height 0, One node tree has height 1.
         *          Time Complexity: O(n)
         *
         * @return std::size_t The height of the LLRB tree
         * */
        std::size_t height(const RBTreeNode<T>* target = nullptr) const noexcept;

        template<typename U>
        friend std::ostream& operator<<(std::ostream& os, const RedBlackTree<U>& obj);
    };
}

#include "DataStructures/RedBlackTree.tpp"

#endif
