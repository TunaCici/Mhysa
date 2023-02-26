//
// Created by Tuna Cici on 25.02.2023.
//

#include "glog/logging.h"

#include "DataStructures/Stack.hpp"

namespace data_struct {
    template<typename T>
    Stack<T>::Stack(unsigned int size) {
        DLOG(INFO) << "Constructing..." << std::endl;

        this->m_pContainer = nullptr;
        this->m_nContainerSize = size;
        this->m_uContainerUsage = 0u;

        this->m_uTop = 0u;
        this->m_bIsDynamic = true;

        if (this->m_nContainerSize != 0u) {
            this->m_bIsDynamic = false;
        }

        /* Create the initial container */
        if (this->m_bIsDynamic == true) {
            this->m_pContainer = std::make_unique<T[]>(this->m_nMinAllowedSize);
            this->m_nContainerSize = this->m_nMinAllowedSize;
        }
        else {
            this->m_pContainer = std::make_unique<T[]>(this->m_nContainerSize);
        }

        DLOG(INFO) << "Created an array of type T elements with size: " << this->m_nContainerSize << std::endl;

        /* Accessing all array elements */
        for (std::size_t idx = 0; idx < this->m_nContainerSize; idx++)  {
            this->m_pContainer[idx] = 0;
            auto temp = this->m_pContainer[idx];
        }

        DLOG(INFO) << "Validated the newly created array" << std::endl;
    }

    template<typename T>
    Stack<T>::~Stack() {
        DLOG(INFO) << "Deconstructing..." << std::endl;
    }

    template<typename T>
    void Stack<T>::optimize() {
        this->m_uContainerUsage = this->m_uTop * 100u / this->m_nContainerSize;
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

        if (optimalContainerSize <= this->m_nMinAllowedSize){
            /* Do not shrink below a certain size */
        }
        else if (optimalContainerSize != this->m_nContainerSize) {
            try{
                std::unique_ptr<T[]> newContainer = std::make_unique<T[]>(optimalContainerSize);
                std::copy(
                        this->m_pContainer.get(),
                        this->m_pContainer.get() + optimalContainerSize,
                        newContainer.get());

                this->m_nContainerSize = optimalContainerSize;
                this->m_pContainer = std::move(newContainer);

                /* Update container usage value */
                this->m_uContainerUsage = this->m_uTop * 100u / this->m_nContainerSize;

                std::size_t size_bytes = sizeof(T) * this->m_nContainerSize;
                DLOG(INFO) << "Resizing to: " << size_bytes << " Bytes" << std::endl;
            }
            catch (const std::exception& e) {
                DLOG(ERROR) << "Error occurred while resizing container: " << e.what() << std::endl;
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

            this->optimize();
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

            this->optimize();
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
    unsigned short Stack<T>::usage() {
        return this->m_uContainerUsage;
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