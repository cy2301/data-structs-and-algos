#ifndef HEAP_H
#define HEAP_H

#include <functional>
#include <deque> 
#include <initializer_list> 
#include <iostream> 
#include <sstream> 
#include <stdexcept> 
#include <vector>

using std::vector, std::cout, std::endl;

template <class Container, class Compare=std::less<typename Container::value_type>>
void percolateDown (size_t holeIndex, Container* container, Compare comp=std::less<typename Container::value_type>{}) // TOFIX: 80 and 60 didn't get swapped. Also alternaive to adjusting hole index? maybe insert 0
{
    size_t childIndex;
    typename Container::value_type temp = container->at(holeIndex);
    //cout << "Temp is: " << temp << endl;
    while (holeIndex * 2 <= container->size()-1) // the -1 accounts for indexed at 1, actual container has an index 0 but we ignore it
    {
        childIndex = holeIndex * 2; // hole at left child
        if (childIndex != container->size()-1 && comp(container->at(childIndex + 1), container->at(childIndex))) // if there's a right child and the right child is smaller than the left
        {
            childIndex += 1;
        }
        if (comp(container->at(childIndex), container->at(holeIndex))) // child < hole then swap them
        {
            temp = container->at(holeIndex);
            container->at(holeIndex) = container->at(childIndex);
            container->at(childIndex) = temp;
            holeIndex = childIndex;
        }
        else
        {
            break;
        }
    }
}

template <class Container, class Compare=std::less<typename Container::value_type>>
void heapify(Container* container, Compare comp=std::less<typename Container::value_type>{})
{
    //Container newContainer (container->size()+1);
    Container newContainer(*container);
    newContainer.push_back(typename Container::value_type());
    //newContainer.resize(container->size()+1);
    //newContainer.at(0) = typename Container::value_type();
    size_t j = 0;
    for (size_t i = 1; i < container->size()+1; i++)
    {
        newContainer.at(i) = container->at(j);
        j++;
    }
    newContainer.at(0) = typename Container::value_type();
    for (size_t i = (newContainer.size() - 1) / 2; i > 0; i--)
    {
        percolateDown(i, &newContainer, comp);
    }
    *container = newContainer;
    
}

template <class Container, class Compare=std::less<typename Container::value_type>>
void heap_insert(Container* container, const typename Container::value_type& value, Compare comp=std::less<typename Container::value_type>{})
{
    if (container->empty())
    {
        heapify(container);
    }
    container->push_back(value);
    int holeIndex = container->size() - 1;
    typename Container::value_type temp;

    while (comp(value,container->at(holeIndex/2)) && holeIndex != 1)
    {
        temp = container->at(holeIndex/2);
        container->at(holeIndex/2) = value;
        container->at(holeIndex) = temp;
        holeIndex /= 2;
    }


}

template <class Container>
const typename Container::value_type& heap_get_min(const Container& container)
{
    if (container.empty()) // if even the 0th index doesn't exist for some reason
    {
        throw std::invalid_argument("Attempting to access completely empty heap, no 0 index");
    }
    if (container.size() == 1) // if the heap is empty
    {
        throw std::invalid_argument("Attempting to access size 1 heap, index 0 exists");
    }
    return container.at(1);
}

template <class Container, class Compare=std::less<typename Container::value_type>>
void heap_delete_min(Container* container, Compare comp=std::less<typename Container::value_type>{})
{
    if (container->empty()) // completely empty heap
    {
        throw std::invalid_argument("Attempting to delete from completely empty heap, no 0 index");
    }
    if (container->size() == 1) // heap has size 1
    {
        throw std::invalid_argument("Attempting to delete from size 1 heap, index 0 exists");
    }
    if (container->size()-1 == 1) // only 1 element
    {
        container->pop_back();
        return;
    }
    container->at(1) = container->at(container->size() - 1);
    container->pop_back();
    percolateDown(1, container, comp);
}


#endif  // HEAP_H