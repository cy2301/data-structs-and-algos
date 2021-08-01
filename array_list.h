#ifndef ARRAY_LIST_H
#define ARRAY_LIST_H

#include <cstddef>
#include <iostream>
#include <stdexcept> 
#include <utility>

template <typename Object>
class ArrayList {
    size_t _size; // size
    size_t _capacity;
	Object* array; // dynamic array of objects

    public:
        ArrayList<Object>() : _size(0), _capacity(1), array(new Object[1]) {}
        explicit ArrayList<Object>(size_t s) : _size(0), _capacity(s), array(new Object[s]) {}

        ArrayList(const ArrayList& other) : _size(other._size), _capacity(other._capacity), array(new Object[other._capacity]) 
        {
            for (size_t i = 0; i < _size; i++)
            {
                array[i] = other.array[i];
            }
        }

        ArrayList& operator=(const ArrayList& other)
        {
            if (this == &other)
            {
                return *this;
            }
            _size = other._size;
            _capacity = other._capacity;
            delete[] array;
            array = new Object[_capacity];
            for (size_t i = 0; i < _size; i++)
            {
                array[i] = other.array[i];
            }
            
            return *this;
        }

        ~ArrayList()
        {
            _size = 0;
            _capacity = 0;
            delete[] array;
        }

        size_t size()
        {
            return _size;
        }

        Object& operator[](size_t index)
        {
            if (index >= _size)
            {
                throw std::out_of_range("");
            }
            return array[index];
        }

        void insert(size_t index, const Object& other)
        {
            if (index > _size) {
                throw std::out_of_range("");
            }
            if (_size == _capacity)
            {
                if (_capacity == 0)
                {
                    _capacity = 1;
                }
                _capacity *= 2;
                Object* tempList = new Object[_capacity];
                for (size_t i = 0; i < index; i++)
                {
                    tempList[i] = array[i];
                }
                for (size_t i = index + 1; i < _size + 1; i++)
                {
                    tempList[i] = array[i-1];
                }
                tempList[index] = other;
                delete[] array;
                array = tempList;
                _size++;
                return;
            }
            for (size_t i = _size; i > index; i--)
            {
                array[i] = array[i-1];
            }
            array[index] = other;
            _size++;
        }

        void remove(size_t index) 
        {
            if (_size == 0 || array == nullptr || _capacity == 0)
            {
                throw std::out_of_range("");
            }
            if (index >= _size)
            {
                throw std::out_of_range("");
            }
            for (size_t i = index; i < _size - 1; i++)
            {
                array[i] = array[i+1];
            }
            _size--;
        }

        void printList() {
            for (size_t i = 0; i < _size; i++)
            {
                std::cout << "[" << array[i] << "]";
            }
            std::cout << std::endl;
        }
};

#endif  // ARRAY_LIST_H