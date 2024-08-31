//
// Created by Tuna Cici on 4.03.2023.
//

#ifndef MHYSA_QUEUE_HPP
#define MHYSA_QUEUE_HPP

#include <ostream>
#include <memory>

namespace data_struct {
    template<typename T>
    class Queue {
    private:
        std::unique_ptr<T[]> m_pContainer;
        std::size_t m_nContainerSize{};
        bool m_bIsDynamic{};

        unsigned int m_uHead{};
        unsigned int m_uTail{};
        std::size_t m_uSize{};

        unsigned short m_uContainerUsage{}; /* Percent */
        const unsigned short m_uMaxAllowedUsage = 90u; /* Percent */
        const unsigned short m_uGrowPercentage = 25u; /* Percent */
        const unsigned short m_uShrinkThreshold = 70u; /* Percent */
        std::size_t m_nMinAllowedSize = 32u;

        void optimize();
    public:
        Queue(unsigned int size = 0u, bool dynamic = true);
        ~Queue();

        Queue(Queue& other) = delete;
        Queue& operator=(const Queue& other) = delete;

        bool enqueue(const T& input);
        bool dequeue(T& output);

        std::size_t size();
        std::size_t container_size();
        unsigned short usage();
        bool is_dynamic();

        template<typename U>
        friend std::ostream& operator<<(std::ostream& os, const Queue<U>& obj);
    };

}

/* Template class definitions are in another file. Sorry... */
#include "DataStructures/Queue.tpp"

#endif
