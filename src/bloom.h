//
// Created by Jayden on 11/02/2024.
//

#ifndef FILTERS_BLOOM_H
#define FILTERS_BLOOM_H

#include <string>
#include "hash_function.h"

namespace bloomFilter {

    static inline uint64_t fastrange64(uint64_t word, uint64_t p);

    class BloomFilter {
    public:
        uint64_t *data;
        size_t maxLength;
        size_t bitCount;
        hash_function::SimpleMixHashing hasher;

        BloomFilter(int bpi, int n, double fpr);

        ~BloomFilter();

        // Add an item to a filter
        void Add(const int &item);

        // Tell if the item is in the filter
        bool Member(const int &item) const;

        // Return the size of the filter
        size_t Size() const;

        void Info() const;
    private:
        size_t size;

        int bits_per_item;

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

