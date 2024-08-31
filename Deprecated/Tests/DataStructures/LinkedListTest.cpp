//
// Created by Tuna Cici on 16.03.2023.
//

#include <random>
#include <vector>
#include <ctime>
#include <limits>

#include "gtest/gtest.h"

#include "DataStructures/LinkedList.hpp"

/* Random int generator mt19937 */
static std::mt19937 mt_machine(time(nullptr));
static std::uniform_int_distribution uni_dist(
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max()
);

TEST(LinkedList, Initialization) {
    std::unique_ptr<data_struct::LinkedList<int>> mySinglyList;
    std::unique_ptr<data_struct::LinkedList<int>> myDoublyList;

    mySinglyList = std::make_unique<data_struct::LinkedList<int>>(data_struct::single_link);
    myDoublyList = std::make_unique<data_struct::LinkedList<int>>(data_struct::double_link);

    EXPECT_EQ(mySinglyList->type(), data_struct::single_link);
    EXPECT_EQ(myDoublyList->type(), data_struct::double_link);

    EXPECT_EQ(mySinglyList->size(), 0u);
    EXPECT_EQ(myDoublyList->size(), 0u);
}

TEST(LinkedList, Basic_Operations) {
    std::unique_ptr<data_struct::LinkedList<int>> mySinglyList;
    std::unique_ptr<data_struct::LinkedList<int>> myDoublyList;

    mySinglyList = std::make_unique<data_struct::LinkedList<int>>(data_struct::single_link);
    myDoublyList = std::make_unique<data_struct::LinkedList<int>>(data_struct::double_link);

    /* Insert something at idx 0 */
    EXPECT_EQ(mySinglyList->insert(47, 0u), true);
    EXPECT_EQ(myDoublyList->insert(48, 0u), true);

    EXPECT_EQ(mySinglyList->size(), 1u);
    EXPECT_EQ(myDoublyList->size(), 1u);

    int out = 0;

    /* Remove from idx 0 */
    EXPECT_EQ(mySinglyList->remove(0u, out), true);
    EXPECT_EQ(out, 47);
    EXPECT_EQ(myDoublyList->remove(0u, out), true);
    EXPECT_EQ(out, 48);

    EXPECT_EQ(mySinglyList->size(), 0u);
    EXPECT_EQ(myDoublyList->size(), 0u);

    const unsigned int vectorSize = 64u;
    std::vector<int> vecRandomIntegers = {};
    for (long i = 0; i < vectorSize; i++) { vecRandomIntegers.push_back(uni_dist(mt_machine));}

    /* Insert 64 random values at idx 0 */
    for (unsigned int i = 0; i < vectorSize; i++) {
        EXPECT_EQ(mySinglyList->insert(vecRandomIntegers[i], 0u), true);
        EXPECT_EQ(myDoublyList->insert(vecRandomIntegers[i], 0u), true);

        EXPECT_EQ(mySinglyList->size(), i + 1);
        EXPECT_EQ(myDoublyList->size(), i + 1);
    }

    /* Try to insert at out of index */
    EXPECT_EQ(mySinglyList->insert(47, -1), false);
    EXPECT_EQ(mySinglyList->insert(47, vectorSize*99), false);

    EXPECT_EQ(myDoublyList->insert(47, -1), false);
    EXPECT_EQ(myDoublyList->insert(47, vectorSize*99), false);

    /* Try to insert/remove to/from the middle */
    EXPECT_EQ(mySinglyList->insert(47, vectorSize / 2u), true);
    EXPECT_EQ(mySinglyList->remove(vectorSize / 2u, out), true);
    EXPECT_EQ(out, 47);

    EXPECT_EQ(myDoublyList->insert(48, vectorSize / 2u), true);
    EXPECT_EQ(myDoublyList->remove(vectorSize / 2u, out), true);
    EXPECT_EQ(out, 48);

    /* Remove from idx 0 to confirm the initial insertion was correct */
    for (unsigned int i = 0; i < vectorSize; i++) {
        EXPECT_EQ(mySinglyList->remove(0u, out), true);
        EXPECT_EQ(out, vecRandomIntegers[vectorSize - (i + 1)]);
        EXPECT_EQ(mySinglyList->size(), vectorSize - (i + 1));

        /* Reset out */
        out = 0;

        EXPECT_EQ(myDoublyList->remove(0u, out), true);
        EXPECT_EQ(out, vecRandomIntegers[vectorSize - (i + 1)]);
        EXPECT_EQ(myDoublyList->size(), vectorSize - (i + 1));
    }
}

