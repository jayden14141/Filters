//
// Created by Jayden on 11/02/2024.
//

#include "bloom.h"
#include "../hash_function.h"

#include <cstdlib>
#include <string>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include <opencl-c.h>

using namespace bloomFilter;

size_t bloomFilter::GetM(int n) {
    double bits_per_item = 64.0;
    return max(1.0, (int)round(bits_per_item/n*exp(1.0)));
}

size_t bloomFilter::GetSize(double fpr) {
    double bit = log2(exp(1)) * log2(1 / fpr);
    return ceil(bit/64);
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

void BloomFilter::Add(const std::string &item) {

}

bool BloomFilter::Member(const std::string &item) const {
    return false;
}

size_t BloomFilter::Size() const {
    return size;
}

void BloomFilter::_insertKeys() const {
    for (int i = 0; i < m; i++) {
        hash_function::SimpleMixHashing hasher;
        for (int j = 0; j < n; j++) {
            int some = 1;
            data[hasher(some)] = 1;
        }
    }
}
