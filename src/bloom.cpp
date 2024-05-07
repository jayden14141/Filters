//
// Created by Jayden on 11/02/2024.
//

#include "bloom.h"
#include "hash_function.h"

#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <algorithm>

using namespace bloomFilter;

BloomFilter::BloomFilter(int n, double fpr, bool construct) {
    this->fpr = fpr;
    this->n = n;
    this->size = (int) _getSize();
    this->bits_per_item = (double)size*64/(double)n;
    this->m = (int) _getM();
    _generateHashFamily();
    this->data = new uint64_t[size];
    std::fill_n(data,size,0);
    if (construct) _insertKeys();
}

BloomFilter::~BloomFilter() {
    delete []data;
}

void BloomFilter::Add(const uint64_t &item) {
    // TODO: Implement the string hashing
//    int some = std::stoi(item);
//    uint64_t hashed = hasher(item);
//    uint64_t a = (hashed >> 32) | (hashed << 32);
//    uint64_t b = hashed;
//    for (int i = 0; i < m; i++) {
//        data[util::fastRange64(a, 64*size)] |= (1 << b);
//        a += b;
//    }
    for (int i = 0; i < m; i++) {
        uint64_t hashed = util::fastRange64(hashFamily[i](item), 64*size);
        uint64_t index = hashed >> 6;
        uint64_t bitPosition = hashed & 63;
        uint64_t mask = (1ULL << bitPosition);
        data[index] |= mask;
    }
}

void BloomFilter::AddAll(std::vector<uint64_t> &keys) {
    for (int i = 0; i < keys.size(); i++) {
        Add(keys[i]);
    }
}

bool BloomFilter::Member(const uint64_t &item) const {
//    int some = std::stoi(item);
    uint64_t hashed = hasher(item);
    uint64_t a = (hashed >> 32) | (hashed << 32);
    uint64_t b = hashed;
    for (int i = 0; i < m; i++) {
//        if ((data[util::fastRange64(a, 64*size)] & (1 << b)) == 0) return false;
//        a += b;
        uint64_t hashed = util::fastRange64(hashFamily[i](item), 64*size);
        uint64_t index = hashed >> 6;
        uint64_t bitPosition = hashed & 63;
//        std::cout << "After: " << data[index] << std::endl;
        if ((data[index] & (1ULL << bitPosition)) == 0) return false;
    }
    return true;
}

size_t BloomFilter::Size() const {
    return size;
}

double BloomFilter::Fpr() const {
    return fpr;
}

void BloomFilter::Info() const {
    std::cout << "Bloom Filter" << std::endl;
    std::cout << "------------------------------- \n" << std::endl;
    std::cout << "Number of keys inserted  : " << n << std::endl;
    std::cout << "False positive rate  : " << fpr << std::endl;
    std::cout << "Number of Hash functions used  : " << m << std::endl;
    std::cout << "Size of a filter in bits  : " << (int)ceil(size * 64) << std::endl;
    std::cout << "Bits per Item (Theoretical) : " << log2(1/fpr) << std::endl;
    std::cout << "Bits per Item : " << Bpi() << std::endl;
    std::cout << "Space overhead : " << 100 * (bits_per_item - log2(1/fpr)) / log2(1/fpr) << "% " << std::endl;
}

double BloomFilter::Bpi() const {
    return bits_per_item;
}

double BloomFilter::Offset() const {
    return (m / fpr)*sqrt((log(size*64)-m*log(fpr))/(size*64));
}

size_t BloomFilter::_getM() const {
    return ceil(bits_per_item * log(2));
}

size_t BloomFilter::_getSize() const {
    double bit = n * log(fpr) / log(1 / pow(2,log(2)));
    return ceil(bit / 64);
}

void BloomFilter::_insertKeys() {
    std::vector<uint64_t> random = util::generateUniqueKeys(n);
    for (int i = 0; i < n; i++) {
        Add((int)random[i]);
    }
}

void BloomFilter::_generateHashFamily() {
    for (int i = 0; i < m; i++) {
        hash_function::SimpleMixHashing h;
        hashFamily.push_back(h);
    }
}
