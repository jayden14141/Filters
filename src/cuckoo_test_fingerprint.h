//
// Created by Jayden on 10/05/2024.
//

#ifndef FILTERS_CUCKOO_TEST_FINGERPRINT_H
#define FILTERS_CUCKOO_TEST_FINGERPRINT_H

#include <string>
#include <cstdlib>
#include "hash_function.h"
#include "table.h"

namespace cuckooFilter {

    class CuckooFilter_test_fingerprint {
    public:

        Table *data;

        hash_function::SimpleMixHashing hasher;

        hash_function::FingerPrinting fingerPrinter;

        int count = 0;

        CuckooFilter_test_fingerprint(int n, double fpr, bool construct, int f);

        ~CuckooFilter_test_fingerprint();

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

        double load_factor;

        void Info() const;
    private:

        // Number of entries (bucket size)
        int b;

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

#endif //FILTERS_CUCKOO_TEST_FINGERPRINT_H
