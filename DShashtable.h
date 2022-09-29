//
// Created by Charlotte on 4/10/2020.
//

#ifndef UNTITLED2_DSHASHTABLE_H
#define UNTITLED2_DSHASHTABLE_H
#include <hash_fun.h>
#include <list>
#include <vector>


template<typename K, typename T>
class DShashtable {
private:
    struct hashNode {
        K key; //in practice, this will be the author name
        std::vector<T> data; //in practice, this will be the list of docIDs that author contributed to

        hashNode(const K& k, const T& t) {
            key = k;
            data.push_back(t);
        }

        hashNode& operator=(const hashNode& copyNode) {
            key = copyNode.key;
            data = copyNode.data;
            return *this;
        }
    };

    struct Bucket {
        std::list<hashNode> contents;

        bool isEmpty() {
            return contents.empty();
        }
    };

    Bucket* table;
    int size; //total number of unique keys in the table
    int capacity; //total number of buckets allowed. This remains constant
public:
    DShashtable();
    //DShashtable(const int& cap); //constructor with custom capacity
    DShashtable(const DShashtable& copyTable);
    DShashtable& operator=(const DShashtable& copyTable);

    int getSize();
    void insert(const K& k, const T& t);
    void clear();
    std::vector<T> operator[](const K& k);

    void tableToFile(const std::string& filename);
    void tableFromFile(const std::string& filename);

    ~DShashtable();
};

template<typename K, typename T>
DShashtable<K, T>::DShashtable() {
    size = 0;
    capacity = 1024; //***adjust this number depending on how the program turns out
    table = new Bucket[capacity];

    Bucket b; //empty bucket
    for(unsigned int i = 0; i < capacity; i++) {
        table[i] = b;
    }
}

//template<typename K, typename T>
//DShashtable<K, T>::DShashtable(const int& cap) {
//    size = 0;
//    capacity = cap;
//    table = new Bucket[capacity];
//
//    Bucket b; //empty bucket
//    for(unsigned int i = 0; i < capacity; i++) {
//        table[i] = b;
//    }
//}

template<typename K, typename T>
DShashtable<K, T>::DShashtable(const DShashtable& copyTable) {
    size = copyTable.size;
    capacity = copyTable.capacity;
    table = new Bucket[capacity];

    for(unsigned int i = 0; i < capacity; i++)
        table[i].contents = copyTable.table[i].contents;
}

template<typename K, typename T>
DShashtable<K, T>& DShashtable<K,T>::operator=(const DShashtable<K, T> &copyTable) { //***rewrite this
    if(capacity != copyTable.capacity) {
        delete[] table;
        capacity = copyTable.capacity;
        table = new Bucket[capacity];
    }

    for(unsigned int i = 0; i < capacity; i++)
        table[i].contents = copyTable.table[i].contents;

    size = copyTable.size;
    return *this;
}

template<typename K, typename T>
int DShashtable<K,T>::getSize() {
    return size;
}

template<typename K, typename T>
void DShashtable<K, T>::insert(const K& k, const T& t) {
    std::size_t h = std::hash<K>{} (k); //standard hash function
    //int index = h % capacity;
    int index = h & 0x3F; //bit shift

    typename std::list<hashNode>::iterator iter = table[index].contents.begin();
    while (iter != table[index].contents.end()) { //iterates through list within the bucket
        if (iter->key == k) { //if the key is already in the bucket, just add the data to the node's data vector
            iter->data.push_back(t);
            break;
        }
        iter++;
    }

    if(iter == table[index].contents.end() || table[index].isEmpty()) { //if the key was not already in the bucket
        hashNode node(k, t);
        table[index].contents.push_back(node);
        size++;
    }
}

template<typename K, typename T>
std::vector<T> DShashtable<K, T>::operator[](const K& k) {
    std::vector<T> retVec;
    size_t h = std::hash<K>{} (k); //standard hash function
    //int index = h % capacity
    int index = h & 0x3F; //bit shift

    typename std::list<hashNode>::iterator iter = table[index].contents.begin();
    while (iter != table[index].contents.end()) { //iterates through list within the bucket
        if (iter->key == k) //if the key is in the bucket
            retVec = iter->data; //
        iter++;
    }

    return retVec; //will return an empty vector if the key is not in the table
}

template<typename K, typename T>
void DShashtable<K, T>::clear() {
    for(unsigned int i = 0; i < capacity; i++) { //capacity remains the same
        if(!table[i].isEmpty()) {
            table[i].contents.clear();
        }
    }
    //leaves an array of empty buckets
    size = 0;
}

template<typename K, typename T>
DShashtable<K,T>::~DShashtable() {
    if(capacity > 0)
        delete[] table;
}

#endif //UNTITLED2_DSHASHTABLE_H
