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
        this->m_nTop = 0u;
        this->m_nContainerSize = size;
        this->m_bIsDynamic = true;

        if (this->m_nContainerSize != 0u) {
            this->m_bIsDynamic = false;
        }

        /* Create the initial container */
        if (this->m_bIsDynamic == true) {
            this->m_pContainer = std::make_unique<T[]>(this->m_nGrowSize);
            this->m_nContainerSize = this->m_nGrowSize;
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
    bool Stack<T>::grow() {
        bool retValue = false;

        if (this->m_bIsDynamic) {
            std::size_t newSize = this->m_nContainerSize + this->m_nGrowSize;
            std::unique_ptr<T[]> newContainer = std::make_unique<T[]>(newSize);
            std::copy(
                    this->m_pContainer.get(),
                    this->m_pContainer.get() + this->m_nContainerSize,
                    newContainer.get());

            this->m_nContainerSize = newSize;
            this->m_pContainer = std::move(newContainer);

            retValue = true;
        }

        std::size_t size_bytes = sizeof(T) * this->m_nContainerSize;
        DLOG(INFO) << "Growing in size..." << "New size: " << size_bytes << " Bytes" << std::endl;

        return retValue;
    }


    template<typename T>
    bool Stack<T>::shrink() {
        bool retValue = false;

        if (this->m_bIsDynamic) {
            std::size_t newSize = this->m_nContainerSize - this->m_nGrowSize;

            /* Make sure to not delete old values */
            if (this->m_nTop < newSize) {
                std::unique_ptr<T[]> newContainer = std::make_unique<T[]>(newSize);
                std::copy(
                        this->m_pContainer.get(),
                        this->m_pContainer.get() + newSize,
                        newContainer.get());

                this->m_nContainerSize = newSize;
                this->m_pContainer = std::move(newContainer);

                retValue = true;
            }
        }

        std::size_t size_bytes = sizeof(T) * this->m_nContainerSize;
        DLOG(INFO) << "Shrinking in size..." << "New size: " << size_bytes << " Bytes" << std::endl;

        return retValue;
    }

    template<typename T>
    bool Stack<T>::push(const T& input) {
        bool retValue = false;

        /* Check for remaining space */
        bool isFull = (this->m_nContainerSize <= this->m_nTop + 1);

        if (!isFull) {
            this->m_nTop++;
            this->m_pContainer[this->m_nTop] = input;

            retValue = true;
        }
        else if (isFull && this->m_bIsDynamic) {
            bool result = this->grow();
            if (result) {
                this->m_nTop++;
                this->m_pContainer[this->m_nTop] = input;

                retValue = true;
            }
            else {
                /* Failed to grow in size */
                DLOG(WARNING) << "Failed to grow in size" << std::endl;
            };
        }
        else {
            /* No space left :( */
        }

        return retValue;
    }

    template<typename T>
    bool Stack<T>::pop(T& output) {
        bool retValue = false;

        if (this->m_nTop != 0u) {
            output = this->m_pContainer[this->m_nTop];

            /* TODO: What to do with the previous value? */
            this->m_nTop--;

            retValue = true;
        }

        /* Check used space (percentage) */
        if (retValue && this->m_bIsDynamic) {
            unsigned short usedSpace = this->m_nTop * 100u / this->m_nContainerSize;

            if (usedSpace <= this->m_nShrinkThreshold && this->m_bIsDynamic) {
                bool result = this->shrink();
                if (!result) {
                    /* Failed to shrink in size */
                    DLOG(WARNING) << "Failed to shrink in size" << std::endl;
                }
            }
        }

        return retValue;
    }

    template<typename T>
    std::size_t Stack<T>::size() {
        return this->m_nTop;
    }

    template<typename U>
    std::ostream& operator<<(std::ostream& os, const Stack<U>& obj) {
        if (obj.m_nTop < obj.m_nGrowSize) {
            /* Output all elements */
            for (std::size_t i = 0 ; i <= obj.m_nTop; i++) {
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
            for (std::size_t i = obj.m_nTop - 3; i <= obj.m_nTop; i++) {
                os << obj.m_pContainer[i] << " ";
            }
        }
        return os;
    }

}