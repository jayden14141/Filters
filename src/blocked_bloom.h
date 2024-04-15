//
// Created by Jayden on 09/04/2024.
//

#ifndef FILTERS_BLOCKED_BLOOM_H
#define FILTERS_BLOCKED_BLOOM_H

#include "bloom.h"
#include "util.h"
#include "hash_function.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include <Accelerate/Accelerate.h>
#include <arm_neon.h>

namespace blockedBloomFilter {
    class BlockedBloom {
    public:

        BlockedBloom(int n, double fpr, bool construct);

        ~BlockedBloom();

        // Add an item to a filter
        void Add(const uint64_t &item);

        // Add bulk items from given vector of keys
        void AddAll(std::vector<uint64_t> &keys);

        // Tell if the item is in the filter
        bool Member(const uint64_t &item);

        // Return the size of the filter in bits
        size_t Size() const;

        // Return the set false positive rate
        double Fpr() const;

        double Bpi() const;

        void Info() const;
    private:

        using Bucket = uint16x8_t;

        int bucketCount;

        Bucket* buckets;

        hash_function::SimpleMixHashing hasher;

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

        uint16x8_t _makeMask(const uint16_t hashed);

        double _getBpi() const;

        size_t _getSize() const;

        void _insertKeys();
    };
}


#endif //FILTERS_BLOCKED_BLOOM_H
