#ifndef HASHTABLE_OPEN_ADDRESSING_H
#define HASHTABLE_OPEN_ADDRESSING_H

#include <iostream>
#include <vector>
#include <list>

using std::vector, std::list, std::cout, std::endl;

template <class Key, class Hash=std::hash<Key>>
class HashTable {
    public:
        enum EntryType { ACTIVE, EMPTY, DELETED };

        HashTable<Key,Hash>() : table(11), totalElements(0) {}
        explicit HashTable(size_t capacity) : table(capacity), totalElements(0) {}
        HashTable(const HashTable& other) : table(other.table.capacity()), totalElements(other.totalElements) {
            for (size_t i = 0; i < other.table.size(); i++)
            {
                table.at(i) = other.table.at(i);
            }
        }
        HashTable& operator=(const HashTable& other) {
            if (this == &other)
            {
                return *this;
            }
            vector<HashEntry> newTable (other.table.capacity());
            for (size_t i = 0; i < other.table.size(); i++)
            {
                newTable.at(i) = other.table.at(i);
            }
            table.swap(newTable);
            totalElements = other.totalElements;
            return *this;
        }
        ~HashTable() {
            totalElements = 0;
            table.clear(); // clears the vector
        }
        bool is_empty() const {
            if (table.empty() || totalElements == 0)
                return true;
            return false;
        }
        size_t size() const {
            return totalElements;
        }
        size_t table_size() const {
            return table.capacity();
        }
        void make_empty() {
            totalElements = 0;
            for (size_t i = 0; i < table_size(); i++)
            {
                table.at(i).info = EMPTY;
            }
        }

        bool isActive(int currentPos) const
        { 
            return table[currentPos].info == ACTIVE;
        }

        bool contains(const Key& x) const
        { 
            return isActive(position(x));
        }

        size_t position(const Key& x) const
        {
            size_t currentPos = Hash {} (x) % table.capacity();
            //cout << "currentPos after hashing: " << currentPos << endl;
            while(table[currentPos].info != EMPTY && table[currentPos].element != x)
            {
                currentPos += 1;
                {
                    if (currentPos >= table.capacity())
                        currentPos = 0;
                }
            }
            return currentPos;
        }

        bool insert(const Key& x)
        {
            if (contains(x))
            {
                return false;
            }
            size_t currentPos = position(x);
            if(isActive(currentPos))
                return false;
            HashEntry toInsert;
            toInsert.element = x;
            toInsert.info = ACTIVE;
            table[currentPos] = toInsert;
            totalElements++;

            if(((float) totalElements / table.capacity()) > 0.5)
            {
                rehash();
            }

            return true;
        }

        size_t remove(const Key& x)
        {
            if (!contains(x))
            {
                return 0;
            }
            size_t currentPos = position(x);
            if(!isActive(currentPos))
                return 0;

            table[currentPos].info = DELETED;
            totalElements--;
            return 1;
        }

        void rehash() 
        {
            vector<HashEntry> oldArray = table;

            table.resize(getNextPrime(2 * oldArray.size()));
            for( HashEntry& entry : table )
                entry.info = EMPTY;

            totalElements = 0;
            for(HashEntry& entry : oldArray)
                if( entry.info == ACTIVE )
                    insert(std::move(entry.element));
        }

        bool isPrime(size_t n)
        {
            if (n <= 1)  
                return false;
            if (n <= 3)  
                return true;
            if (n % 2 == 0 || n % 3 == 0) 
                return false;
        
            for (size_t i = 5; i * i <= n; i+=6)
            {
                if (n % i == 0 || n % (i + 2) == 0)
                {
                    return false;
                }
            }
            return true;
        }

        size_t getNextPrime(size_t N)
        {
            if (N <= 1)
                return 2;
            size_t prime = N;

            while (true) {
                prime++;
                if (isPrime(prime))
                    return prime;
            }
        }

        void print_table(std::ostream& os =std::cout) const
        {
            if (totalElements == 0)
            {
                os << "<empty>\n";
                return;
            }
            int index = 0;
            for (HashEntry entry : table)
            {
                if (entry.info == ACTIVE)
                {
                    os << index << ": " << entry.element << "\n";
                }
                index++;
            }
        }

    private: 
        struct HashEntry
        {
            Key element;
            EntryType info = EMPTY;
        };
        vector<HashEntry> table;
        size_t totalElements;
};

#endif  // HASHTABLE_OPEN_ADDRESSING_H