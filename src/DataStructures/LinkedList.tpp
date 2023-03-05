//
// Created by Tuna Cici on 5.03.2023.
//

#include "DataStructures/LinkedList.hpp"

namespace data_struct {
    template<typename T>
    LinkedList<T>::LinkedList(NodeTypes type) {
        this->m_pHead = nullptr;
        this->m_pTail = nullptr;

        this->m_eNodeType = type;

        this->m_uSize = 0u;
    }

    template<typename T>
    LinkedList<T>::~LinkedList() {
        /* std::unique_ptr releases data automatically */
        /* TODO: Possible recursion/stack-over-flow!! Manually release them */
        this->m_pHead = nullptr;
    }

    template<typename T>
    bool LinkedList<T>::push(const T& input) {
        bool retValue = false;

        std::unique_ptr<Node> newNode;

        try {
            if (this->m_eNodeType == NodeTypes::single_link) {
                newNode = std::make_unique<SLNode>();
                newNode->next = nullptr;
            }
            else {
                newNode = std::make_unique<DLNode>();
                static_cast<DLNode*>(newNode.get())->prev = nullptr;
                newNode->next = nullptr;
            }

            newNode->key = input;

            this->m_uSize++;
            retValue = true;
        }
        catch (const std::exception& e) {
            /* TODO: Do nothing for now */
        }

        if (this->m_pHead != nullptr) {
            std::swap(this->m_pHead, newNode); /* 'newNode' becomes the old head */
            this->m_pHead->next = std::move(newNode); /* head's next now points to old head */

            if (this->m_eNodeType == NodeTypes::double_link) {
                static_cast<DLNode*>(this->m_pHead->next.get())->prev = this->m_pHead.get();
            }

            if (this->m_pTail == nullptr) {
                this->m_pTail = this->m_pHead->next.get();
            }
        }
        else {
            this->m_pHead = std::move(newNode);
            this->m_pTail = this->m_pHead.get();
        }

        return retValue;
    }

    template<typename T>
    bool LinkedList<T>::pop(T& output) {
        bool retValue = false;

        if (this->m_pHead != nullptr) {
            output = this->m_pHead->key;
            this->m_pHead = std::move(this->m_pHead->next);

            this->m_uSize--;
            retValue = true;
        }

        return retValue;
    }

    template<typename T>
    bool LinkedList<T>::enqueue(const T& input) {
        return false;
    }

    template<typename T>
    bool LinkedList<T>::dequeue(T& output) {
        return false;
    }

    template<typename T>
    bool LinkedList<T>::insert(const std::size_t idx, const T& input) {
        return false;
    }

    template<typename T>
    bool LinkedList<T>::remove(const std::size_t idx, const T& output) {
        return false;
    }

    template<typename T>
    bool LinkedList<T>::search(const T& target, std::size_t idx) {
        return false;
    }

    template<typename T>
    bool LinkedList<T>::peek(const std::size_t idx) {
        return false;
    }

    template<typename T>
    bool LinkedList<T>::sort() {
        return false;
    }

    template<typename T>
    bool LinkedList<T>::reverse() {
        return false;
    }

    template<typename T>
    std::size_t LinkedList<T>::size() {
        return this->m_uSize;
    }

    template<typename T>
    NodeTypes LinkedList<T>::type() {
        return this->m_eNodeType;
    }

    template<typename U>
    std::ostream& operator<<(std::ostream& os, const LinkedList<U>& obj) {
        const unsigned short printThreshold = 32u;

        /* Output all elements */
        if (obj.m_uSize < printThreshold) {
            for (auto iter = obj.m_pHead.get(); iter != nullptr; iter = iter->next.get()) {
                os << iter->key << " ";
            }
        }
        else {
            /* Output first 3 elements */
            auto current = obj.m_pHead.get();

            os << current->key << " ";
            current = current->next.get();
            os << current->key << " ";
            current = current->next.get();
            os << current->key << " ";

            os << "... ";

            /* Temporarily save the last 3 elements */
            U lastThree[3];

            if (obj.m_eNodeType == NodeTypes::single_link) {
                current = obj.m_pHead.get();

                for (std::size_t i = 0u; i < obj.m_uSize - 3u; i++) {
                    current = current->next.get();
                }

                lastThree[0] = current->key;
                current = current->next.get();
                lastThree[1] = current->key;
                current = current->next.get();
                lastThree[2] = current->key;
                current = current->next.get();

            }
            else {
                current = obj.m_pTail;

                lastThree[2] = current->key;
                current = static_cast<typename LinkedList<U>::DLNode *>(current)->prev;
                lastThree[1] = current->key;
                current = static_cast<typename LinkedList<U>::DLNode *>(current)->prev;
                lastThree[0] = current->key;
            }

            /* Output last 3 elements  */
            for (auto & i : lastThree) {
                os << i << " ";
            }
        }

        return os;
    }
}