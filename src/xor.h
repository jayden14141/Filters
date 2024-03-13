//
// Created by Jayden on 03/03/2024.
//

#ifndef FILTERS_XOR_H
#define FILTERS_XOR_H

#include <string>
#include <cstdint>
#include <unordered_map>

#include "hash_function.h"
#include "util.h"

namespace xorFilter {
    class XorFilter {
    public:
        uint16_t *data;
        hash_function::SimpleMixHashing hasher0;
        hash_function::SimpleMixHashing hasher1;
        hash_function::SimpleMixHashing hasher2;
        hash_function::FingerPrinting fingerPrinter;

        XorFilter(int n, double fpr, bool construct);

        ~XorFilter();

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

        // Size of the the fingerprint function
        int f;

        // Number of keys inserted
        int n;

        // False Positive Rate
        double fpr;

        // Get values that solely owns the slot.
        std::unordered_map<size_t,uint16_t> _getSingleElements();

        int _getF() const;

        size_t _getSize() const;

        void _insertKeys();
    };
}


#endif //FILTERS_XOR_H