TEST(LinkedList, Queue_Operations) {
    std::unique_ptr<data_struct::LinkedList<int>> mySinglyList;
    std::unique_ptr<data_struct::LinkedList<int>> myDoublyList;

    mySinglyList = std::make_unique<data_struct::LinkedList<int>>(data_struct::single_link);
    myDoublyList = std::make_unique<data_struct::LinkedList<int>>(data_struct::double_link);

    const unsigned int vectorSize = 64u;
    std::vector<int> vecRandomIntegers = {};
    for (long i = 0; i < vectorSize; i++) { vecRandomIntegers.push_back(uni_dist(mt_machine));}

    /* Enqueue 64 random values */
    for (unsigned int i = 0; i < vectorSize; i++) {
        EXPECT_EQ(mySinglyList->enqueue(vecRandomIntegers[i]), true);
        EXPECT_EQ(myDoublyList->enqueue(vecRandomIntegers[i]), true);

        EXPECT_EQ(mySinglyList->size(), i + 1);
        EXPECT_EQ(myDoublyList->size(), i + 1);
    }

    int out = 0;

    /* Dequeue */
    for (unsigned int i = 0; i < vectorSize; i++) {
        EXPECT_EQ(mySinglyList->dequeue(out), true);
        EXPECT_EQ(out, vecRandomIntegers[i]);
        EXPECT_EQ(mySinglyList->size(), vectorSize - (i + 1));

        /* Reset out */
        out = 0;

        EXPECT_EQ(myDoublyList->dequeue(out), true);
        EXPECT_EQ(out, vecRandomIntegers[i]);
        EXPECT_EQ(myDoublyList->size(), vectorSize - (i + 1));
    }
}

TEST(LinkedList, Stack_Operations) {
    std::unique_ptr<data_struct::LinkedList<int>> mySinglyList;
    std::unique_ptr<data_struct::LinkedList<int>> myDoublyList;

    mySinglyList = std::make_unique<data_struct::LinkedList<int>>(data_struct::single_link);
    myDoublyList = std::make_unique<data_struct::LinkedList<int>>(data_struct::double_link);

    const unsigned int vectorSize = 64u;
    std::vector<int> vecRandomIntegers = {};
    for (long i = 0; i < vectorSize; i++) { vecRandomIntegers.push_back(uni_dist(mt_machine));}

    /* Push 64 random values */
    for (unsigned int i = 0; i < vectorSize; i++) {
        EXPECT_EQ(mySinglyList->push(vecRandomIntegers[i]), true);
        EXPECT_EQ(myDoublyList->push(vecRandomIntegers[i]), true);

        EXPECT_EQ(mySinglyList->size(), i + 1);
        EXPECT_EQ(myDoublyList->size(), i + 1);
    }

    int out = 0;

    /* Pop */
    for (unsigned int i = 0; i < vectorSize; i++) {
        EXPECT_EQ(mySinglyList->pop(out), true);
        EXPECT_EQ(out, vecRandomIntegers[vectorSize - (i + 1)]);
        EXPECT_EQ(mySinglyList->size(), vectorSize - (i + 1));

        /* Reset out */
        out = 0;

        EXPECT_EQ(myDoublyList->pop(out), true);
        EXPECT_EQ(out, vecRandomIntegers[vectorSize - (i + 1)]);
        EXPECT_EQ(myDoublyList->size(), vectorSize - (i + 1));
    }
}

