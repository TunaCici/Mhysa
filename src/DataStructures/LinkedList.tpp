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
    bool LinkedList<T>::create_node(const T& input, std::unique_ptr<Node>& output) {
        bool retValue = false;

        /* Create a new node */
        try {
            if (this->m_eNodeType == NodeTypes::single_link) {
                output = std::make_unique<SLNode>();
                output->next = nullptr;
            }
            else {
                output = std::make_unique<DLNode>();
                static_cast<DLNode*>(output.get())->prev = nullptr;
                output->next = nullptr;
            }

            output->key = input;

            this->m_uSize++;
            retValue = true;
        }
        catch (const std::exception& e) {
            /* TODO: Do nothing for now */
        }

        return retValue;
    }

    template<typename T>
    bool LinkedList<T>::swap_nodes(size_t idx_a, size_t idx_b) {
        bool retValue = false;

        /* Check if both indexes are within range */
        if (this->m_uSize <= idx_a || this->m_uSize <= idx_b) {
            return retValue;
        }

        /* Apparently I can't std::swap an std::unique_ptr with a nullptr */
        /* BUG: When idx_a == m_uSize -1 and idx_b == m_uSize - 2 I get the above error */
        /* The below line makes sure idx_a is always smaller so that the above bug does not occur */
        /* Because of this 'fix', parameters can't be 'const' :( */
        if (idx_b < idx_a) {
            std::swap(idx_a, idx_b);
        }

        /* Nodes are the same */
        if (idx_a == idx_b) {
            retValue = true;
        }
        /* One of them is head */
        else if (idx_a == 0u || idx_b == 0u) {
            const std::size_t notHeadIdx = std::max(idx_a, idx_b);

            /* Iterate until you reach the parent of notHead */
            /* Not really parent, but its previous node :/ */
            /* TODO: Make use of double_link's prev pointer? */
            Node* parent = this->m_pHead.get();
            for (std::size_t iterCount = 0u; iterCount < notHeadIdx - 1u; iterCount++) {
                parent = parent->next.get();
            }

            /* Temporarily hold the std::unique_ptr that is the head */
            Node* tempPtr = this->m_pHead.get();

            /* Swap the 'next' values */
            std::swap(this->m_pHead, parent->next);
            std::swap(this->m_pHead->next, tempPtr->next);

            /* Swap the 'prev' values if possible */
            if (this->m_eNodeType != NodeTypes::single_link) {
                std::swap(
                    static_cast<DLNode*>(this->m_pHead.get())->prev,
                    static_cast<DLNode*>(tempPtr)->prev
                );
            }

            /* Finally update the tail */
            if (notHeadIdx == this->m_uSize - 1u) {
                this->m_pTail = tempPtr;
            }

            retValue = true;
        }
        else {
            /* Iterate until you reach the parent of idx_a */
            /* Not really parent, but its previous node :/ */
            /* TODO: Make use of double_link's prev pointer? */
            Node* parent_a = this->m_pHead.get();
            for (std::size_t iterCount = 0u; iterCount < idx_a - 1u; iterCount++) {
                parent_a = parent_a->next.get();
            }

            /* Iterate until you reach the parent of idx_b */
            /* Not really parent, but its previous node :/ */
            /* TODO: Make use of double_link's prev pointer? */
            Node* parent_b = this->m_pHead.get();
            for (std::size_t iterCount = 0u; iterCount < idx_b - 1u; iterCount++) {
                parent_b = parent_b->next.get();
            }

            /* Swap the 'next' values */
            std::swap(parent_a->next, parent_b->next);
            if (parent_b->next->next == nullptr) {
                parent_b->next->next = std::move(parent_a->next->next);
            }
            else {
                std::swap(parent_a->next->next, parent_b->next->next);
            }

            /* Swap the 'prev' values if possible */
            if (this->m_eNodeType != NodeTypes::single_link) {
                std::swap(
                    static_cast<DLNode*>(parent_a->next.get())->prev,
                    static_cast<DLNode*>(parent_b)->prev
                );
            }

            /* Finally update the tail */
            if (idx_a == this->m_uSize - 1u) {
                this->m_pTail = parent_b->next.get();
            }
            else if (idx_b == this->m_uSize - 1u) {
                this->m_pTail = parent_a->next.get();
            }

            retValue = true;
        }


        return retValue;
    }

    template<typename T>
    bool LinkedList<T>::push(const T& input) {
        bool retValue = false;

        std::unique_ptr<Node> newNode;
        retValue = this->create_node(input, newNode);

        /* Failed to create a new node */
        if (!retValue) {
            return retValue;
        }

        /* Push it into the LinkedList */
        if (this->m_pHead != nullptr) {
            std::swap(this->m_pHead, newNode); /* 'newNode' becomes the old head */
            this->m_pHead->next = std::move(newNode); /* head's next now points to the old head */

            if (this->m_eNodeType != NodeTypes::single_link) {
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

            /* FYI: Popped node is auto-released by the std::unique_ptr */

            if (this->m_uSize != 0u) {
                this->m_uSize--;
            }
            retValue = true;
        }

        return retValue;
    }

    template<typename T>
    bool LinkedList<T>::enqueue(const T& input) {
        bool retValue = false;

        std::unique_ptr<Node> newNode;
        retValue = this->create_node(input, newNode);

        /* Failed to create a new node */
        if (!retValue) {
            return retValue;
        }

        /* Enqueue it into the LinkedList */
        if (this->m_pTail != nullptr) {
            if (this->m_eNodeType != NodeTypes::single_link) {
                static_cast<DLNode*>(newNode.get())->prev = this->m_pTail;
            }

            this->m_pTail->next = std::move(newNode);
            this->m_pTail = this->m_pTail->next.get();
        }
        else {
            this->m_pHead = std::move(newNode);
            this->m_pTail = this->m_pHead.get();
        }

        return retValue;
    }

    template<typename T>
    bool LinkedList<T>::dequeue(T& output) {
        bool retValue = false;

        /* Dequeue from the head (same as popping out) */
        retValue = this->pop(output);

        return retValue;
    }

    template<typename T>
    bool LinkedList<T>::insert(const T& input, const std::size_t& idx) {
        bool retValue = false;

        /* Check if idx is within range */
        if (this->m_uSize < idx) {
            return retValue;
        }

        /* Head */
        if (idx == 0u) {
            retValue = this->push(input);
        }
        /* Tail */
        else if (idx == this->m_uSize) {
            retValue = this->enqueue(input);
        }
        /* Middle */
        else {
            std::unique_ptr<Node> newNode;
            retValue = this->create_node(input, newNode);

            /* Failed to create a new node */
            if (!retValue) {
                return retValue;
            }

            /* Iterate until you reach the target - 1 */
            Node* target = this->m_pHead.get();
            for (std::size_t iterCount = 0u; iterCount < idx - 1u; iterCount++) {
                target = target->next.get();
            }

            std::swap(target->next, newNode); /* newNode is now at target idx */
            std::swap(target->next->next, newNode); /* Update newNode's next pointer */

            if (this->m_eNodeType != NodeTypes::single_link) {
             static_cast<DLNode*>(target->next.get())->prev = target;
             static_cast<DLNode*>(target->next->next.get())->prev = target->next.get();
            }
        }

        return retValue;
    }

    template<typename T>
    bool LinkedList<T>::remove(const std::size_t& idx, T& output) {
        bool retValue = false;

        /* Check if idx is within range */
        if (this->m_uSize <= idx) {
            return retValue;
        }

        /* Head */
        if (idx == 0u) {
            retValue = this->pop(output);
        }
        /* Middle or Tail */
        else {
            /* Iterate until you reach the target - 1 */
            Node* target = this->m_pHead.get();
            for (std::size_t iterCount = 0u; iterCount < idx - 1u; iterCount++) {
                target = target->next.get();
            }

            output = target->next->key;

            target->next = std::move(target->next->next); /* target gets auto destroyed thanks to std::unique_ptr */

            if (this->m_eNodeType != NodeTypes::single_link && target->next != nullptr) {
                static_cast<DLNode*>(target->next.get())->prev = target;
            }

            if (this->m_uSize != 0u) {
                this->m_uSize--;
            }
            retValue = true;
        }

        return retValue;
    }

    template<typename T>
    bool LinkedList<T>::search(const T& target, std::size_t& idx) {
        bool retValue = false;

        /* TODO: Make use of double_link's prev pointer? */
        Node* iter = this->m_pHead.get();
        std::size_t iterCount = 0u;
        for (iterCount = 0u; iterCount < this->m_uSize; iterCount++) {
            if (iter->key == target) {
                idx = iterCount;
                retValue = true;
                break;
            }

            iter = iter->next.get();
        }

        return retValue;
    }

    template<typename T>
    bool LinkedList<T>::peek(const std::size_t& idx, T& target) {
        bool retValue = false;

        /* Check if idx is within range */
        if (this->m_uSize <= idx) {
            return retValue;
        }

        /* TODO: Make use of double_link's prev pointer? */
        Node* iter = this->m_pHead.get();
        for (std::size_t iterCount = 0u; iterCount < idx; iterCount++) {
            iter = iter->next.get();
        }

        target = iter->key;
        retValue = true;

        return retValue;
    }

    template<typename T>
    int LinkedList<T>::quick_sort_partition(const int& start_idx, const int& end_idx) {
        T pivot;
        this->peek(end_idx, pivot); // TODO: How to choose pivot? Median-of-3 or etc.
        int first_idx = start_idx - 1;

        /* Compare each variable with the pivot */
        /* Put the variable either in the 1st partition(first_idx) or the 2nd partition (second_idx) */
        for (int second_idx = start_idx; second_idx <= end_idx - 1; second_idx++) {
            T output;
            this->peek(second_idx, output);

            if (output < pivot) {
                first_idx++;
                this->swap_nodes(first_idx, second_idx);
            }
        }

        /* Swap the pivot so that it lies between the two partitions */
        first_idx++;
        this->swap_nodes(first_idx, end_idx);

        return first_idx;
    }

    template<typename T>
    void LinkedList<T>::quick_sort(const int& start_idx, const int& end_idx) {
        if (start_idx < end_idx) {
            std::size_t pivot = this->quick_sort_partition(start_idx, end_idx);

            /* Sort pivot's both left and right sub-array */
            this->quick_sort(start_idx, pivot - 1u);
            this->quick_sort(pivot + 1, end_idx);
        }
    }

    template<typename T>
    bool LinkedList<T>::sort() {
        bool retValue = false;

        /* Limitation(s) */
        /* 1. Time complexity of avg. O(NLOG(N)) */
        /* 2. Space complexity of O(1) (No heap allocation. Assume stack is unlimited) */
        /* 3. Operations can be done using only the nodes. (Swapping data is not allowed) */

        /* Design choice(s) */
        /* 1. Quick sort will be used */
        /* 2. List will be sorted in ascending order */

        if (this->m_uSize <= 1u) {
            retValue = true;
        }
        else {
            this->quick_sort(0u, this->m_uSize - 1u);

            /* Check if ordered */
            /* Iterate until you reach the target - 1 */
            Node* target = this->m_pHead.get();
            std::size_t iterCount = 0u;
            T prevKey = target->key;

            for (iterCount = 1u; iterCount < this->m_uSize; iterCount++) {
                target = target->next.get();

                if (target && (target->key < prevKey)) {
                    break;
                }
            }

            if (iterCount == this->m_uSize) {
                retValue = true;
            }
        }

        return retValue;
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

        /* Output size */
        os << "[n=" << obj.m_uSize << "] ";

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