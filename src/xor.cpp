//
// Created by Jayden on 03/03/2024.
//

#include <cstdlib>

#include "xor.h"
#include "util.h"

xorFilter::XorFilter::XorFilter(int n, double fpr, bool construct) : n(n), fpr(fpr) {
    this->f = _getF();
    fingerPrinter.set(f);
    this->size = _getSize();
    this->data = new uint16_t[size];
    this->bits_per_item = (double)size/(double)n;
    std::fill_n(data,size,0);
    if (construct) _insertKeys();
}

xorFilter::XorFilter::~XorFilter() {
    delete[] data;
}

void xorFilter::XorFilter::Add(const int &item) {

}

bool xorFilter::XorFilter::Member(const int &item) const {
    uint16_t finger = fingerPrinter(item);
    uint16_t h0 = util::fastRangeSize(hasher0(item), size);
    uint16_t h1 = util::fastRangeSize(hasher1(item), size);
    uint16_t h2 = util::fastRangeSize(hasher2(item), size);
    return f == (data[h0] ^ data[h1] ^ data[h2]);
}

size_t xorFilter::XorFilter::Size() const {
    return size*16;
}

double xorFilter::XorFilter::Fpr() const {
    return fpr;
}

void xorFilter::XorFilter::Info() const {

}

int xorFilter::XorFilter::_getF() const {
    return ceil(log2(1/fpr));
}

size_t xorFilter::XorFilter::_getSize() const {
    return ceil(1.23*n*f/16);
}

void xorFilter::XorFilter::_insertKeys() {

}

std::unordered_map<size_t, uint16_t> xorFilter::XorFilter::_getSingleElements() {
    return std::unordered_map<size_t, uint16_t>();
}
