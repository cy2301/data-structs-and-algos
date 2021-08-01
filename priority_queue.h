#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <functional>
#include <vector>
#include <iostream>
#include "heap.h"

using std::cout, std::endl;

template <class Comparable, class Container=std::vector<Comparable>, class Compare=std::less<typename Container::value_type>>
class PriorityQueue {
    Compare compare; // less than greater than etc
    Container container; // container
    size_t _size;
    public:
        PriorityQueue<Comparable,Container,Compare>() : compare(), container(), _size(0) {
            heapify(&container, compare);
        }
        explicit PriorityQueue(const Compare& comp) : compare (comp), container(), _size(0) {
            heapify(&container, compare);
        }
        explicit PriorityQueue(const Container& givenContainer) : compare(), container(givenContainer), _size(givenContainer.size()) {
            heapify(&container, compare);
        }
        PriorityQueue(const Compare& comp, const Container& givenContainer) : compare(comp), container(givenContainer), _size(givenContainer.size()){
            heapify(&container,compare);
        }
        PriorityQueue(const PriorityQueue& other) : compare(other.compare), container(other.container), _size(other._size) {}
        PriorityQueue& operator=(const PriorityQueue& other)
        {
            if (this == &other)
            {
                return *this;
            }
            compare = other.compare;
            container = other.container;
            _size = other._size;
            return *this;
        }
        ~PriorityQueue()
        {
            container.clear();
        }
        typename Container::const_reference top() const {
            return heap_get_min(container);
        }
        bool empty() const {
            if (container.size() == 1 || container.empty())
            {
                return true;
            }
            return false;
        }
        size_t size() const {
            return _size;
        }
        void make_empty() { // question: does that include the 0th index
            _size = 0;
            container.clear();
            container.push_back(typename Container::value_type());
        }
        void push(const typename Container::value_type& value) {
            heap_insert(&container,value,compare);
            _size++;
        }
        void pop() {
            if (empty())
            {
                return;
            }
            heap_delete_min(&container,compare);
            _size--;
        }
        void print_queue(std::ostream& os =std::cout) const {
            if (empty())
            {
                os << "<empty>\n";
                return;
            }
            os << container.at(1);
            for (size_t i = 2; i < container.size(); i++)
            {
                os << ", " << container.at(i);
            }
            os << "\n";
        }
};

#endif  // PRIORITY_QUEUE_H