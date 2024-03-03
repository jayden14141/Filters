//
// Created by Jayden on 28/02/2024.
//

#include <bitset>

#include "table.h"

namespace cuckooFilter {

    Table::Table(int b, int f, int m) : numberOfEntry(b), fingerPrintSize(f), numberOfBuckets(m) {
        // Size of each bucket = number of entries * bit of fingerprint function
        buckets = new Bucket[m];
        for (int i = 0; i < m; i++) buckets[i].size = b*f;
    }

    Table::~Table() {
        delete[] buckets;
    }

    bool Table::HasEmptyEntry(size_t row) const {
        if (row >= numberOfBuckets) throw std::out_of_range("Row index out of range");
        for (size_t i = 0; i < numberOfEntry; i++) {
            bool empty = true;
            for (size_t j = 0; j < fingerPrintSize; j++) {
                if (buckets[row].data.test(i * fingerPrintSize + j)) {
                    empty = false;
                    break;
                }
            }
            if (empty) return true;
        }
        return false;
    }

    size_t Table::ReadTag(size_t row, size_t column) const {
        if (row >= numberOfBuckets || column >= fingerPrintSize) {
            throw std::out_of_range("Index out of range");
        }
        size_t tag = 0;
        size_t initialPosition = column * fingerPrintSize;

        // Extracts 'f' bit starting from initialPosition
        for (size_t i = 0; i < fingerPrintSize; i++) {
            tag |= buckets[row].data[initialPosition + i];
            if (i < fingerPrintSize - 1) tag <<= 1;
        }
        return tag;
    }

    bool Table::WriteTag(size_t row, size_t value) {
        for (size_t i = 0; i < numberOfEntry; i++) {
            if (ReadTag(row, i) == 0) {
                WriteTag(row, i , value);
                return true;
            }
        }
        return false;
    }

    void Table::WriteTag(size_t row, size_t column, size_t value) {
        size_t initialPosition = column * fingerPrintSize;
        for (size_t i = 0; i < fingerPrintSize; i++) {
            bool bit = (value >> i) & 1;
            buckets[row].data.set(initialPosition + fingerPrintSize - 1 - i, bit);
        }
    }

    bool Table::InsertTag(size_t row, size_t value, bool evict, size_t &victim) {
        if (evict) {
            int random = rand() % numberOfEntry;
            victim = ReadTag(row, random);
            WriteTag(row, random, value);
            return true;
        } else return WriteTag(row, value);
    }

    bool Table::FindTag(size_t row1, size_t row2, size_t value) const {
        for (size_t i = 0; i < numberOfEntry; i++) {
            if ((ReadTag(row1, i) == value) || (ReadTag(row2, i) == value)) {
                return true;
            }
        }
        return false;
    }

    void Table::DeleteTag(size_t row, size_t column) {
        size_t initialPosition = column * fingerPrintSize;
        for (size_t i = 0; i < fingerPrintSize; i++) {
            buckets[row].data.set(initialPosition + i, 0);
        }
    }
}
