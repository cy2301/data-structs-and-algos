#ifndef STACK_H
#define STACK_H

#include "doubly_linked_list.h"
#include <cstddef>
#include <iostream>
#include <stdexcept> 
#include <utility>

template <typename Object>
class Stack {
   private:
      DoublyLinkedList<Object> list;
   public:
      Stack<Object>() : list({}) {}
      Stack(const Stack& other) : list(other.list) {}
      ~Stack() {
         list.clear();
      }
      Stack& operator=(const Stack& other) {
         if (this == &other)
         {
            return *this;
         }
         list.clear();
         list = other.list;
         return *this;
      }
      void push(const Object& other) {
         list.insert(0, other);
      }
      void pop() {
         list.remove(0);
      }
      Object& top() {
         return list[0];
      }
      void printStack() {
         if (list.size() == 0)
         {
            std::cout << "Stack empty" << std::endl;
         }
         list.printList();
      }
};

#endif  // STACK_H