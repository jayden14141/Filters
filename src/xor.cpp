//
// Created by Jayden on 03/03/2024.
//

#include <cstdlib>
#include <stack>
#include <utility>
#include <queue>
#include <iostream>
#include <unordered_set>

#include "xor.h"
#include "util.h"

xorFilter::XorFilter::XorFilter(int n, double fpr, bool construct) : n(n), fpr(fpr) {
    this->f = _getF();
    fingerPrinter.set(f);
    this->size = _getSize();
    this->data = new uint16_t[size];
    this->bits_per_item = (double)size*f/(double)n;
    _newHash();
    std::fill_n(data,size,0);
    if (construct) {
        this->keys = util::generateUniqueKeys(n);
        _insertKeys();
    }
}

xorFilter::XorFilter::~XorFilter() {
    delete[] data;
}

void xorFilter::XorFilter::Add(std::vector<uint64_t> &keys) {
    this->keys = keys;
    _insertKeys();
}

bool xorFilter::XorFilter::Member(const uint64_t &item) const {
    uint16_t finger = fingerPrinter(item);
    uint16_t h0 = hasher0(item);
    uint16_t h1 = hasher1(item);
    uint16_t h2 = hasher2(item);
    return finger == (data[h0] ^ data[h1] ^ data[h2]);
}

size_t xorFilter::XorFilter::Size() const {
    return size*f;
}

double xorFilter::XorFilter::Fpr() const {
    return fpr;
}

void xorFilter::XorFilter::Info() const {
    std::cout << "XOR Filter" << std::endl;
    std::cout << "------------------------------- \n" << std::endl;
    std::cout << "Number of keys inserted  : " << n << std::endl;
    std::cout << "False positive rate  : " << fpr << std::endl;
    std::cout << "Size of a fingerprint function  : " << f << std::endl;
    std::cout << "Size of a filter in bits  : " << (int)ceil(Size()) << std::endl;
    std::cout << "Bits per Item (Theoretical) : " << log2(1/fpr) << std::endl;
    std::cout << "Bits per Item : " << bits_per_item << std::endl;
    std::cout << "Space overhead : " << 100 * (bits_per_item - log2(1/fpr)) / log2(1/fpr) << "% " << std::endl;
}

int xorFilter::XorFilter::_getF() const {
    return ceil(log2(1/fpr));
}

size_t xorFilter::XorFilter::_getSize() const {
    return ceil(1.23*n + 32);
}

void xorFilter::XorFilter::_insertKeys() {
    std::vector<std::pair<uint64_t, size_t>> sigma;
    sigma.reserve(n);
    bool success = false;
    success = _map(sigma);
    while (!success) {
        _newHash();
        sigma.clear();
        sigma.reserve(n);
        success = _map(sigma);
    }

    _assign(sigma);
//    if (success) {
//        _assign(sigma);
//    } else {
//        std::cout << "Not working" << std::endl;
//    }
}

bool xorFilter::XorFilter::_map(std::vector<std::pair<uint64_t, size_t>> &sigma) {
    std::vector<std::unordered_set<uint64_t>> H(size);
    std::queue<size_t> queue;

    for (const uint64_t &key: keys) {
        uint64_t h0 = hasher0(key);
        uint64_t h1 = hasher1(key);
        uint64_t h2 = hasher2(key);
        H[h0].insert(key);
        H[h1].insert(key);
        H[h2].insert(key);
    }

    for (size_t i = 0; i < H.size(); i++) {
        if (H[i].size() == 1) {
            queue.push(i);
        }
    }

    while (!queue.empty()) {
        size_t i = queue.front();
        queue.pop();
        if (H[i].size() == 1) {
            uint64_t x = *H[i].begin();
            sigma.emplace_back(x, i);
            for (auto &h: {hasher0(x), hasher1(x), hasher2(x)}) {
                H[h].erase(x);
                if (H[h].size() == 1) queue.push(h);
            }
        }
    }
    return sigma.size() == keys.size();
}

void xorFilter::XorFilter::_assign(const std::vector<std::pair<uint64_t, size_t>> &sigma) {
    for (const auto &[x, i]: sigma) {
        data[i] = 0;
        uint64_t finger = fingerPrinter(x);
        data[i] = finger ^ data[hasher0(x)] ^ data[hasher1(x)] ^ data[hasher2(x)];
    }
}

void xorFilter::XorFilter::_newHash() {
    hasher0 = hash_function::Ranged_SimpleMixHashing(0,size/3);
    hasher1 = hash_function::Ranged_SimpleMixHashing(size/3,size/3);
    hasher2 = hash_function::Ranged_SimpleMixHashing(2*size/3,size/3);
}