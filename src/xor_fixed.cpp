//
// Created by Jayden on 03/03/2024.
//

#include <cstdlib>
#include <stack>
#include <utility>
#include <queue>
#include <iostream>
#include <unordered_set>
#include <thread>
#include <mutex>
#include <atomic>

#include "xor_fixed.h"
#include "util.h"

xorFilter::XorFilter_fixed::XorFilter_fixed(int n, double fpr, bool construct) : n(n), fpr(fpr) {
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

xorFilter::XorFilter_fixed::~XorFilter_fixed() {
    delete[] data;
}

void xorFilter::XorFilter_fixed::Add(std::vector<uint64_t> &keys) {
    this->keys = keys;
    _insertKeys();
}

void xorFilter::XorFilter_fixed::AddAll(std::vector<uint64_t> &keys) {
    this->keys = keys;
    _insertKeys();
}

bool xorFilter::XorFilter_fixed::Member(const uint64_t &item) const {
    uint16_t finger = fingerPrinter(item);
    uint16_t h0 = hasher0(item);
    uint16_t h1 = hasher1(item);
    uint16_t h2 = hasher2(item);
    return finger == (data[h0] ^ data[h1] ^ data[h2]);
}

size_t xorFilter::XorFilter_fixed::Size() const {
    return size*f;
}

double xorFilter::XorFilter_fixed::Fpr() const {
    return fpr;
}

double xorFilter::XorFilter_fixed::Bpi() const {
    return bits_per_item;
}

void xorFilter::XorFilter_fixed::Info() const {
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

int xorFilter::XorFilter_fixed::_getF() const {
    return ceil(log2(1/fpr));
}

size_t xorFilter::XorFilter_fixed::_getSize() const {
    return ceil(1.23*n + 32);
}

void xorFilter::XorFilter_fixed::_insertKeys() {
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

bool xorFilter::XorFilter_fixed::_map(std::vector<std::pair<uint64_t, size_t>> &sigma) {
    std::vector<std::unordered_set<uint64_t>> H(size);
    bool init = false;
    std::vector<size_t> initLone;

    for (const uint64_t &key: keys) {
        uint64_t h0 = hasher0(key);
        uint64_t h1 = hasher1(key);
        uint64_t h2 = hasher2(key);
        H[h0].insert(key);
        H[h1].insert(key);
        H[h2].insert(key);
    }

    for (size_t i = 0; i < H.size(); i++) {
        if (H[i].size() == 1) initLone.push_back(i);
    }

    for (int start = 0; start < initLone.size(); start++) {
        std::queue<size_t> queue;
        std::vector<std::pair<uint64_t, size_t>> tmpSigma;
        std::vector<std::unordered_set<uint64_t>> tempH = H;
        if (tempH[initLone[start]].size() == 1) {
            queue.push(initLone[start]);
        }

        for (size_t idx : initLone) {
            if (idx != initLone[start]) {
                if (tempH[idx].size() == 1) queue.push(idx);
            }
        }

        while (!queue.empty()) {
            size_t i = queue.front();
            queue.pop();
            if (tempH[i].size() == 1) {
                uint64_t x = *tempH[i].begin();
                tmpSigma.emplace_back(x, i);
                for (auto &h: {hasher0(x), hasher1(x), hasher2(x)}) {
                    tempH[h].erase(x);
                    if (tempH[h].size() == 1) queue.push(h);
                }
            }
        }
        if (sigma.size() == keys.size()) {
            sigma = std::move(tmpSigma);
            return true;
        }
    }
    return false;
}

void xorFilter::XorFilter_fixed::_assign(const std::vector<std::pair<uint64_t, size_t>> &sigma) {
    for (const auto &[x, i]: sigma) {
        data[i] = 0;
        uint64_t finger = fingerPrinter(x);
        data[i] = finger ^ data[hasher0(x)] ^ data[hasher1(x)] ^ data[hasher2(x)];
    }
}

void xorFilter::XorFilter_fixed::_newHash() {
    hasher0 = hash_function::Ranged_SimpleMixHashing(0,size/3);
    hasher1 = hash_function::Ranged_SimpleMixHashing(size/3,size/3);
    hasher2 = hash_function::Ranged_SimpleMixHashing(2*size/3,size/3);
}