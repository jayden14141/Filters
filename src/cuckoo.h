//
// Created by Jayden on 28/02/2024.
//

#ifndef FILTERS_CUCKOO_H
#define FILTERS_CUCKOO_H

#include <string>
#include <cstdlib>
#include "hash_function.h"
#include "table.h"

namespace cuckooFilter {

//    static inline uint64_t fastrange64(uint64_t word, uint64_t p);

    class CuckooFilter {
    public:

        Table *data;

        hash_function::SimpleMixHashing hasher;

        // TODO: Set up a new fingerprint function depending on f
        hash_function::SimpleMixHashing fingerprint;

        CuckooFilter(int n,  int f, double fpr);

        ~CuckooFilter();

        // Add an item to a filter
        bool Add(const int &item);

        // Tell if the item is in the filter
        bool Member(const int &item) const;

        // Return the size of the filter in bits
        size_t Size() const;

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
        int m;

        // Size of a filter in bits
        size_t size;

        size_t _getM() const;

        size_t _getSize() const;

        void _insertKeys();
    };
}

#endif //FILTERS_CUCKOO_H
