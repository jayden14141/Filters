//
// Created by Jayden on 03/03/2024.
//

#ifndef FILTERS_XOR_H
#define FILTERS_XOR_H

#include <string>
#include <cstdint>
#include <unordered_map>
#include <stack>
#include <vector>
#include <unordered_set>


#include "hash_function.h"
#include "util.h"

namespace xorFilter {
    class XorFilter {
    public:
        uint16_t *data;
        hash_function::Ranged_SimpleMixHashing hasher0;
        hash_function::Ranged_SimpleMixHashing hasher1;
        hash_function::Ranged_SimpleMixHashing hasher2;
        hash_function::FingerPrinting fingerPrinter;
        std::vector<uint64_t> keys;

        XorFilter(int n, double fpr, bool construct);

        ~XorFilter();

        // Add an item to a filter
        void Add(std::vector<uint64_t> &keys);

        // Tell if the item is in the filter
        bool Member(const uint64_t &item) const;

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

        // Size of the fingerprint function
        int f;

        // Number of keys inserted
        int n;

        // False Positive Rate
        double fpr;

        struct hashDist {
            uint64_t hashValue;
            uint64_t count;
        };

        typedef struct hashDist hashDist;

        int _getF() const;

        size_t _getSize() const;

        void _insertKeys();

        bool _map(std::vector<std::pair<uint64_t, size_t>> &sigma);

        void _assign(const std::vector<std::pair<uint64_t , size_t>> &sigma);

        void _newHash();
    };
}


#endif //FILTERS_XOR_H
