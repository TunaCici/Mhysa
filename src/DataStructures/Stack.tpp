//
// Created by Tuna Cici on 25.02.2023.
//

#include "DataStructures/Stack.hpp"

namespace data_struct {
    template<typename T>
    Stack<T>::Stack(unsigned int size, bool dynamic) {
        if (size == std::numeric_limits<unsigned int>::max()) {
            throw std::invalid_argument("Possible integer overflow. Size was set to: " + std::to_string(size));
        }

        this->m_pContainer = nullptr;
        this->m_nContainerSize = size + 1u; /* First element is empty on purpose */
        this->m_uContainerUsage = 0u;

        this->m_uTop = 0u;
        this->m_bIsDynamic = dynamic;

        /* Create the initial container */
        if (this->m_bIsDynamic == true) {
            /* Initial size cannot be smaller than m_nMinAllowedSize */
            if (this->m_nContainerSize <= this->m_nMinAllowedSize) {
                this->m_nContainerSize = this->m_nMinAllowedSize + 1u; /* First element is empty on purpose */
            }

            /* Make sure that the empty container does not shrink below the initial size when 'optimize()' */
            this->m_nMinAllowedSize = this->m_nContainerSize;

            this->m_pContainer = std::make_unique<T[]>(this->m_nContainerSize);
        }
        else {
            this->m_pContainer = std::make_unique<T[]>(this->m_nContainerSize);
        }

        /* Accessing all array elements just in case */
        for (std::size_t idx = 0; idx < this->m_nContainerSize; idx++) {
            T temp = this->m_pContainer[idx];
        }
    }

    template<typename T>
    Stack<T>::~Stack() {
        this->m_pContainer = nullptr;
    }

    template<typename T>
    void Stack<T>::optimize() {
        this->m_uContainerUsage = this->m_uTop * 100u / (this->m_nContainerSize - 1u);
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
                std::copy(
                        this->m_pContainer.get(),
                        this->m_pContainer.get() + optimalContainerSize,
                        newContainer.get());

                this->m_nContainerSize = optimalContainerSize;
                this->m_pContainer = std::move(newContainer);
            }
            catch (const std::exception& e) {
                /* TODO: Do nothing for now */
            }
        }
    }

    template<typename T>
    bool Stack<T>::push(const T& input) {
        bool retValue = false;

        /* Check for remaining space */
        bool isFull = (this->m_nContainerSize <= this->m_uTop + 1);

        if (!isFull) {
            this->m_uTop++;
            this->m_pContainer[this->m_uTop] = input;

            if(this->m_bIsDynamic) {
                this->optimize();
            }

            this->m_uContainerUsage = this->m_uTop * 100u / (this->m_nContainerSize - 1u);

            retValue = true;
        }
        else {
            /* No space left :( */
        }

        return retValue;
    }

    template<typename T>
    bool Stack<T>::pop(T& output) {
        bool retValue = false;

        if (this->m_uTop != 0u) {
            output = this->m_pContainer[this->m_uTop];

            /* TODO: What to do with the previous value? */
            this->m_uTop--;

            if(this->m_bIsDynamic) {
                this->optimize();
            }

            this->m_uContainerUsage = (this->m_uTop + 1u) * 100u / (this->m_nContainerSize - 1u);

            retValue = true;
        }
        else {
            /* Nothing to pop :( */
        }

        return retValue;
    }

    template<typename T>
    std::size_t Stack<T>::size() {
        return this->m_uTop;
    }

    template<typename T>
    std::size_t Stack<T>::container_size() {
        return this->m_nContainerSize - 1u;
    }

    template<typename T>
    unsigned short Stack<T>::usage() {
        return this->m_uContainerUsage;
    }

    template<typename T>
    bool Stack<T>::is_dynamic() {
        return this->m_bIsDynamic;
    }

    template<typename U>
    std::ostream& operator<<(std::ostream& os, const Stack<U>& obj) {
        if (obj.m_uTop < obj.m_nMinAllowedSize) {
            /* Output all elements */
            for (std::size_t i = 0 ; i <= obj.m_uTop; i++) {
                os << obj.m_pContainer[i] << " ";
            }
        }
        else {
            /* Output first 3 elements */
            for (std::size_t i = 0; i <= 3; i++) {
                os << obj.m_pContainer[i] << " ";
            }

            os << "... ";

            /* Output last 3 elements */
            for (std::size_t i = obj.m_uTop - 3; i <= obj.m_uTop; i++) {
                os << obj.m_pContainer[i] << " ";
            }
        }
        return os;
    }

}