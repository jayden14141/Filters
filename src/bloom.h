//
// Created by Jayden on 11/02/2024.
//

#ifndef FILTERS_BLOOM_H
#define FILTERS_BLOOM_H

#include <string>

namespace bloomFilter {

    static inline size_t GetM(int n);

    static inline size_t GetSize(double fpr);

    class BloomFilter {
    public:
        uint64_t *data;
        size_t maxLength;
        size_t bitCount;

        BloomFilter(int bpi, int n, double fpr);

        ~BloomFilter();

        // Add an item to a filter
        void Add(const std::string &item);

        // Tell if the item is in the filter
        bool Member(const std::string &item) const;

        // Return the size of the filter
        size_t Size() const;
    private:
        size_t size;

        int bits_per_item;

        // Number of hash functions used
        int m;

        // Number of keys inserted
        int n;

        // False Positive Rate
        double fpr;

        void _insertKeys() const;
    };

}

#endif //FILTERS_BLOOM_H

