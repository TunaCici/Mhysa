//
// Created by Tuna Cici on 5.03.2023.
//

#ifndef MHYSA_LINKEDLIST_HPP
#define MHYSA_LINKEDLIST_HPP

#include <ostream>
#include <memory>

namespace data_struct {
    enum NodeTypes {
        /* TODO: Implement a 'circular' LinkedList */
        single_link,
        double_link,
    };

    template<typename T>
    class LinkedList {
    private:
        struct Node {
            virtual ~Node() = default;
            T key;
            std::unique_ptr<Node> next;
        };
        struct SLNode : public Node {};
        struct DLNode : public Node {Node* prev;};

        std::unique_ptr<Node> m_pHead{};
        Node* m_pTail{};

        NodeTypes m_eNodeType{};
        std::size_t m_uSize{};

        bool create_node(const T& input, std::unique_ptr<Node>& output);
        int quick_sort_partition(const int& start_idx = 0, const int& end_idx = 0);
        void quick_sort(const int& start_idx = 0, const int& end_idx = 0);
        bool swap_nodes(const std::size_t& idx_a = 0u, const std::size_t& idx_b = 0u);
    public:
        LinkedList(NodeTypes type = NodeTypes::single_link);
        ~LinkedList();

        LinkedList(LinkedList& other) = delete;
        LinkedList& operator=(const LinkedList& other) = delete;

        /* Stack-like operations */
        bool push(const T& input);
        bool pop(T& output);

        /* Queue-like operations */
        bool enqueue(const T& input);
        bool dequeue(T& output);

        /* LinkedList operations */
        bool insert(const T& input = nullptr, const std::size_t& idx = 0u);
        bool remove(const std::size_t& idx = 0u, T& output = nullptr);

        bool search(const T& target, std::size_t& idx);
        bool peek(const std::size_t& idx = 0u, T& target = nullptr);
        bool sort();
        bool reverse();

        std::size_t size();
        NodeTypes type();

        template<typename U>
        friend std::ostream& operator<<(std::ostream& os, const LinkedList<U>& obj);
    };
}

#include "DataStructures/LinkedList.tpp"

#endif
