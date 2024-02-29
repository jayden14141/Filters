//
// Created by Jayden on 11/02/2024.
//

#ifndef FILTERS_BLOOM_H
#define FILTERS_BLOOM_H

#include <string>
#include <cstdint>
#include "hash_function.h"
#include "util.h"

namespace bloomFilter {

    class BloomFilter {
    public:
        uint64_t *data;
        hash_function::SimpleMixHashing hasher;

        BloomFilter(int n, double fpr, bool construct);

        ~BloomFilter();

        // Add an item to a filter
        void Add(const int &item);

        // Tell if the item is in the filter
        bool Member(const int &item) const;

        // Return the size of the filter in bits
        size_t Size() const;

        // Return the set false positive rate
        double Fpr() const;

        void Info() const;
    private:
        // Size of the array itself
        size_t size;

        // (Size * 64 / n)
        double bits_per_item;

        // Number of hash functions used
        int m;

        // Number of keys inserted
        int n;

        // False Positive Rate
        double fpr;

        size_t _getM() const;

        size_t _getSize() const;

        void _insertKeys();
    };
}

#endif //FILTERS_BLOOM_H

