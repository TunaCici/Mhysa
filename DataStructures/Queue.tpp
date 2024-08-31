//
// Created by Tuna Cici on 4.03.2023.
//

#include "DataStructures/Queue.hpp"

namespace data_struct {
    template<typename T>
    Queue<T>::Queue(unsigned int size, bool dynamic) {
        if (size == std::numeric_limits<unsigned int>::max()) {
            throw std::invalid_argument("Possible integer overflow. Size was set to: " + std::to_string(size));
        }

        this->m_pContainer = nullptr;
        this->m_nContainerSize = size;
        this->m_bIsDynamic = dynamic;

        this->m_uHead = 0u;
        this->m_uTail = 0u;
        this->m_uSize = 0u;

        this->m_uContainerUsage = 0u;

        if (this->m_bIsDynamic == true) {
            /* Initial size cannot be smaller than m_nMinAllowedSize if dynamic active */
            if (this->m_nContainerSize <= this->m_nMinAllowedSize) {
                this->m_nContainerSize = this->m_nMinAllowedSize;
            }

            /* Make sure that the empty container does not shrink below the initial size when 'optimize()' */
            this->m_nMinAllowedSize = this->m_nContainerSize;
        }

        this->m_pContainer = std::make_unique<T[]>(this->m_nContainerSize);

        /* Accessing all array elements just in case */
        for (std::size_t idx = 0; idx < this->m_nContainerSize; idx++) {
            T temp = this->m_pContainer[idx];
        }
    }

    template<typename T>
    Queue<T>::~Queue() {
        this->m_pContainer = nullptr;
    }

    template<typename T>
    void Queue<T>::optimize() {
        this->m_uContainerUsage = this->m_uSize * 100u / this->m_nContainerSize;
        std::size_t optimalContainerSize = this->m_nContainerSize;

        if (this->m_uMaxAllowedUsage <= this->m_uContainerUsage) {
            /* TODO: Better way to calculate optimalContainerSize. Use float? */
            optimalContainerSize = (this->m_nContainerSize * (100u + this->m_uGrowPercentage)) / 100u;
        }
        else if (this->m_uContainerUsage <= this->m_uShrinkThreshold) {
            /* Shrink down to m_uMaxAllowedUsage minus 5. TODO: Explain why 5 is used */
            unsigned short shrinkPercentage = (100u * this->m_uShrinkThreshold) / (this->m_uMaxAllowedUsage - 5u);

            /* TODO: Better way to calculate optimalContainerSize. Use float? */
            optimalContainerSize = (this->m_nContainerSize * shrinkPercentage) / 100u;
        }
        else {
            /* Already optimized */
        }

        /* Do not shrink below the m_nMinAllowedSize */
        if (optimalContainerSize < this->m_nMinAllowedSize){
            optimalContainerSize = this->m_nMinAllowedSize;
        }

        if (optimalContainerSize != this->m_nContainerSize) {
            try{
                std::unique_ptr<T[]> newContainer = std::make_unique<T[]>(optimalContainerSize);

                /* Move from one container to other */
                /* TODO: More efficient way possible? */
                for (std::size_t idx = 0u; idx < this->m_uSize; idx++) {
                    newContainer[idx] = this->m_pContainer[this->m_uHead];
                    this->m_uHead = (this->m_uHead + 1u) % this->m_nContainerSize;
                }

                this->m_nContainerSize = optimalContainerSize;
                this->m_pContainer = std::move(newContainer);
                this->m_uHead = 0u;
                this->m_uTail = this->m_uSize;
            }
            catch (const std::exception& e) {
                /* TODO: Do nothing for now */
            }
        }
    }


    template<typename T>
    bool Queue<T>::enqueue(const T &input) {
        bool retValue = false;

        /* Check for remaining space */
        bool isFull = (this->m_nContainerSize <= this->m_uSize);

        if (!isFull) {
            this->m_pContainer[this->m_uTail] = input;
            this->m_uTail = (this->m_uTail + 1u) % this->m_nContainerSize;

            this->m_uSize += 1u;

            if (this->m_bIsDynamic) {
                this->optimize();
            }

            this->m_uContainerUsage = this->m_uSize * 100u / this->m_nContainerSize;

            retValue = true;
        }

        return retValue;
    }

    template<typename T>
    bool Queue<T>::dequeue(T &output) {
        bool retValue = false;

        bool isEmpty = (this->m_uSize == 0);

        if (!isEmpty) {
            output = this->m_pContainer[this->m_uHead];

            /* TODO: What to do with the previous value? */
            this->m_uHead = (this->m_uHead + 1u) % this->m_nContainerSize;

            if (0u != this->m_uSize) {
                this->m_uSize -= 1u;
            }

            if (this->m_bIsDynamic) {
                this->optimize();
            }

            this->m_uContainerUsage = this->m_uSize * 100u / this->m_nContainerSize;

            retValue = true;
        }

        return retValue;
    }

    template<typename T>
    std::size_t Queue<T>::size() {
        return this->m_uSize;
    }

    template<typename T>
    std::size_t Queue<T>::container_size() {
        return this->m_nContainerSize;
    }

    template<typename T>
    unsigned short Queue<T>::usage() {
        return this->m_uContainerUsage;
    }

    template<typename T>
    bool Queue<T>::is_dynamic() {
        return this->m_bIsDynamic;
    }

    template<typename U>
    std::ostream& operator<<(std::ostream& os, const Queue<U>& obj) {
        std::size_t temp = obj.m_uHead;

        if (obj.m_uSize < obj.m_nMinAllowedSize) {
            /* Output all elements */
            for (std::size_t i = 0 ; i < obj.m_uSize; i++) {
                os << obj.m_pContainer[temp] << " ";
                temp = (temp + 1u) % obj.m_nContainerSize;
            }
        }
        else {
            /* Output first 3 elements */
            temp = obj.m_uHead;
            for (std::size_t i = 0u; i < 3; i++) {
                os << obj.m_pContainer[temp] << " ";
                temp = (temp + 1u) % obj.m_nContainerSize;
            }

            os << "... ";

            /* Output last 3 elements */
            temp = obj.m_uTail;

            /* Move 3 index back */
            temp = (temp != 0u) ? (temp - 1u) : obj.m_nContainerSize - 1u;
            temp = (temp != 0u) ? (temp - 1u) : obj.m_nContainerSize - 1u;
            temp = (temp != 0u) ? (temp - 1u) : obj.m_nContainerSize - 1u;

            for (std::size_t i = 0u; i < 3; i++) {
                os << obj.m_pContainer[temp] << " ";
                temp = (temp + 1u) % obj.m_nContainerSize;
            }
        }
        return os;
    }
}