//
// Created by Tuna Cici on 25.02.2023.
//

#ifndef MHYSA_STACK_HPP
#define MHYSA_STACK_HPP

#include <ostream>
#include <memory>

namespace data_struct{
    template<typename T>
    class Stack {
    private:
        std::unique_ptr<T[]> m_pContainer;
        std::size_t m_nContainerSize{};

        unsigned int m_uTop{};
        bool m_bIsDynamic{};

        /* Grow or shrink if */
        unsigned short m_uContainerUsage{}; /* Percent */
        const unsigned short m_uMaxAllowedUsage = 90u; /* Percent */
        const unsigned short m_uGrowPercentage = 25u; /* Percent */
        const unsigned short m_uShrinkThreshold = 70u; /* Percent */
        const std::size_t m_nMinAllowedSize = 32u;

        void optimize();
    public:
        Stack(unsigned int size = 0);
        ~Stack();

        Stack(Stack& other) = delete;
        Stack& operator=(const Stack& other) = delete;

        bool push(const T& input);
        bool pop(T& output);
        std::size_t size();
        unsigned short usage();

        template<typename U>
        friend std::ostream& operator<<(std::ostream& os, const Stack<U>& obj);
    };
}

/* Template class definitions are in another file. Sorry... */
#include "DataStructures/Stack.tpp"

#endif