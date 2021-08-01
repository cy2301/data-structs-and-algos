#ifndef DOUBLY_LINKED_LIST_H
#define DOUBLY_LINKED_LIST_H

#include <cstddef>
#include <iostream>
#include <stdexcept> 
#include <utility>

template <typename Object>
class DoublyLinkedList {
    private:
        struct Node {
            Object data;
            Node* next;
            Node* prev;
        };
        size_t _size;
        Node* _head;
        Node* _tail;
        Node* _marker;
        Node* _shadow;
    public:
        DoublyLinkedList <Object>() : _size(0), _head(nullptr), _tail(nullptr), _marker(nullptr), _shadow(nullptr) {}
        DoublyLinkedList(const DoublyLinkedList& other) : _size(other._size), _head(nullptr), _tail(nullptr), _marker(nullptr), _shadow(nullptr) 
        {
            if (_size == 0)
            {
                return;
            }
            Node* newHead = new Node;
            newHead->data = other._head->data;
            newHead->prev = nullptr;
            _head = newHead;
            _head->next = nullptr;

            _marker = _head;
            Node* secondMarker = other._head;
            while (secondMarker->next != nullptr)
            {
                secondMarker = secondMarker->next;
                Node* newNode = new Node;
                newNode->data = secondMarker->data;
                newNode->next = nullptr;
                newNode->prev = _marker;
                _marker->next = newNode;
                _marker = newNode;
            }
            _tail = _marker;
        }
        ~DoublyLinkedList() {
            clear();
        }
        void clear() { // Destructor helper
            _marker = _head;
            while (_head != nullptr)
            {
                _marker = _marker->next;
                delete _head;
                _head = _marker;
                _size--;
            }
        }
        DoublyLinkedList& operator=(const DoublyLinkedList& other)
        {
            if (this == &other)
            {
                return *this;
            }
            
            this->clear();
            _head = nullptr;
            _tail = nullptr;
            _marker = nullptr;
            _shadow = nullptr;
            if (other._size == 0) // Setting it equal to empty list
            {
                _size = other._size;
                return *this;
            }
            _size = other._size;

            Node* newHead = new Node;
            newHead->data = other._head->data;
            newHead->prev = nullptr;
            _head = newHead;
            _head->next = nullptr;

            _marker = _head;
             Node* secondMarker = other._head;
            while (secondMarker->next != nullptr)
            {
                secondMarker = secondMarker->next;
                Node* newNode = new Node;
                newNode->data = secondMarker->data;
                newNode->next = nullptr;
                newNode->prev = _marker;
                _marker->next = newNode;
                _marker = newNode;
            }
            _tail = _marker;
            return *this;
        }
        size_t size() {
            return _size;
        }
        Object& operator[](size_t index) {
            if (index >= _size || index < 0)
            {
                throw std::out_of_range("");
            }
            if (index == 0)
            {
                return _head->data;
            }
            size_t i;
            if ((_size - 1) - index >= (_size / 2)) { //start at head
                _marker = _head;
                i = 0;
                while (i != index) {
                    i++;
                    _marker = _marker->next;
                }
                return _marker->data;
            }
            else { //start at tail
                _marker = _tail;
                i = _size - 1;
                while (i != index)
                {
                    i--;
                    _marker = _marker->prev;
                }
                return _marker->data;
            }
        }

        void insert(size_t index, const Object& other)
        {
            if (index > _size)
            {
                throw std::out_of_range("");
            }
            // account for index == 0
            if (_head == nullptr)
            {
                Node* newNode = new Node;
                newNode->data = other;
                newNode->prev = nullptr;
                newNode->next = nullptr;
                _head = newNode;
                _tail = newNode;
                _size++;
                return;
            }
            if (index == 0)
            {
                Node* newNode = new Node;
                newNode->data = other;
                newNode->prev = nullptr;
                newNode->next = _head;
                _head->prev = newNode;
                _head = newNode;
                _size++;
                return;
            }
            if (index == _size) // adding a tail
            {
                _marker = _tail;
                Node* newNode = new Node;
                newNode->data = other;
                newNode->prev = _marker;
                newNode->next = nullptr;
                _marker->next = newNode;
                _tail = newNode;
                _size++;
                return;
            }
            _marker = _head;
            size_t i = 1;
            while (i != index)
            {
                i++;
                _marker = _marker->next;
            }
            Node* newNode = new Node;
            newNode->data = other;
            newNode->next = _marker->next;
            newNode->prev = _marker;
            _marker->next->prev = newNode;
            _marker->next = newNode;
            _size++;
        }

        void remove(size_t index)
        {
            if (_size == 0 || _head == nullptr)
            {
                throw std::out_of_range("");
            } 
            if (index >= _size)
            {
                throw std::out_of_range("");
            }
            _marker = _head;
            if (index == 0 && _size == 1) // if the head is the only one
            {
                delete _head;
                _head = nullptr;
                _size--;
                return;
            }
            if (index == 0) // deleting head
            {
                _marker = _marker->next;
                _marker->prev = nullptr;
                delete _head;
                _head = _marker;
                _size--;
                return;
            }
            if (index == _size - 1) // deleting tail
            {
                _marker = _tail->prev;
                _marker->next = nullptr;
                _tail->prev = nullptr;
                delete _tail;
                _tail = _marker;
                _size--;
                return;
            }
            size_t i = 1;
            _marker = _head->next;
            while (i != index)
            {
                i++;
                _marker = _marker->next;
            }
            _marker->prev->next = _marker->next;
            _marker->next->prev = _marker->prev;
            _marker->next = nullptr;
            _marker->prev = nullptr;
            delete _marker;
            _size--;
        }
        void printList() {
            _marker = _head;
            if (_marker == nullptr) {
                std::cout << "Linked list empty" << std::endl;
                return;
            }

            while (_marker != nullptr) {
                std::cout << "[ " << _marker->data << " ]" << std::endl;
                _marker = _marker->next;
            }
        }
        void printListRev() {
            _marker = _tail;
            if (_marker == nullptr) {
                std::cout << "Linked list empty" << std::endl;
                return;
            }

            while (_marker != nullptr) {
                std::cout << "[ " << _marker->data << " ]" << std::endl;
                _marker = _marker->prev;
            }
        }
};

#endif  // DOUBLY_LINKED_LIST_H
