//
// Created by Jayden on 28/02/2024.
//

#ifndef FILTERS_TABLE_H
#define FILTERS_TABLE_H

#include <cstdlib>

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
    };
}


#endif //FILTERS_TABLE_H