TEST(LinkedList, Search) {
    std::unique_ptr<data_struct::LinkedList<int>> mySinglyList;
    std::unique_ptr<data_struct::LinkedList<int>> myDoublyList;

    mySinglyList = std::make_unique<data_struct::LinkedList<int>>(data_struct::single_link);
    myDoublyList = std::make_unique<data_struct::LinkedList<int>>(data_struct::double_link);

    int garbage = 0;
    std::size_t out = 0u;

    /* Search empty linked list */
    EXPECT_EQ(mySinglyList->search(47, out), false);
    EXPECT_EQ(myDoublyList->search(47, out), false);

    const unsigned int vectorSize = 64u;
    std::vector<int> vecRandomIntegers = {};
    for (long i = 0; i < vectorSize; i++) { vecRandomIntegers.push_back(uni_dist(mt_machine));}

    /* Insert 64 random values */
    for (unsigned int i = 0; i < vectorSize; i++) {
        EXPECT_EQ(mySinglyList->insert(vecRandomIntegers[i]), true);
        EXPECT_EQ(myDoublyList->insert(vecRandomIntegers[i]), true);
    }

    /* Search for all added values */
    for (unsigned int i = 0; i < vectorSize; i++) {
        EXPECT_EQ(mySinglyList->search(vecRandomIntegers[i], out), true);
        EXPECT_EQ(out, vectorSize - (i + 1));

        /* Reset out */
        out = 0u;

        EXPECT_EQ(myDoublyList->search(vecRandomIntegers[i], out), true);
        EXPECT_EQ(out, vectorSize - (i + 1));
    }

    /* Clear the linked lists */
    while (mySinglyList->size() != 0u || myDoublyList->size() != 0u) {
        EXPECT_EQ(mySinglyList->pop(garbage), true);
        EXPECT_EQ(myDoublyList->pop(garbage), true);
    }

    /* Add pre-determined values */
    EXPECT_EQ(mySinglyList->push(47), true);
    EXPECT_EQ(mySinglyList->push(48), true);

    EXPECT_EQ(myDoublyList->push(47), true);
    EXPECT_EQ(myDoublyList->push(48), true);

    /* Search for a non-existent value */
    EXPECT_EQ(mySinglyList->search(999, out), false);
    EXPECT_EQ(myDoublyList->search(999, out), false);
}

TEST(LinkedList, Peek) {
    std::unique_ptr<data_struct::LinkedList<int>> mySinglyList;
    std::unique_ptr<data_struct::LinkedList<int>> myDoublyList;

    mySinglyList = std::make_unique<data_struct::LinkedList<int>>(data_struct::single_link);
    myDoublyList = std::make_unique<data_struct::LinkedList<int>>(data_struct::double_link);

    int out = 0;

    const unsigned int vectorSize = 64u;
    std::vector<int> vecRandomIntegers = {};
    for (long i = 0; i < vectorSize; i++) { vecRandomIntegers.push_back(uni_dist(mt_machine));}

    /* Insert 64 random values */
    for (unsigned int i = 0; i < vectorSize; i++) {
        EXPECT_EQ(mySinglyList->insert(vecRandomIntegers[i], i), true);
        EXPECT_EQ(myDoublyList->insert(vecRandomIntegers[i], i), true);
    }

    /* Peek at out of index -1 */
    EXPECT_THROW({
         try {
             mySinglyList->peek(-1, out);
         }
         catch(const std::invalid_argument& e) {
             throw;
         }
     }, std::invalid_argument);
    EXPECT_THROW({
         try {
             myDoublyList->peek(-1, out);
         }
         catch(const std::invalid_argument& e) {
             throw;
         }
     }, std::invalid_argument);

    /* Peek at out of index grater than size */
    EXPECT_EQ(mySinglyList->peek(999, out), false);
    EXPECT_EQ(myDoublyList->peek(999, out), false);

    /* Peek at each of them */
    /* Search for all added values */
    for (unsigned int i = 0; i < vectorSize; i++) {
        EXPECT_EQ(mySinglyList->peek(i, out), true);
        EXPECT_EQ(out, vecRandomIntegers[i]);

        /* Reset out */
        out = 0u;

        EXPECT_EQ(myDoublyList->peek(i, out), true);
        EXPECT_EQ(out, vecRandomIntegers[i]);
    }
}

