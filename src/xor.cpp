//
// Created by Jayden on 03/03/2024.
//

#include <cstdlib>
#include <stack>
#include <utility>
#include <queue>
#include <iostream>

#include "xor.h"
#include "util.h"

xorFilter::XorFilter::XorFilter(int n, double fpr, bool construct) : n(n), fpr(fpr) {
    this->f = _getF();
    fingerPrinter.set(f);
    this->size = _getSize();
    this->data = new uint16_t[size];
    this->bits_per_item = (double)size/(double)n;
    keys = util::generateUniqueKeys(n);
    std::fill_n(data,size,0);
    hasher0 = hash_function::Ranged_SimpleMixHashing(0,size/3);
    hasher1 = hash_function::Ranged_SimpleMixHashing(size/3,size/3);
    hasher2 = hash_function::Ranged_SimpleMixHashing(2*size/3,size/3);
    if (construct) _insertKeys();
}

xorFilter::XorFilter::~XorFilter() {
    delete[] data;
}

void xorFilter::XorFilter::Add(const int &item) {

}

bool xorFilter::XorFilter::Member(const int &item) const {
    uint16_t finger = fingerPrinter(item);
    uint16_t h0 = hasher0(item);
    uint16_t h1 = hasher1(item);
    uint16_t h2 = hasher2(item);
    return f == (data[h0] ^ data[h1] ^ data[h2]);
}

size_t xorFilter::XorFilter::Size() const {
    return size*16;
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
    return ceil(1.23*n*f/16 + 32);
}

void xorFilter::XorFilter::_insertKeys() {
    std::stack<std::pair<int, size_t>> sigma;
    bool success = false;
    while (!success) {
        auto result = _map();
        success = result.first;
        sigma = result.second;
    }

    _assign(sigma);
}

std::pair<bool, std::stack<std::pair<int, size_t>>> xorFilter::XorFilter::_map() {
    std::vector<std::vector<uint64_t>> H(size, std::vector<uint64_t>());
    std::queue<size_t> queue;
    std::stack<std::pair<int, size_t>> sigma;

    for (const uint64_t &key : keys) {
        uint64_t h0 = hasher0(key);
        uint64_t h1 = hasher1(key);
        uint64_t h2 = hasher2(key);
        H[h0].push_back(key);
        H[h1].push_back(key);
        H[h2].push_back(key);
    }

    for (size_t i = 0; i < H.size(); i++) {
        if (H[i].size() == 1) {
            queue.push(i);
        }
    }

    while (!queue.empty()) {
        size_t i = queue.front();
        if (H[i].size() == 1) {
            uint64_t x = H[i].front();
            sigma.push({x,i});
            for (auto& h : {hasher0(x), hasher1(x), hasher2(x)}) {
                H[h].erase(std::remove(H[h].begin(), H[h].end(), x), H[h].end());
                if (H[h].size() == 1) {
                    queue.push(h);
                }
            }
        }
    }

    return {sigma.size() == keys.size(), sigma};
}

void xorFilter::XorFilter::_assign(std::stack<std::pair<int, size_t>> &sigma) {
    while (!sigma.empty()) {
        uint64_t x = sigma.top().first;
        size_t i = sigma.top().second;
        sigma.pop();
        uint64_t finger = fingerPrinter(x);
        data[i] = finger ^ data[hasher0(x) ^ hasher1(x) ^ hasher2(x)];
    }
}


