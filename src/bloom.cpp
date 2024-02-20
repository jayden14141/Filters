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

uint64_t bloomFilter::fastrange64(uint64_t word, uint64_t p)  {
    // http://lemire.me/blog/2016/06/27/a-fast-alternative-to-the-modulo-reduction/
#ifdef __SIZEOF_INT128__ // then we know we have a 128-bit int
    return (uint64_t)(((__uint128_t)word * (__uint128_t)p) >> 64);
#elif defined(_MSC_VER) && defined(_WIN64)
    // supported in Visual Studio 2005 and better
        uint64_t highProduct;
        _umul128(word, p, &highProduct); // ignore output
        return highProduct;
        unsigned __int64 _umul128(
            unsigned __int64 Multiplier,
            unsigned __int64 Multiplicand,
            unsigned __int64 *HighProduct
        );
    #else
        return word % p; // fallback
#endif // __SIZEOF_INT128__
}

size_t bloomFilter::GetM(int n) {
    double bits_per_item = 64.0;
    return std::max(1, (int)round(bits_per_item/n*exp(1.0)));
}

size_t bloomFilter::GetSize(double fpr) {
    double bit = log2(exp(1)) * log2(1 / fpr);
    return ceil(bit);
}

BloomFilter::BloomFilter(int bpi, int n, double fpr) {
    this->bits_per_item = bpi;
    this->fpr = fpr;
    this->n = n;
    this->m = (int) GetM(n);
    this->size = (int) GetSize(fpr);
    this->data = new uint64_t[size];
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
        data[fastrange64(a,this->size)] |= (1 << b);
        a += b;
    }
}

bool BloomFilter::Member(const int &item) const {
//    int some = std::stoi(item);
    uint64_t hashed = hasher(item);
    uint64_t a = (hashed >> 32) | (hashed << 32);
    uint64_t b = hashed;
    for (int i = 0; i < m; i++) {
        if ((data[fastrange64(a,this->size)] & (1 << b)) == 0) return false;
        a += b;
    }
    return true;
}

size_t BloomFilter::Size() const {
    return size;
}

void BloomFilter::_insertKeys() {
    for (int i = 0; i < n; i++) {
        //TODO: Generate random strings
        int some = 123;
        Add(some);
    }
}