TEST(LinkedList, Sort) {
    std::unique_ptr<data_struct::LinkedList<int>> mySinglyList;
    std::unique_ptr<data_struct::LinkedList<int>> myDoublyList;

    mySinglyList = std::make_unique<data_struct::LinkedList<int>>(data_struct::single_link);
    myDoublyList = std::make_unique<data_struct::LinkedList<int>>(data_struct::double_link);

    int out = 0;

    /* Sort an empty list */
    EXPECT_EQ(mySinglyList->sort(), true);
    EXPECT_EQ(myDoublyList->sort(), true);

    /* Sort a list with size 1 */
    EXPECT_EQ(mySinglyList->insert(47), true);
    EXPECT_EQ(myDoublyList->insert(48), true);
    EXPECT_EQ(mySinglyList->size(), 1u);
    EXPECT_EQ(myDoublyList->size(), 1u);

    EXPECT_EQ(mySinglyList->sort(), true);
    EXPECT_EQ(myDoublyList->sort(), true);

    /* Clear the list */
    EXPECT_EQ(mySinglyList->pop(out), true);
    EXPECT_EQ(myDoublyList->pop(out), true);

    const unsigned int vectorSize = 64u;
    std::vector<int> vecRandomIntegers = {};
    for (long i = 0; i < vectorSize; i++) { vecRandomIntegers.push_back(uni_dist(mt_machine));}

    /* Insert 64 random values */
    for (unsigned int i = 0; i < vectorSize; i++) {
        EXPECT_EQ(mySinglyList->insert(vecRandomIntegers[i], i), true);
        EXPECT_EQ(myDoublyList->insert(vecRandomIntegers[i], i), true);
    }

    EXPECT_EQ(mySinglyList->sort(), true);
    EXPECT_EQ(myDoublyList->sort(), true);

    int previousSingly = 0;
    int previousDoubly = 0;

    EXPECT_EQ(mySinglyList->peek(0, previousSingly), true);
    EXPECT_EQ(myDoublyList->peek(0, previousDoubly), true);

    /* Check if sorted */
    for (unsigned int i = 0; i < vectorSize; i++) {
        EXPECT_EQ(mySinglyList->peek(i, out), true);
        EXPECT_GE(out, previousSingly);
        previousSingly = out;

        /* Reset out */
        out = 0u;

        EXPECT_EQ(myDoublyList->peek(i, out), true);
        EXPECT_GE(out, previousDoubly);
        previousDoubly = out;

    }
}

TEST(LinkedList, Reverse) {
    std::unique_ptr<data_struct::LinkedList<int>> mySinglyList;
    std::unique_ptr<data_struct::LinkedList<int>> myDoublyList;

    mySinglyList = std::make_unique<data_struct::LinkedList<int>>(data_struct::single_link);
    myDoublyList = std::make_unique<data_struct::LinkedList<int>>(data_struct::double_link);

    int out = 0;

    /* Reverse an empty list */
    EXPECT_EQ(mySinglyList->reverse(), true);
    EXPECT_EQ(myDoublyList->reverse(), true);

    /* Reverse a list with size 1 */
    EXPECT_EQ(mySinglyList->insert(47), true);
    EXPECT_EQ(myDoublyList->insert(48), true);
    EXPECT_EQ(mySinglyList->size(), 1u);
    EXPECT_EQ(myDoublyList->size(), 1u);

    EXPECT_EQ(mySinglyList->reverse(), true);
    EXPECT_EQ(myDoublyList->reverse(), true);

    /* Clear the list */
    EXPECT_EQ(mySinglyList->pop(out), true);
    EXPECT_EQ(out, 47);

    EXPECT_EQ(myDoublyList->pop(out), true);
    EXPECT_EQ(out, 48);

    const unsigned int vectorSize = 64u;
    std::vector<int> vecRandomIntegers = {};
    for (long i = 0; i < vectorSize; i++) { vecRandomIntegers.push_back(uni_dist(mt_machine));}

    /* Insert 64 random values */
    for (unsigned int i = 0; i < vectorSize; i++) {
        EXPECT_EQ(mySinglyList->insert(vecRandomIntegers[i], i), true);
        EXPECT_EQ(myDoublyList->insert(vecRandomIntegers[i], i), true);
    }

    EXPECT_EQ(mySinglyList->reverse(), true);
    EXPECT_EQ(myDoublyList->reverse(), true);

    /* Check if reversed */
    for (unsigned int i = 0; i < vectorSize; i++) {
        EXPECT_EQ(mySinglyList->peek(i, out), true);
        EXPECT_EQ(out, vecRandomIntegers[vectorSize - (i + 1)]);

        /* Reset out */
        out = 0u;

        EXPECT_EQ(myDoublyList->peek(i, out), true);
        EXPECT_EQ(out, vecRandomIntegers[vectorSize - (i + 1)]);
    }
}

