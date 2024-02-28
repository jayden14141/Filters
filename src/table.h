//
// Created by Jayden on 28/02/2024.
//

#ifndef FILTERS_TABLE_H
#define FILTERS_TABLE_H

#include <cstdlib>
#include <iostream>

namespace cuckooFilter {
    struct Bucket {
        void *data;
        // Size in bytes
        size_t size;

        Bucket();
        void allocation(size_t count, size_t typeSize);
        ~Bucket();
    };

    class Table {
    public:
        Bucket *buckets;
        size_t bucketCount;
        size_t entrySize;

        Table(int b, int f, int m);

        ~Table();

        bool HasEmptyEntry(size_t row);

        // Read tags based on row and column number
        size_t ReadTag(size_t row, size_t column);

        // Write tag on certain row (any empty place)
        bool WriteTag(size_t row, size_t value);

        // Write tag on certain position
        void WriteTag(size_t row, size_t column, size_t value);

        bool InsertTag(size_t row, size_t value, bool evict, size_t &victim);

        bool FindTag(size_t row1, size_t row2, size_t value);

        void DeleteTag(size_t row, size_t column);


    private:
        int numberOfEntry;
        int fingerPrintSize;
        int numberOfBuckets;
    };
}


#endif //FILTERS_TABLE_H
