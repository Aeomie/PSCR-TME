#pragma once
#include <iostream>
using namespace std;
#include <vector>
#include <forward_list>

template <typename K, typename V>
class HashMap
{
    class Entry
    {
    public:
        const K key;
        V value;

        Entry(K k, V v) : key(k), value(v) {}; // constructor
    };
    typedef vector<forward_list<Entry>> buckets_v;

public:
    buckets_v buckets;

    HashMap(size_t bucket_Count) : buckets(bucket_Count) {};

    V *getValue(const K &key)
    {
        int hashValue = std::hash<K>()(key) % size();
        int keyIndex = hashValue % size();
        for (Entry &entry : buckets[keyIndex])
        {
            if (entry.key == key)
            {
                return &entry.value;
            }
        }
        return nullptr;
    };
    bool insert(const K &key, const V &value)
    {
        int keyIndex = std::hash<K>()(key) % size();
        for (auto &entry : buckets[keyIndex])
        {
            if (entry.key == key)
            {
                entry.value = value;
                return true;
            }
        }
        buckets[keyIndex].emplace_front(key, value);
        return false;
    };
    size_t size()
    {
        return buckets.size();
    }
};
