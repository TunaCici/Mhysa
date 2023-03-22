//
// Created by Tuna Cici on 21.03.2023.
//

#include <random>
#include <vector>
#include <ctime>
#include <limits>

#include "gtest/gtest.h"

#include "DataStructures/BinaryTree.hpp"

/* Random int generator mt19937 */
static std::mt19937 mt_machine(time(nullptr));
static std::uniform_int_distribution uni_dist(
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max()
);

TEST(BinaryTree, Initialization) {
    std::unique_ptr<data_struct::BinaryTree<int>> myBinaryTree;
    myBinaryTree = std::make_unique<data_struct::BinaryTree<int>>();

    EXPECT_EQ(myBinaryTree->size(), 0u);
    EXPECT_EQ(myBinaryTree->height(), 0u);
}

TEST(BinaryTree, Insert) {
    std::unique_ptr<data_struct::BinaryTree<int>> myBinaryTree;
    myBinaryTree = std::make_unique<data_struct::BinaryTree<int>>();

    const unsigned int vectorSize = 64u;
    std::vector<int> vecRandomIntegers = {};
    for (long i = 0; i < vectorSize; i++) { vecRandomIntegers.push_back(uni_dist(mt_machine));}

    /* Insert 64 random values */
    for (std::size_t i = 0; i < vectorSize; i++) {
        EXPECT_EQ(myBinaryTree->insert(vecRandomIntegers[i]), true);
        EXPECT_EQ(myBinaryTree->size(), i + 1);
    }
}

TEST(BinaryTree, Height) {
    std::unique_ptr<data_struct::BinaryTree<int>> myBinaryTree;
    myBinaryTree = std::make_unique<data_struct::BinaryTree<int>>();

    /* Pre-determined values */
    std::vector<int> someValues = {8, 4, 12, 2, 6, 10, 14, 1, 3, 5, 7, 9, 15};
    for (int someValue : someValues) {
        EXPECT_EQ(myBinaryTree->insert(someValue), true);
        if (someValue == 8) {
            EXPECT_EQ(myBinaryTree->height(), 1u);
        }
        else if (someValue == 4 || someValue == 12) {
            EXPECT_EQ(myBinaryTree->height(), 2u);
        }
        else if (someValue == 2 || someValue == 6 || someValue == 10 || someValue == 14) {
            EXPECT_EQ(myBinaryTree->height(), 3u);
        }
        else if (someValue == 1 || someValue == 3 || someValue == 5 || someValue == 7 || someValue == 9 || someValue == 15) {
            EXPECT_EQ(myBinaryTree->height(), 4u);
        }
    }
}

TEST(BinaryTree, Search) {
    std::unique_ptr<data_struct::BinaryTree<int>> myBinaryTree;
    myBinaryTree = std::make_unique<data_struct::BinaryTree<int>>();

    /* Search an empty tree */
    EXPECT_EQ(myBinaryTree->search(47), nullptr);

    data_struct::BTreeNode<int>* out = nullptr;

    /* Pre-determined values */
    std::vector<int> someValues = {8, 4, 12, 2, 6, 10, 14, 1, 3, 5, 7, 9, 15};
    for (int someValue : someValues) {
        EXPECT_EQ(myBinaryTree->insert(someValue), true);
    }

    /* Search for a non-existent value */
    EXPECT_EQ(myBinaryTree->search(47), nullptr);

    /* Search for a value located at the root */
    out = myBinaryTree->search(8);
    EXPECT_NE(out, nullptr);
    EXPECT_EQ(out->key, 8);

    /* Search for a value located at a subtree */
    out = myBinaryTree->search(4);
    EXPECT_NE(out, nullptr);
    EXPECT_EQ(out->key, 4);

    /* Search for a value located at a leaf */
    out = myBinaryTree->search(15);
    EXPECT_NE(out, nullptr);
    EXPECT_EQ(out->key, 15);

    const unsigned int vectorSize = 64u;
    std::vector<int> vecRandomIntegers = {};
    for (long i = 0; i < vectorSize; i++) { vecRandomIntegers.push_back(uni_dist(mt_machine));}

    /* Insert 64 random values */
    for (std::size_t i = 0; i < vectorSize; i++) {
        EXPECT_EQ(myBinaryTree->insert(vecRandomIntegers[i]), true);
    }

    /* Search for all values */
    for (std::size_t i = 0; i < vectorSize; i++) {
        out = myBinaryTree->search(vecRandomIntegers[i]);
        EXPECT_NE(out, nullptr);
        EXPECT_EQ(out->key, vecRandomIntegers[i]);
    }
}

TEST(BinaryTree, Remove) {
    std::unique_ptr<data_struct::BinaryTree<int>> myBinaryTree;
    myBinaryTree = std::make_unique<data_struct::BinaryTree<int>>();

    const unsigned int vectorSize = 64u;
    std::vector<int> vecRandomIntegers = {};
    for (long i = 0; i < vectorSize; i++) { vecRandomIntegers.push_back(uni_dist(mt_machine));}

    /* Insert 64 random values */
    for (std::size_t i = 0; i < vectorSize; i++) {
        EXPECT_EQ(myBinaryTree->insert(vecRandomIntegers[i]), true);
    }

    /* Search for all values */
    data_struct::BTreeNode<int>* out = nullptr;
    for (std::size_t i = 0; i < vectorSize; i++) {
        out = myBinaryTree->search(vecRandomIntegers[i]);
        EXPECT_NE(out, nullptr);
        EXPECT_EQ(out->key, vecRandomIntegers[i]);
    }

    /* Remove 64 random values */
    for (std::size_t i = 0; i < vectorSize; i++) {
        EXPECT_EQ(myBinaryTree->remove(vecRandomIntegers[i]), true);
        EXPECT_EQ(myBinaryTree->size(), vectorSize - (i + 1));
    }
}

