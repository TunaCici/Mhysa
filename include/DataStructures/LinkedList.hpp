//
// Created by Tuna Cici on 5.03.2023.
//

#ifndef MHYSA_LINKEDLIST_HPP
#define MHYSA_LINKEDLIST_HPP

#include <ostream>
#include <memory>

namespace data_struct {
    enum NodeTypes {
        single_link,
        double_link,
        circular
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

        struct DLNode : public Node {
            Node* prev;
        };

        std::unique_ptr<Node> m_pHead{};
        Node* m_pTail{};

        NodeTypes m_eNodeType{};

        std::size_t m_uSize{};
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
        bool insert(const std::size_t idx = 0u, const T& input = nullptr);
        bool remove(const std::size_t idx = 0u, const T& output = nullptr);

        bool search(const T& target = nullptr, std::size_t idx = 0u);
        bool peek(const std::size_t idx = 0u);
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
