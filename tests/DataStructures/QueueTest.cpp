//
// Created by Tuna Cici on 2.03.2023.
//

#include <random>
#include <vector>
#include <ctime>
#include <limits>

#include "gtest/gtest.h"

#include "DataStructures/Queue.hpp"

/* Random int generator mt19937 */
static std::mt19937 mt_machine(time(nullptr));
static std::uniform_int_distribution uni_dist(
        std::numeric_limits<int>::min(),
        std::numeric_limits<int>::max()
);

TEST(Queue, Edge_Cases_Static) {
    /* Dequeued value */
    int out = 0;

    /* Size is set to -1*/
    /* Possible 'integer overflow' */
    /* Throws std::invalid_argument exception */
    std::unique_ptr<data_struct::Queue<int>> myIntegerQueue;
    EXPECT_THROW({
         try {
             myIntegerQueue = std::make_unique<data_struct::Queue<int>>(-1, false);
         }
         catch(const std::invalid_argument& e) {
             throw;
         }
     }, std::invalid_argument);

    /* Size is set to0 */
    std::unique_ptr<data_struct::Queue<int>> myIntegerQueue2;
    myIntegerQueue2 = std::make_unique<data_struct::Queue<int>>(0u, false);

    /* Both size and container size is 0 */
    /* Cannot enqueue or dequeue */
    EXPECT_EQ(myIntegerQueue2->size(), 0u);
    EXPECT_EQ(myIntegerQueue2->container_size(), 0u);

    EXPECT_EQ(myIntegerQueue2->is_dynamic(), false);
    EXPECT_EQ(myIntegerQueue2->enqueue(43), false);
    EXPECT_EQ(myIntegerQueue2->dequeue(out), false);

    EXPECT_EQ(myIntegerQueue2->size(), 0u);
    EXPECT_EQ(myIntegerQueue2->container_size(), 0u);

    /* Size is set to 1 */
    std::unique_ptr<data_struct::Queue<int>> myIntegerQueue3;
    myIntegerQueue3 = std::make_unique<data_struct::Queue<int>>(1u, false);

    /* Size is 0 and container size is 1 */
    /* Can enqueue and dequeue */
    EXPECT_EQ(myIntegerQueue3->size(), 0u);
    EXPECT_EQ(myIntegerQueue3->container_size(), 1u);

    EXPECT_EQ(myIntegerQueue3->enqueue(44), true);
    EXPECT_EQ(myIntegerQueue3->size(), 1u);
    EXPECT_EQ(myIntegerQueue3->enqueue(45), false);
    EXPECT_EQ(myIntegerQueue3->size(), 1u);
    EXPECT_EQ(myIntegerQueue3->dequeue(out), true);
    EXPECT_EQ(myIntegerQueue3->size(), 0u);
    EXPECT_EQ(out, 44);
    EXPECT_EQ(myIntegerQueue3->dequeue(out), false);

    EXPECT_EQ(myIntegerQueue3->size(), 0u);
    EXPECT_EQ(myIntegerQueue3->container_size(), 1u);
}

