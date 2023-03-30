//
// Created by Tuna Cici on 30.03.2023.
//

#include <random>
#include <vector>
#include <ctime>
#include <limits>

#include "gtest/gtest.h"

#include "DataStructures/RedBlackTree.hpp"

/* Random int generator mt19937 */
static std::mt19937 mt_machine(time(nullptr));
static std::uniform_int_distribution uni_dist(
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max()
);

TEST(RedBlackTree, Initialization) {
    std::unique_ptr<data_struct::RedBlackTree<int>> myRedBlackTree;
    myRedBlackTree = std::make_unique<data_struct::RedBlackTree<int>>();

    EXPECT_EQ(myRedBlackTree->size(), 0u);
    EXPECT_EQ(myRedBlackTree->height(), 0u);
}

TEST(RedBlackTree, Insert) {
    std::unique_ptr<data_struct::RedBlackTree<int>> myRedBlackTree;
    myRedBlackTree = std::make_unique<data_struct::RedBlackTree<int>>();

    const unsigned int vectorSize = 64u;
    std::vector<int> vecRandomIntegers = {};
    for (long i = 0; i < vectorSize; i++) { vecRandomIntegers.push_back(uni_dist(mt_machine));}

    /* Make sure the vector is unique */
    std::sort(vecRandomIntegers.begin(), vecRandomIntegers.end());
    vecRandomIntegers.erase(std::unique(vecRandomIntegers.begin(), vecRandomIntegers.end()), vecRandomIntegers.end());

    /* Check if the vector is unique */
    EXPECT_EQ(vecRandomIntegers.size(), vectorSize);

    /* Insert 64 random values */
    for (std::size_t i = 0; i < vectorSize; i++) {
        EXPECT_EQ(myRedBlackTree->insert(vecRandomIntegers[i]), true);
        EXPECT_EQ(myRedBlackTree->size(), i + 1);

        /* For LLRB tree, height is at most 2 * log2(n + 1) */
        EXPECT_LE(myRedBlackTree->height(), 2 * std::log2( myRedBlackTree->size() + 1));
    }
}

TEST(RedBlackTree, Search) {
    std::unique_ptr<data_struct::RedBlackTree<int>> myRedBlackTree;
    myRedBlackTree = std::make_unique<data_struct::RedBlackTree<int>>();

    /* Search for an empty tree */
    EXPECT_EQ(myRedBlackTree->search(47), nullptr);

    /* Pre-determined values */
    std::vector<int> someValues = {8, 4, 12, 2, 6, 10, 14, 1, 3, 5, 7, 9, 15};
    for (int someValue : someValues) {
        EXPECT_EQ(myRedBlackTree->insert(someValue), true);
    }

    /* Search for a non-existing value */
    EXPECT_EQ(myRedBlackTree->search(47), nullptr);

    /* Search for the minimum value */
    EXPECT_EQ(*myRedBlackTree->search(1), 1);

    /* Search for the maximum value */
    EXPECT_EQ(*myRedBlackTree->search(15), 15);

    const unsigned int vectorSize = 1024u;
    std::vector<int> vecRandomIntegers = {};
    for (long i = 0; i < vectorSize; i++) { vecRandomIntegers.push_back(uni_dist(mt_machine));}

    /* Insert random values */
    for (std::size_t i = 0; i < vecRandomIntegers.size(); i++) {
        EXPECT_EQ(myRedBlackTree->insert(vecRandomIntegers[i]), true);
    }

    /* Search for all values */
    for (std::size_t i = 0; i < vecRandomIntegers.size(); i++) {
        const int* searchResult = myRedBlackTree->search(vecRandomIntegers[i]);
        EXPECT_NE(searchResult, nullptr);
        EXPECT_EQ(*searchResult, vecRandomIntegers[i]);
    }
}

TEST(RedBlackTree, Remove) {
    std::unique_ptr<data_struct::RedBlackTree<int>> myRedBlackTree;
    myRedBlackTree = std::make_unique<data_struct::RedBlackTree<int>>();

    /* Remove from an empty tree */
    EXPECT_EQ(myRedBlackTree->remove(47), false);
    EXPECT_EQ(myRedBlackTree->size(), 0u);

    /* Pre-determined values */
    std::vector<int> someValues = {8, 4, 12, 2, 6, 10, 14, 1, 3, 5, 7, 9, 15};

    /* Remove from a tree with only one node */
    for (int someValue : someValues) {
        EXPECT_EQ(myRedBlackTree->insert(someValue), true);
        EXPECT_EQ(myRedBlackTree->remove(someValue), true);
        EXPECT_EQ(myRedBlackTree->size(), 0u);
    }

    /* Insert all pre-determined values */
    for (int someValue : someValues) {
        EXPECT_EQ(myRedBlackTree->insert(someValue), true);
    }

    /* Remove the minimum value */
    EXPECT_EQ(myRedBlackTree->remove(1), true);
    EXPECT_EQ(myRedBlackTree->search(1), nullptr);

    /* Remove the maximum value */
    EXPECT_EQ(myRedBlackTree->remove(15), true);
    EXPECT_EQ(myRedBlackTree->search(15), nullptr);

    EXPECT_EQ(myRedBlackTree->size(), someValues.size() - 2);

    /* Remove all remaining values */
    for (int someValue : someValues) {
        if (someValue == 1 || someValue == 15) { continue; }

        EXPECT_EQ(myRedBlackTree->remove(someValue), true);
        EXPECT_EQ(myRedBlackTree->search(someValue), nullptr);
    }

    /* Make sure the tree is empty */
    EXPECT_EQ(myRedBlackTree->size(), 0u);

    const unsigned int vectorSize = 1024u;
    std::vector<int> vecRandomIntegers = {};
    for (long i = 0; i < vectorSize; i++) { vecRandomIntegers.push_back(uni_dist(mt_machine));}

    /* Make sure the vector is unique */
    std::sort(vecRandomIntegers.begin(), vecRandomIntegers.end());
    vecRandomIntegers.erase(std::unique(vecRandomIntegers.begin(), vecRandomIntegers.end()), vecRandomIntegers.end());

    /* Insert random values */
    for (std::size_t i = 0; i < vecRandomIntegers.size(); i++) {
        EXPECT_EQ(myRedBlackTree->insert(vecRandomIntegers[i]), true);
        EXPECT_EQ(myRedBlackTree->size(), i + 1);
    }

    /* Remove all values */
    for (std::size_t i = 0; i < vecRandomIntegers.size(); i++) {
        EXPECT_EQ(myRedBlackTree->remove(vecRandomIntegers[i]), true);
        EXPECT_EQ(myRedBlackTree->size(), vecRandomIntegers.size() - i - 1);

        /* Search for the removed value */
        EXPECT_EQ(myRedBlackTree->search(vecRandomIntegers[i]), nullptr);

        /* Check if remaining values are still in the tree */
        for (std::size_t j = i + 1; j < vecRandomIntegers.size(); j++) {
            EXPECT_NE(myRedBlackTree->search(vecRandomIntegers[j]), nullptr);
        }
    }
}
