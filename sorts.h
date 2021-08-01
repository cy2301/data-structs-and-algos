#ifndef SORTS_H
#define SORTS_H

#include <functional>
#include <iostream>
#include <vector>
#include <utility>
#include "heap.h"

using std::cout, std::endl, std::vector;

// Pretty printing for vectors of Printable objects (have ostream operator<< defined)
// Be careful: modifications risk failing tests
template <class Printable>
std::ostream& operator<<(std::ostream& os, const std::vector<Printable>& container) {
    os << "[";
    bool first = false;
    for (const Printable& t : container) {
        if (first) {
            os << ", ";
        }
        if (typeid(t) == typeid(std::string)) {
            os << "\"" << t << "\"";
        } else {
            os << t;
        }
        first = true;
    }
    os << "]";
    return os;
}

template <class Comparable>
void selection_sort(std::vector<Comparable>& container) {
    // print the initial container
    std::cout << container << std::endl;
    
    if (container.empty()) {
        return;
    }

    for (size_t index = 0; index < container.size()-1; index++) {
        // do 1 pass of selection sort: find the min and swap it to the front
        size_t index_min = index;
        for (size_t i = index+1; i < container.size(); i++) {
            if (container[i] < container[index_min]) {
                index_min = i;
            }
        }
        Comparable t = container[index];
        container[index] = container[index_min];
        container[index_min] = t;
        
        // print the container after each pass
        std::cout << container << std::endl;
    }
}

template <class Comparable>
void insertion_sort(std::vector<Comparable>& container) {
    std::cout << container << std::endl;

    if (container.empty())
    {
        return;
    }

    for(size_t i = 1; i < container.size(); i++)
    {
        Comparable temp = container[i];
        size_t j = i;
        while (j > 0 && temp < container[j-1])
        {
            container[j] = container[j-1];
            j--;
        }
        container[j] = temp;
        cout << container << endl;
    }
}

size_t power(size_t x, size_t y)
{
    if (y == 0)
    {
        return 1;
    }
    size_t result = 1;
    for (size_t i = 0; i < y; i++)
    {
        result *= x;
    }
    return result;
}

size_t hibbard(size_t n)
{
    vector<size_t> hibbardArray;
    size_t hibbardIndex = 0;
    for (size_t i = 1; i <= n; i++)
    {
        hibbardArray.push_back(power(2,i)-1);
    }
    for (size_t i = 0; i < hibbardArray.size(); i++)
    {
        if (hibbardArray.at(i) >= n)
        {
            hibbardIndex = i;
            break;
        }
    }
    return hibbardIndex;
}

template <class Comparable>
void shell_sort(std::vector<Comparable>& container) {
    cout << container << endl;
    if (container.empty())
    {
        return;
    }
    size_t k = hibbard(container.size());
    size_t hibbardNum = power(2,k)-1;
    while (k > 0)
    {
        for (size_t i = hibbardNum; i < container.size(); i++)
        {
            Comparable temp = container.at(i);
            size_t j = i;

            for (; j >= hibbardNum && temp < container.at(j-hibbardNum); j -= hibbardNum)
            {
                container[j] = container[j-hibbardNum];
            }
            container[j] = temp;
        }
        k -= 1;
        hibbardNum = power(2,k)-1;
        cout << container << endl;
    }
}

template <class Comparable>
void heap_sort(std::vector<Comparable>& container) {
    cout << container << endl;
    if (container.empty())
    {
        return;
    }
    if (container.size() == 1)
    {
        return;
    }
    heapify(&container);
    vector<Comparable> sortedVector;
    cout << container << endl;
    while(container.size() != 1)
    {
        sortedVector.push_back(heap_get_min(container));
        heap_delete_min(&container);
        cout << container << endl;
        cout << sortedVector << endl;
    }
    std::swap(container,sortedVector);
}

template <class Comparable>
void merge(vector<Comparable>& container, vector<Comparable>& tmpArray, int leftPos, int rightPos, int rightEnd)
{
    int leftEnd = rightPos - 1;
    int tmpPos = leftPos;
    int numElements = rightEnd - leftPos + 1;

    while(leftPos <= leftEnd && rightPos <= rightEnd)
        if(container[leftPos] <= container[rightPos])
            tmpArray[tmpPos++] = std::move(container[leftPos++]);
        else
            tmpArray[tmpPos++] = std::move(container[rightPos++]);

    while(leftPos <= leftEnd)
        tmpArray[tmpPos++] = std::move(container[leftPos++]);

    while(rightPos <= rightEnd)
        tmpArray[tmpPos++] = std::move(container[rightPos++]);

    for(int i = 0; i < numElements; ++i, --rightEnd)
        container[rightEnd] = std::move(tmpArray[rightEnd]);
}

template <class Comparable>
void mergeSort(vector<Comparable>& container, vector<Comparable> & tmpArray, int left, int right)
{
    if(left < right)
    {
        int center = (left + right)/2;
        mergeSort(container, tmpArray, left, center);
        mergeSort(container, tmpArray, center+1, right);
        merge(container, tmpArray, left, center+1, right);
        cout << container << endl;
    }
}

template <class Comparable>
void merge_sort(vector<Comparable> & container)
{
    cout << container << endl;
    if (container.empty())
    {
        return;
    }
    if (container.size() == 1)
    {
        return;
    }
    vector<Comparable> tmpArray(container.size());
    mergeSort(container, tmpArray, 0, container.size()-1);
}

void bucket_sort(std::vector<unsigned>& container) {
    cout << container << endl;
    if (container.empty())
    {
        return;
    }
    if (container.size() == 1)
    {
        return;
    }
    unsigned max = container.at(0);
    for (size_t i = 0; i < container.size(); i++)
    {
        if (container.at(i) > max)
        {
            max = container.at(i);
        }
    }
    vector<unsigned> bucketVector (max + 1);
    for (size_t i = 0; i < container.size(); i++)
    {
        bucketVector.at(container.at(i)) += 1;
    }
    cout << bucketVector << endl;
    container.clear();
    for (size_t i = 0; i < bucketVector.size(); i++)
    {
        for (unsigned j = 0; j < bucketVector.at(i); j++)
        {
            container.push_back(i);
        }
        if (bucketVector.at(i) != 0)
            cout << container << endl;
    }
}

#endif  // SORTS_H