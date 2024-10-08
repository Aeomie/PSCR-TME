#pragma once
#include <iostream>
#include <vector>
#include <forward_list>
#include <utility> // for std::pair

template <typename K, typename V>
class HashMap
{
public:
    class Entry
    {
    public:
        const K key;
        V value;

        Entry(K k, V v) : key(k), value(v) {}; // constructor
    };

private:
    typedef std::forward_list<Entry> list_v;
    typedef std::vector<list_v> buckets_v;

public:
    class iterator
    {
        buckets_v &ref_buckets;
        typename buckets_v::iterator vit;
        typename list_v::iterator lit;

    public:
        iterator(buckets_v &buckets, typename buckets_v::iterator vit_p) : ref_buckets(buckets), vit(vit_p)
        {
            if (vit != ref_buckets.end())
            {
                advance();
            }
        };
        iterator &operator++()
        {
            ++lit;
            if (lit == vit->end())
            {
                advance();
            }
            return *this;
        };

        Entry &operator*()
        {
            return *lit;
        };

        bool operator!=(const iterator &o)
        {
            return &ref_buckets != &o.ref_buckets || vit != o.vit || lit != o.lit;
        };
        void advance()
        {
            for (++vit; vit != ref_buckets.end(); ++vit)
            {
                if (!vit->empty())
                {
                    lit = vit->begin();
                    return;
                }
            }
        };
    };

    buckets_v buckets;

    HashMap(size_t bucket_Count) : buckets(bucket_Count) {}

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
    }

    bool insert(const K &key, const V &value)
    {
        int keyIndex = std::hash<K>()(key) % size();
        for (auto &entry : buckets[keyIndex])
        {
            if (entry.key == key)
            {
                entry.value = value; // Update existing value
                return true;
            }
        }
        buckets[keyIndex].emplace_front(key, value); // Insert new entry
        return false;
    }

    size_t size() const
    {
        return buckets.size();
    }

    iterator begin()
    {
        return iterator(buckets, buckets.begin());
    }

    iterator end()
    {
        return iterator(buckets, buckets.end());
    }
};