TEST(BinaryTree, Successor) {
    std::unique_ptr<data_struct::BinaryTree<int>> myBinaryTree;
    myBinaryTree = std::make_unique<data_struct::BinaryTree<int>>();

    /* Pre-determined values */
    std::vector<int> someValues = {8, 4, 12, 2, 6, 10, 14, 1, 3, 5, 7, 9, 15};
    for (int someValue : someValues) {
        EXPECT_EQ(myBinaryTree->insert(someValue), true);
    }

    data_struct::BTreeNode<int>* successorNode = nullptr;
    data_struct::BTreeNode<int>* searchNode = nullptr;

    /* Search for the successor of the root */
    searchNode = myBinaryTree->search(8);
    successorNode = myBinaryTree->successor(searchNode);
    EXPECT_NE(successorNode, nullptr);
    EXPECT_EQ(successorNode->key, 9);

    /* Search for the successor of 15 */
    searchNode = myBinaryTree->search(15);
    successorNode = myBinaryTree->successor(searchNode);
    EXPECT_EQ(successorNode, nullptr);

    /* Search for the successor of 1 */
    searchNode = myBinaryTree->search(1);
    successorNode = myBinaryTree->successor(searchNode);
    EXPECT_NE(successorNode, nullptr);
    EXPECT_EQ(successorNode->key, 2);
}

TEST(BinaryTree, Predecessor) {
    std::unique_ptr<data_struct::BinaryTree<int>> myBinaryTree;
    myBinaryTree = std::make_unique<data_struct::BinaryTree<int>>();

    /* Pre-determined values */
    std::vector<int> someValues = {8, 4, 12, 2, 6, 10, 14, 1, 3, 5, 7, 9, 15};
    for (int someValue : someValues) {
        EXPECT_EQ(myBinaryTree->insert(someValue), true);
    }

    data_struct::BTreeNode<int>* predecessorNode = nullptr;
    data_struct::BTreeNode<int>* searchNode = nullptr;

    /* Search for the predecessor of the root */
    searchNode = myBinaryTree->search(8);
    predecessorNode = myBinaryTree->predecessor(searchNode);
    EXPECT_NE(predecessorNode, nullptr);
    EXPECT_EQ(predecessorNode->key, 7);

    /* Search for the predecessor of 1 */
    searchNode = myBinaryTree->search(1);
    predecessorNode = myBinaryTree->predecessor(searchNode);
    EXPECT_EQ(predecessorNode, nullptr);

    /* Search for the predecessor of 15 */
    searchNode = myBinaryTree->search(15);
    predecessorNode = myBinaryTree->predecessor(searchNode);
    EXPECT_NE(predecessorNode, nullptr);
    EXPECT_EQ(predecessorNode->key, 14);
}

TEST(BinaryTree, Min) {
    std::unique_ptr<data_struct::BinaryTree<int>> myBinaryTree;
    myBinaryTree = std::make_unique<data_struct::BinaryTree<int>>();

    /* Check min value in an empty tree */
    data_struct::BTreeNode<int>* minNode = myBinaryTree->min();
    EXPECT_EQ(minNode, nullptr);

    const unsigned int vectorSize = 64u;
    std::vector<int> vecRandomIntegers = {};
    for (long i = 0; i < vectorSize; i++) { vecRandomIntegers.push_back(uni_dist(mt_machine));}

    /* Insert 64 random values */
    for (std::size_t i = 0; i < vectorSize; i++) {
        EXPECT_EQ(myBinaryTree->insert(vecRandomIntegers[i]), true);
    }

    /* Get the minimum value from the vector */
    int min = *std::min_element(vecRandomIntegers.begin(), vecRandomIntegers.end());

    /* Search for the minimum value */
    minNode = myBinaryTree->min();
    EXPECT_NE(minNode, nullptr);

    /* Check if the minimum value is correct */
    EXPECT_EQ(minNode->key, min);
}

TEST(BinaryTree, Max) {
    std::unique_ptr<data_struct::BinaryTree<int>> myBinaryTree;
    myBinaryTree = std::make_unique<data_struct::BinaryTree<int>>();

    /* Check max value in an empty tree */
    data_struct::BTreeNode<int>* maxNode = myBinaryTree->max();
    EXPECT_EQ(maxNode, nullptr);

    const unsigned int vectorSize = 64u;
    std::vector<int> vecRandomIntegers = {};
    for (long i = 0; i < vectorSize; i++) { vecRandomIntegers.push_back(uni_dist(mt_machine));}

    /* Insert 64 random values */
    for (std::size_t i = 0; i < vectorSize; i++) {
        EXPECT_EQ(myBinaryTree->insert(vecRandomIntegers[i]), true);
    }

    /* Get the maximum value from the vector */
    int max = *std::max_element(vecRandomIntegers.begin(), vecRandomIntegers.end());

    /* Search for the maximum value */
    maxNode = myBinaryTree->max();
    EXPECT_NE(maxNode, nullptr);

    /* Check if the maximum value is correct */
    EXPECT_EQ(maxNode->key, max);
}