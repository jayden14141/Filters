//
// Created by Jayden on 28/02/2024.
//

#include "table.h"

cuckooFilter::Bucket::Bucket() : data(nullptr), size(0) {}

void cuckooFilter::Bucket::allocation(size_t count, size_t typeSize) {
    data = std::malloc(count*typeSize);
    size = count * typeSize;
    std::memset(data,0,size);
}

cuckooFilter::Bucket::~Bucket() {
    std::free(data);
}

cuckooFilter::Table::Table(int b, int f, int m) {
    // Size of each bucket = number of entries * bit of fingerprint function
    // Size of each bucket should be power of 2.
    this->numberOfEntry = b;
    this->fingerPrintSize = f;
    this->numberOfBuckets = m;

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

bool cuckooFilter::Table::HasEmptyEntry(size_t row) {
    for (size_t i = 0; i < numberOfEntry; i++) {
        if (*((char*)buckets[row].data + i) == 0) return true;
    }
    return false;
}

size_t cuckooFilter::Table::ReadTag(size_t row, size_t column) {
    auto *bucketData = static_cast<size_t*>(buckets[row].data);
    return bucketData[column];
}

bool cuckooFilter::Table::WriteTag(size_t row, size_t value) {
    for (size_t i = 0; i < numberOfEntry; i++) {
        if (ReadTag(row, i) == 0) {
            WriteTag(row, i , value);
            return true;
        }
    }
    return false;
}

void cuckooFilter::Table::WriteTag(size_t row, size_t column, size_t value) {
    auto *bucketData = static_cast<size_t*>(buckets[row].data);
    bucketData[column] = value;
}

bool cuckooFilter::Table::InsertTag(size_t row, size_t value, bool evict, size_t &victim) {
    if (evict) {
        int random = rand() % numberOfEntry;
        victim = ReadTag(row, random);
        WriteTag(row, random, value);
        return true;
    } else return WriteTag(row, value);
}

bool cuckooFilter::Table::FindTag(size_t row1, size_t row2, size_t value) {
    for (size_t i = 0; i < numberOfEntry; i++) {
        if ((ReadTag(row1, i) == value) || (ReadTag(row2, i) == value)) {
            return true;
        }
    }
    return false;
}

void cuckooFilter::Table::DeleteTag(size_t row, size_t column) {
    auto *bucketData = static_cast<size_t*>(buckets[row].data);
    bucketData[column] = 0;
}
