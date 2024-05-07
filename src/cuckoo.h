//
// Created by Jayden on 28/02/2024.
//

#ifndef FILTERS_CUCKOO_H
#define FILTERS_CUCKOO_H

#include <string>
#include <cstdlib>
#include "hash_function.h"
#include "table.h"
#include "table.h"

namespace cuckooFilter {

    class CuckooFilter {
    public:

        Table *data;

        hash_function::SimpleMixHashing hasher;

        hash_function::FingerPrinting fingerPrinter;

        int eviction = 0;

        CuckooFilter(int n, double fpr, bool construct);

        ~CuckooFilter();

        // Add an item to a filter
        bool Add(const uint64_t &item);

        // Add bulk items from given vector of keys
        bool AddAll(std::vector<uint64_t> &keys);

        // Tell if the item is in the filter
        bool Member(const uint64_t &item) const;

        // Return the size of the filter in bits
        size_t Size() const;

        double Fpr() const;

        double Bpi() const;

        void Info() const;
    private:

        double load_factor = 0.94;

        // Number of entries (bucket size)
        int b = 4;

        double bits_per_item;

        // Number of keys inserted
        int n;

        // Size of fingerprint function
        int f;

        // False Positive Rate
        double fpr;

        // Number of Buckets
        size_t m;

        // Size of a filter in bits
        size_t size;

        int _getF() const;

        size_t _getM() const;

        size_t _getSize() const;

        void _insertKeys();
    };
}

#endif //FILTERS_CUCKOO_H
