//
// Created by Tuna Cici on 2.03.2023.
//

#include <random>
#include <vector>
#include <ctime>
#include <limits>

#include "gtest/gtest.h"

#include "DataStructures/Stack.hpp"

/* Random int generator mt19937 */
static std::mt19937 mt_machine(time(nullptr));
static std::uniform_int_distribution uni_dist(
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max()
);

TEST(Stack, Edge_Cases_Static) {
    /* Popped value */
    int out = 0;

    /* Size is set to -1*/
    /* Possible 'integer overflow' */
    /* Throws std::invalid_argument exception */
    std::unique_ptr<data_struct::Stack<int>> myIntegerStack;
    EXPECT_THROW({
         try {
             myIntegerStack = std::make_unique<data_struct::Stack<int>>(-1, false);
         }
         catch(const std::invalid_argument& e) {
             throw;
         }
     }, std::invalid_argument);

    /* Size is set to0 */
    std::unique_ptr<data_struct::Stack<int>> myIntegerStack2;
    myIntegerStack2 = std::make_unique<data_struct::Stack<int>>(0u, false);

    /* Both size and container size is 0 */
    /* Cannot push or pop */
    EXPECT_EQ(myIntegerStack2->size(), 0u);
    EXPECT_EQ(myIntegerStack2->container_size(), 0u);

    EXPECT_EQ(myIntegerStack2->is_dynamic(), false);
    EXPECT_EQ(myIntegerStack2->push(43), false);
    EXPECT_EQ(myIntegerStack2->pop(out), false);

    EXPECT_EQ(myIntegerStack2->size(), 0u);
    EXPECT_EQ(myIntegerStack2->container_size(), 0u);

    /* Size is set to 1 */
    std::unique_ptr<data_struct::Stack<int>> myIntegerStack3;
    myIntegerStack3 = std::make_unique<data_struct::Stack<int>>(1u, false);

    /* Size is 0 and container size is 1 */
    /* Can push and pop */
    EXPECT_EQ(myIntegerStack3->size(), 0u);
    EXPECT_EQ(myIntegerStack3->container_size(), 1u);

    EXPECT_EQ(myIntegerStack3->push(44), true);
    EXPECT_EQ(myIntegerStack3->size(), 1u);
    EXPECT_EQ(myIntegerStack3->push(45), false);
    EXPECT_EQ(myIntegerStack3->size(), 1u);
    EXPECT_EQ(myIntegerStack3->pop(out), true);
    EXPECT_EQ(myIntegerStack3->size(), 0u);
    EXPECT_EQ(out, 44);
    EXPECT_EQ(myIntegerStack3->pop(out), false);

    EXPECT_EQ(myIntegerStack3->size(), 0u);
    EXPECT_EQ(myIntegerStack3->container_size(), 1u);
}

TEST(Stack, Edge_Cases_Dynamic) {
    /* Popped value */
    int out = 0;

    /* Initial size is set to -1 */
    /* Possible 'integer overflow' */
    /* Throws std::invalid_argument exception */
    std::unique_ptr<data_struct::Stack<int>> myIntegerStack;
    EXPECT_THROW({
         try {
             myIntegerStack = std::make_unique<data_struct::Stack<int>>(-1, true);
         }
         catch(const std::invalid_argument& e) {
             throw;
         }
     }, std::invalid_argument);

    /* Initial size can not be smaller than m_nMinAllowedSize */
    /* m_nMinAllowedSize is hard coded to 32u when using dynamic sizing */
    std::unique_ptr<data_struct::Stack<int>> myIntegerStack2;
    for (unsigned short i = 0; i < 32u; i++) {
        myIntegerStack2 = std::make_unique<data_struct::Stack<int>>(i, true);
        EXPECT_EQ(myIntegerStack2->is_dynamic(), true);
        EXPECT_EQ(myIntegerStack2->container_size(), 32u);
    }

    /* I can push & pop more than the initial size (dynamic sizing) */
    std::unique_ptr<data_struct::Stack<int>> myIntegerStack3;
    myIntegerStack3 = std::make_unique<data_struct::Stack<int>>(64u, true);
    EXPECT_EQ(myIntegerStack3->is_dynamic(), true);

    for (unsigned short i = 0; i < 128u; i++) {
        EXPECT_EQ(myIntegerStack3->push(i*2 + 7), true);
        EXPECT_EQ(myIntegerStack3->size(), i+1);
        EXPECT_EQ(64u <= myIntegerStack3->container_size(), true);
    }

    /* Stack container size does not go below the initial size */
    while(myIntegerStack3->pop(out)) {
        EXPECT_EQ(64u <= myIntegerStack3->container_size(), true);
    }

    /* Empty container size is equal to the initial size */
    EXPECT_EQ(myIntegerStack3->container_size(), 64u);
}


TEST(Stack, Usage_Static) {
    std::unique_ptr<data_struct::Stack<int>> myStaticIntegerStack;
    myStaticIntegerStack = std::make_unique<data_struct::Stack<int>>(1024, false);
    EXPECT_EQ(myStaticIntegerStack->is_dynamic(), false);

    std::vector<int> vecRandomIntegers = {};
    for (long i = 0; i < 1024; i++) { vecRandomIntegers.push_back(uni_dist(mt_machine));}

    for (unsigned int i = 0u; i < 1024; i++) {
        EXPECT_EQ(myStaticIntegerStack->push(vecRandomIntegers[i]), true);
        EXPECT_EQ(myStaticIntegerStack->size(), (i+1));
        EXPECT_EQ(myStaticIntegerStack->container_size(), 1024);
    }

    int out = 0;
    for (unsigned int i = 0u; i < myStaticIntegerStack->container_size(); i++) {
        EXPECT_EQ(myStaticIntegerStack->pop(out), true);
        EXPECT_EQ(myStaticIntegerStack->size(), 1024 - (i+1));
        EXPECT_EQ(out, vecRandomIntegers[1024 - (i+1)]);
        EXPECT_EQ(myStaticIntegerStack->container_size(), 1024);
    }
}

TEST(Stack, Usage_Dynamic) {
    std::unique_ptr<data_struct::Stack<int>> myDynamicIntegerStack;
    myDynamicIntegerStack = std::make_unique<data_struct::Stack<int>>(64u,true);
    EXPECT_EQ(myDynamicIntegerStack->is_dynamic(), true);

    std::vector<int> vecRandomIntegers = {};
    for (long i = 0; i < 1024; i++) { vecRandomIntegers.push_back(uni_dist(mt_machine));}

    for (unsigned int i = 0u; i < vecRandomIntegers.size(); i++) {
        EXPECT_EQ(myDynamicIntegerStack->push(vecRandomIntegers[i]), true);
        EXPECT_EQ(myDynamicIntegerStack->size(), (i+1));
        EXPECT_EQ(64u <= myDynamicIntegerStack->container_size(), true);
    }

    int out = 0;
    for (unsigned int i = 0u; i < vecRandomIntegers.size(); i++) {
        EXPECT_EQ(myDynamicIntegerStack->pop(out), true);
        EXPECT_EQ(myDynamicIntegerStack->size(), 1024 - (i+1));
        EXPECT_EQ(out, vecRandomIntegers[1024 - (i+1)]);
        EXPECT_EQ(64u <= myDynamicIntegerStack->container_size(), true);
    }

    EXPECT_EQ(myDynamicIntegerStack->container_size(), 64u);
}
