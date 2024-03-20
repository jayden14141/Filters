//
// Created by Jayden on 17/03/2024.
//

#ifndef FILTERS_XORTABLE_H
#define FILTERS_XORTABLE_H


#include <cstdlib>
#include <iostream>

namespace xoredCuckoo {
    struct Bucket {
        std::bitset<36> data;
        // Size in bits
        size_t size;

        Bucket() = default;
        ~Bucket() = default;
    };

    class XorTable {
    public:
        Bucket *buckets;
        size_t bucketCount;
        size_t entrySize;

        XorTable(int b, int f, int m);

        ~XorTable();

        bool HasEmptyEntry(size_t row) const;

        bool ExtractStateBit(size_t value) const;

        void SetStateBit(size_t row, size_t column, bool bit);

        // Read tags based on row and column number
        size_t ReadTag(size_t row, size_t column) const;

        size_t ReadTag(size_t row, size_t column, size_t value) const;

        // Write tag on certain row (any empty place)
        bool WriteTag(size_t row, size_t value);

        // Write tag on certain position
        void WriteTag(size_t row, size_t column, size_t value);

        bool InsertTag(size_t row, size_t value, bool evict, size_t &victim);

        bool FindTag(size_t row1, size_t row2, size_t value) const;

        void DeleteTag(size_t row, size_t column, size_t value);


    private:
        int numberOfEntry;
        int fingerPrintSize;
        int numberOfBuckets;
    };
}

#endif //FILTERS_XORTABLE_H