TEST(Queue, Edge_Cases_Dynamic) {
    /* Dequeued value */
    int out = 0;

    /* Initial size is set to -1 */
    /* Possible 'integer overflow' */
    /* Throws std::invalid_argument exception */
    std::unique_ptr<data_struct::Queue<int>> myIntegerQueue;
    EXPECT_THROW({
         try {
             myIntegerQueue = std::make_unique<data_struct::Queue<int>>(-1, true);
         }
         catch(const std::invalid_argument& e) {
             throw;
         }
     }, std::invalid_argument);

    /* Initial size can not be smaller than m_nMinAllowedSize */
    /* m_nMinAllowedSize is hard coded to 32u when using dynamic sizing */
    std::unique_ptr<data_struct::Queue<int>> myIntegerQueue2;
    for (unsigned short i = 0; i < 32u; i++) {
        myIntegerQueue2 = std::make_unique<data_struct::Queue<int>>(i, true);
        EXPECT_EQ(myIntegerQueue2->is_dynamic(), true);
        EXPECT_EQ(myIntegerQueue2->container_size(), 32u);
    }

    /* I can push & pop more than the initial size (dynamic sizing) */
    std::unique_ptr<data_struct::Queue<int>> myIntegerQueue3;
    myIntegerQueue3 = std::make_unique<data_struct::Queue<int>>(64u, true);
    EXPECT_EQ(myIntegerQueue3->is_dynamic(), true);

    for (unsigned short i = 0; i < 128u; i++) {
        EXPECT_EQ(myIntegerQueue3->enqueue(i*2 + 7), true);
        EXPECT_EQ(myIntegerQueue3->size(), i+1);
        EXPECT_EQ(64u <= myIntegerQueue3->container_size(), true);
    }

    /* Stack container size does not go below the initial size */
    while(myIntegerQueue3->dequeue(out)) {
        EXPECT_EQ(64u <= myIntegerQueue3->container_size(), true);
    }

    /* Empty container size is equal to the initial size */
    EXPECT_EQ(myIntegerQueue3->container_size(), 64u);
}


TEST(Queue, Usage_Static) {
    std::unique_ptr<data_struct::Queue<int>> myStaticIntegerQueue;
    myStaticIntegerQueue = std::make_unique<data_struct::Queue<int>>(1024, false);
    EXPECT_EQ(myStaticIntegerQueue->is_dynamic(), false);

    std::vector<int> vecRandomIntegers = {};
    for (long i = 0; i < 1024; i++) { vecRandomIntegers.push_back(uni_dist(mt_machine));}

    for (unsigned int i = 0u; i < 1024; i++) {
        EXPECT_EQ(myStaticIntegerQueue->enqueue(vecRandomIntegers[i]), true);
        EXPECT_EQ(myStaticIntegerQueue->size(), (i+1));
        EXPECT_EQ(myStaticIntegerQueue->container_size(), 1024);
    }

    int out = 0;
    for (unsigned int i = 0u; i < myStaticIntegerQueue->container_size(); i++) {
        EXPECT_EQ(myStaticIntegerQueue->dequeue(out), true);
        EXPECT_EQ(myStaticIntegerQueue->size(), 1024 - (i+1));
        EXPECT_EQ(out, vecRandomIntegers[i]);
        EXPECT_EQ(myStaticIntegerQueue->container_size(), 1024);
    }
}

TEST(Queue, Usage_Dynamic) {
    std::unique_ptr<data_struct::Queue<int>> myDynamicIntegerQueue;
    myDynamicIntegerQueue = std::make_unique<data_struct::Queue<int>>(64u,true);
    EXPECT_EQ(myDynamicIntegerQueue->is_dynamic(), true);

    std::vector<int> vecRandomIntegers = {};
    for (long i = 0; i < 1024; i++) { vecRandomIntegers.push_back(uni_dist(mt_machine));}

    for (unsigned int i = 0u; i < vecRandomIntegers.size(); i++) {
        EXPECT_EQ(myDynamicIntegerQueue->enqueue(vecRandomIntegers[i]), true);
        EXPECT_EQ(myDynamicIntegerQueue->size(), (i+1));
        EXPECT_EQ(64u <= myDynamicIntegerQueue->container_size(), true);
    }

    int out = 0;
    for (unsigned int i = 0u; i < vecRandomIntegers.size(); i++) {
        EXPECT_EQ(myDynamicIntegerQueue->dequeue(out), true);
        EXPECT_EQ(myDynamicIntegerQueue->size(), 1024 - (i+1));
        EXPECT_EQ(out, vecRandomIntegers[i]);
        EXPECT_EQ(64u <= myDynamicIntegerQueue->container_size(), true);
    }

    EXPECT_EQ(myDynamicIntegerQueue->container_size(), 64u);
}
