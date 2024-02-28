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

BloomFilter::BloomFilter(int n, double fpr) {
    this->fpr = fpr;
    this->n = n;
    this->size = (int) _getSize();
    this->m = (int) _getM();
    this->data = new uint64_t[size];
    this->bits_per_item = (double)size*64/(double)n;
    std::fill_n(data,size,0);
    _insertKeys();
}

BloomFilter::~BloomFilter() {
    delete []data;
}

void BloomFilter::Add(const int &item) {
    // TODO: Implement the string hashing
//    int some = std::stoi(item);
    uint64_t hashed = hasher(item);
    uint64_t a = (hashed >> 32) | (hashed << 32);
    uint64_t b = hashed;
    for (int i = 0; i < m; i++) {
        data[util::fastrange64(a,this->size)] |= (1 << b);
        a += b;
    }
}

bool BloomFilter::Member(const int &item) const {
//    int some = std::stoi(item);
    uint64_t hashed = hasher(item);
    uint64_t a = (hashed >> 32) | (hashed << 32);
    uint64_t b = hashed;
    for (int i = 0; i < m; i++) {
        if ((data[util::fastrange64(a,this->size)] & (1 << b)) == 0) return false;
        a += b;
    }
    return true;
}

size_t BloomFilter::Size() const {
    return size;
}

void BloomFilter::Info() const {
    std::cout << "Bloom Filter" << std::endl;
    std::cout << "------------------------------- \n" << std::endl;
    std::cout << "Bits per Item : " << bits_per_item << std::endl;
    std::cout << "Size of a filter in bits  : " << (int)ceil(size * 64) << std::endl;
    std::cout << "Number of Hash functions used  : " << m << std::endl;
    std::cout << "Number of keys inserted  : " << n << std::endl;
    std::cout << "False positive rate  : " << fpr << std::endl;
}

size_t BloomFilter::_getM() const {
    return std::max(1, (int)round((double)size*64*log(2)/n));
}

size_t BloomFilter::_getSize() const {
    double bit = n * log(fpr) / log(1 / pow(2,log(2)));
    return ceil(bit / 64);
}

void BloomFilter::_insertKeys() {
    for (int i = 0; i < n; i++) {
        //TODO: Generate random strings
        int some = random();
        Add(some);
    }
}