//
// Created by Jayden on 17/03/2024.
//

#include "xorTable.h"

#include <bitset>

#include "table.h"

namespace xoredCuckoo {

    XorTable::XorTable(int b, int f, int m) : numberOfEntry(b / 2), fingerPrintSize(f + 1), numberOfBuckets(m) {
        // Size of each bucket = number of entries * bit of fingerprint function
        buckets = new Bucket[m];
        for (int i = 0; i < m; i++) buckets[i].size = numberOfEntry * fingerPrintSize;
    }

    XorTable::~XorTable() {
        delete[] buckets;
    }

    bool XorTable::HasEmptyEntry(size_t row) const {
        if (row >= numberOfBuckets) throw std::out_of_range("Row index out of range");
        for (size_t i = 0; i < numberOfEntry; i++) {
            // Checking if there is a slot with state bit set to zero
            if (buckets[row].data.test(i * fingerPrintSize)) {
                return true;
            }
        }
        return false;
    }

    bool XorTable::ExtractStateBit(size_t value) const {
        // A mask with only the MSB bit set
        size_t mask = 1 << (fingerPrintSize - 1);
        // Extract the MSB and shift it to the rightmost position
        size_t msb = (value & mask) >> (fingerPrintSize - 1);
        return msb;
    }

    void XorTable::SetStateBit(size_t row, size_t column ,bool bit) {
        size_t initialPosition = column * fingerPrintSize;
        buckets[row].data.set(initialPosition, bit);
    }

    size_t XorTable::ReadTag(size_t row, size_t column) const {
        if (row >= numberOfBuckets || column >= numberOfEntry) {
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


    bool XorTable::WriteTag(size_t row, size_t value) {
        for (size_t i = 0; i < numberOfEntry; i++) {
            // If the state bit is set to 0 (There is an occupancy)
            if (!ExtractStateBit(ReadTag(row, i))) {
                WriteTag(row, i , value);
                return true;
            }
        }
        return false;
    }

    // Attempts to write if and only if the state bit is set to zero
    void XorTable::WriteTag(size_t row, size_t column, size_t value) {
        size_t initialPosition = column * fingerPrintSize;
        size_t storedValue = ReadTag(row, column);
        // If the slot is empty, fill the slot without setting the state bit
        if (storedValue == 0) {
            for (size_t i = 0; i < fingerPrintSize - 1; i++) {
                bool bit = (value >> i) & 1;
                buckets[row].data.set(initialPosition + fingerPrintSize - 1 - i, bit);
            }
        } else { // If the slot already contains a single fingerprint, xor the value and set the state bit to 1
            for (size_t i = 0; i < fingerPrintSize - 1; i++) {
                bool bit = (value >> i) ^ (storedValue >> i);
                buckets[row].data.set(initialPosition + fingerPrintSize - 1 - i, bit);
            }
            // Set the state bit to 1 to indicate the slot is now full
            buckets[row].data.set(initialPosition, true);
        }
    }

    bool XorTable::InsertTag(size_t row, size_t value, bool evict, size_t &victim) {
        if (evict) {
            int random = rand() % numberOfEntry;
            victim = ReadTag(row, random);
            WriteTag(row, random, value);
            return true;
        } else return WriteTag(row, value);
    }

    bool XorTable::FindTag(size_t row1, size_t row2, size_t value) const {
        for (size_t i = 0; i < numberOfEntry; i++) {
            size_t v1 = ReadTag(row1, i);
            size_t v2 = ReadTag(row2, i);

            if (ExtractStateBit(v1) )
            if ((ReadTag(row1, i) == value) || (ReadTag(row2, i) == value)) {
                return true;
            }
        }
        return false;
    }

    void XorTable::DeleteTag(size_t row, size_t column, size_t value) {
        size_t initialPosition = column * fingerPrintSize;
        size_t originalValue = ReadTag(row,column);
        if(!ExtractStateBit(originalValue)) {
            for (size_t i = 0; i < fingerPrintSize - 1; i++) {
                buckets[row].data.set(initialPosition + i, false);
            }
        } else {
            for (size_t i = 0; i < fingerPrintSize  - 1; i++) {
                bool bit = (value >> i) ^ (originalValue >> i);
                buckets[row].data.set(initialPosition + i, bit);
                SetStateBit(row,column,false);
            }
        }
    }
}