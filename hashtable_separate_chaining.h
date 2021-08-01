#ifndef HASHTABLE_SEPARATE_CHAINING_H
#define HASHTABLE_SEPARATE_CHAINING_H

#include <iostream>
#include <vector>
#include <list>

using std::vector, std::list, std::cout, std::endl;

template <class Key, class Hash=std::hash<Key>>
class HashTable {
    private: 
        vector<list<Key>> table;
        size_t totalElements;
        float maxLoad;
    public:
        HashTable<Key,Hash>() : table(11), totalElements(0), maxLoad(1) {}
        explicit HashTable(size_t capacity) : table(capacity), totalElements(0), maxLoad(1) {}
        HashTable(const HashTable& other) : table(other.table.capacity()), totalElements(other.totalElements), maxLoad(other.maxLoad) {
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
            vector<list<Key>> newTable (other.table.capacity());
            for (size_t i = 0; i < other.table.size(); i++)
            {
                newTable.at(i) = other.table.at(i);
            }
            table.swap(newTable);
            totalElements = other.totalElements;
            maxLoad = other.maxLoad;
            return *this;
        }
        ~HashTable() {
            for (size_t i = 0; i < table.size(); i++)
            {
                table.at(i).clear(); // clears each linked list in the vector
            }
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

        void make_empty() {
            totalElements = 0;
            for (size_t i = 0; i < table.size(); i++)
            {
                table.at(i).clear(); // clears each linked list in the vector
            }
        }

        bool insert(const Key& key) 
        {
            if (contains(key))
            {
                return false;
            }
            size_t hash_value = Hash {} (key) % table.capacity();
            table.at(hash_value).push_back(key);
            totalElements++;
            if (load_factor() > maxLoad)
            {
                size_t nextPrime = getNextPrime(2*table.capacity());
                rehash(nextPrime);   
            }
            return true;
        }

        size_t remove(const Key& key)
        {
            if (!contains(key))
            {
                return 0;
            }
            size_t hash_value = Hash {} (key) % table.capacity();
            for (const Key& value : table.at(hash_value))
            {
                if (value == key)
                {
                    table.at(hash_value).remove(value);
                    totalElements--;
                    break;
                }
            }
            return 1;
        }

        bool contains(const Key& key) const
        {
            size_t hash_value = Hash {} (key) % table.capacity();
            for (const Key& value : table.at(hash_value))
            {
                if (value == key)
                {
                    return true;
                }
            }
            return false;
        }

        size_t bucket_count() const {
            return table.capacity();
        }

        size_t bucket_size(size_t index) const
        {
            if (index > table.size() || index >= table.capacity())
                throw std::out_of_range("");
            return table.at(index).size();
        }

        
        size_t bucket(const Key& key) const {
            return (Hash {} (key) % table.capacity()); 
        }

        float load_factor() const {
            if (table.capacity() == 0)
            {
                return 0;
            }
            return (float) totalElements / table.capacity();
        }

        float max_load_factor() const {
            return maxLoad;
        }

        void max_load_factor(float newMax) {
            if (newMax <= 0)
            {
                throw std::invalid_argument("");
            }
            maxLoad = newMax;
            if (load_factor() > maxLoad)
            {
                size_t nextPrime = getNextPrime(2*table.capacity());
                rehash(nextPrime);
            }
        }

        void rehash(size_t newCap) {
            size_t oldCap = table.capacity();
            if (((float) totalElements / newCap) > maxLoad)
            {
                newCap = getNextPrime(size() / max_load_factor());
            }
            vector<list<Key>> newTable (newCap);
            if (newCap != oldCap) // number of buckets has changed
            {
                for (size_t i = 0; i < oldCap; i++) // was previously old size
                {
                    if (!table.at(i).empty())
                    {
                        for (const Key& value : table.at(i))
                        {
                            newTable.at(Hash {} (value) % newTable.capacity()).push_back(value);
                        }
                    }
                }
                table.swap(newTable); // was outside, may have caused issues
            }
        }

        void print_table(std::ostream& os =std::cout) const
        {
            if (totalElements == 0)
            {
                os << "<empty>\n";
                return;
            }
            for (size_t i = 0; i < table.size(); i++)
            {
                if (!table.at(i).empty())
                {
                    os << i << ": [";
                    for (const Key& value : table.at(i))
                    {
                        os << " | " << value << " | ";
                    }
                    os << "]\n";
                }
            }
        }

        bool isPrime(int n)
        {
            if (n <= 1)  
                return false;
            if (n <= 3)  
                return true;
            if (n % 2 == 0 || n % 3 == 0) 
                return false;
        
            for (int i = 5; i * i <= n; i+=6)
            {
                if (n % i == 0 || n % (i + 2) == 0)
                {
                    return false;
                }
            }
            return true;
        }

        int getNextPrime(int N)
        {
            if (N <= 1)
                return 2;
            int prime = N;

            while (true) {
                prime++;
                if (isPrime(prime))
                    return prime;
            }
        }


};

#endif  // HASHTABLE_SEPARATE_CHAINING_H