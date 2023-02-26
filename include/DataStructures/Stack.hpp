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
        unsigned int m_nTop{};
        bool m_bIsDynamic{};

        const unsigned short m_nGrowSize = 32u;
        const unsigned short m_nShrinkThreshold = 75u; /* Percent */
    public:
        Stack(unsigned int size = 0);
        ~Stack();

        Stack(Stack& other) = delete;
        Stack& operator=(const Stack& other) = delete;

        bool grow();
        bool shrink();
        bool push(const T& input);
        bool pop(T& output);
        std::size_t size();

        template<typename U>
        friend std::ostream& operator<<(std::ostream& os, const Stack<U>& obj);
    };
}

/* Template class definitions are in another file. Sorry... */
#include "DataStructures/Stack.tpp"

#endif