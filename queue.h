#ifndef QUEUE_H
#define QUEUE_H

#include "doubly_linked_list.h"
#include <cstddef>
#include <iostream>
#include <stdexcept> 
#include <utility>

template <typename Object>
class Queue {
    private:
        DoublyLinkedList<Object> list;
    public:
        Queue<Object>() : list({}) {}
        Queue(const Queue& other) : list(other.list) {}
        ~Queue() {
            list.clear();
        }
        Queue& operator=(const Queue& other) {
            if (this == &other)
            {
                return *this;
            }
            list.clear();
            list = other.list;
            return *this;
        }
        void enqueue(const Object& other) {
            list.insert(list.size(),other);
        }
        Object dequeue() {
            Object first = list[0];
            list.remove(0);
            return first;
        }
        Object& front() {
            return list[0];
        }
        void printQueue() {
            if (list.size() == 0)
            {
                std::cout << "Queue empty" << std::endl;
            }
            list.printList();
        }

};

#endif  // QUEUE_H
