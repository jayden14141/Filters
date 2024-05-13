//
// Created by Jayden on 13/05/2024.
//

#include "xor_test.h"

#include <cstdlib>
#include <stack>
#include <utility>
#include <queue>
#include <iostream>
#include <unordered_set>

#include "xor.h"
#include "util.h"

xorFilter::XorFilter_test::XorFilter_test(int n, double fpr, double scale, bool construct, bool local) :scale(scale), n(n), fpr(fpr), local(local) {
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

xorFilter::XorFilter_test::~XorFilter_test() {
    delete[] data;
}

bool xorFilter::XorFilter_test::Add(std::vector<uint64_t> &keys) {
    this->keys = keys;
    return _insertKeys();
}

void xorFilter::XorFilter_test::AddAll(std::vector<uint64_t> &keys) {
    this->keys = keys;
    _insertKeys();
}

bool xorFilter::XorFilter_test::Member(const uint64_t &item) const {
    uint16_t finger = fingerPrinter(item);
    uint16_t h0 = hasher0(item);
    uint16_t h1 = hasher1(item);
    uint16_t h2 = hasher2(item);
    return finger == (data[h0] ^ data[h1] ^ data[h2]);
}

size_t xorFilter::XorFilter_test::Size() const {
    return size*f;
}

double xorFilter::XorFilter_test::Fpr() const {
    return fpr;
}

double xorFilter::XorFilter_test::Bpi() const {
    return bits_per_item;
}

void xorFilter::XorFilter_test::Info() const {
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

int xorFilter::XorFilter_test::_getF() const {
    return ceil(log2(1/fpr));
}

size_t xorFilter::XorFilter_test::_getSize() const {
    return ceil(scale* n + 32);
}

bool xorFilter::XorFilter_test::_insertKeys() {
    std::vector<std::pair<uint64_t, size_t>> sigma;
    sigma.reserve(n);
    bool success = false;
    success = _map(sigma);
    if (!success) return false;
    else return true;
//    while (!success) {
//        attempt++;
//        _newHash();
//        sigma.clear();
//        sigma.reserve(n);
//        success = _map(sigma);
//    }

    _assign(sigma);
//    if (success) {
//        _assign(sigma);
//    } else {
//        std::cout << "Not working" << std::endl;
//    }
}

bool xorFilter::XorFilter_test::_map(std::vector<std::pair<uint64_t, size_t>> &sigma) {
    std::vector<size_t> counts(size, 0);
    std::vector<std::vector<uint64_t>> H(size);
    std::queue<size_t> queue;
    for (const uint64_t &key: keys) {
        uint64_t h0 = hasher0(key);
        uint64_t h1 = hasher1(key);
        uint64_t h2 = hasher2(key);
        H[h0].emplace_back(key);
        H[h1].emplace_back(key);
        H[h2].emplace_back(key);
        counts[h0]++;
        counts[h1]++;
        counts[h2]++;
    }

    for (size_t i = 0; i < counts.size(); i++) {
        if (counts[i] == 1){
            queue.push(i);
        }
    }

    while (!queue.empty()) {
        size_t i = queue.front();
        queue.pop();
        if (counts[i] == 1) {
            uint64_t x = H[i].front();
            sigma.emplace_back(x, i);
            for (auto &h: {hasher0(x), hasher1(x), hasher2(x)}) {
//              H[h].erase(x);
                auto &keysAtH = H[h];
                keysAtH.erase(std::remove(keysAtH.begin(), keysAtH.end(), x), keysAtH.end());
                if (--counts[h] == 1) queue.push(h);
            }
        }
    }
    return sigma.size() == keys.size();
}

void xorFilter::XorFilter_test::_assign(const std::vector<std::pair<uint64_t, size_t>> &sigma) {

//    for (const auto &[x, i]: sigma) {
//        data[i] = 0;
//        uint64_t finger = fingerPrinter(x);
//        data[i] = finger ^ data[hasher0(x)] ^ data[hasher1(x)] ^ data[hasher2(x)];
//    }
    for (auto it = sigma.rbegin(); it != sigma.rend(); it++) {
        const auto &[x, i] = *it;
        uint64_t finger = fingerPrinter(x);
        data[i] = finger ^ data[hasher0(x)] ^ data[hasher1(x)] ^ data[hasher2(x)];
    }
}

void xorFilter::XorFilter_test::_newHash() {
    if (local) {
        hasher0 = hash_function::Ranged_SimpleMixHashing(0,size/3);
        hasher1 = hash_function::Ranged_SimpleMixHashing(size/3,size/3);
        hasher2 = hash_function::Ranged_SimpleMixHashing(2*size/3,size/3);
    } else {
        hasher0 = hash_function::Ranged_SimpleMixHashing(0,size);
        hasher1 = hash_function::Ranged_SimpleMixHashing(0,size);
        hasher2 = hash_function::Ranged_SimpleMixHashing(0,size);
    }
}