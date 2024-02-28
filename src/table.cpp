//
// Created by Jayden on 28/02/2024.
//

#include "table.h"

cuckooFilter::Bucket::Bucket() : data(nullptr), size(0) {}

void cuckooFilter::Bucket::allocation(size_t count, size_t typeSize) {
    data = std::malloc(count*typeSize);
    size = count * typeSize;
}

cuckooFilter::Bucket::~Bucket() {
    std::free(data);
}

cuckooFilter::Table::Table(int b, int f, int m) {
    // Size of each bucket = number of entries * bit of fingerprint function
    // Size of each bucket should be power of 2.
    size_t sizePerBucket = b * f;

    if (sizePerBucket == 8) {
        entrySize = sizeof(uint8_t);
    } else if (sizePerBucket == 16) {
        entrySize = sizeof(uint16_t);
    } else if (sizePerBucket == 32) {
        entrySize = sizeof(uint32_t);
    } else {
        entrySize = sizeof(uint32_t);
    }

    this->buckets = new Bucket[m];
    for (size_t i = 0; i < m; i++) {
        buckets[i].allocation(b, entrySize);
    }

}

cuckooFilter::Table::~Table() {
    delete[] buckets;
}
