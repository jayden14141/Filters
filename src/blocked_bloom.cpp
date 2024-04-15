//
// Created by Jayden on 09/04/2024.
//

#include "blocked_bloom.h"
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
#include <cstdint>
#include <algorithm>
#include <arm_neon.h>
#include <Accelerate/Accelerate.h>

blockedBloomFilter::BlockedBloom::BlockedBloom(int n, double fpr, bool construct)
    : buckets(nullptr), fpr(fpr), n(n) {
    bits_per_item = _getBpi();
    size = _getSize();

    bucketCount = ceil((bits_per_item * n / 64) / sizeof(Bucket));
    size_t alloc_size = bucketCount * sizeof(Bucket);
    buckets = static_cast<Bucket*>(malloc(alloc_size));

    memset(buckets, 0, alloc_size);
}

blockedBloomFilter::BlockedBloom::~BlockedBloom() {
    free(buckets);
    buckets = nullptr;
}

void blockedBloomFilter::BlockedBloom::Add(const uint64_t &item) {
    auto hashed = hasher(item);
    uint32_t bucket_index = util::fastRangeSize(hashed, bucketCount);
    uint16x8_t mask = _makeMask(static_cast<uint16_t>(hashed & 0xFFFF));
    uint16x8_t bucket = vld1q_u16(reinterpret_cast<uint16_t*>(&buckets[bucket_index]));
    bucket = vorrq_u16(bucket, mask);
    vst1q_u16(reinterpret_cast<uint16_t*>(&buckets[bucket_index]), bucket);
}

void blockedBloomFilter::BlockedBloom::AddAll(std::vector<uint64_t> &keys) {
    for (auto key: keys) {
        Add(key);
    }
}

bool blockedBloomFilter::BlockedBloom::Member(const uint64_t &item) {
    auto hashed = hasher(item);
    uint32_t bucket_index = util::fastRangeSize(hashed, bucketCount);
    uint16x8_t mask = _makeMask(static_cast<uint16_t>(hashed & 0xFFFF));
    uint16x8_t bucket = vld1q_u16(reinterpret_cast<uint16_t*>(&buckets[bucket_index]));

    uint16x8_t compare = vceqq_u16(vandq_u16(bucket,mask) , mask);

    uint64x1_t reduced = vpmin_u32(vget_low_u16(compare), vget_high_u16(compare));
    return vget_lane_u16(reduced, 0) == 0xFFFF;
}

size_t blockedBloomFilter::BlockedBloom::Size() const {
    return bucketCount * sizeof(Bucket);
}

double blockedBloomFilter::BlockedBloom::Fpr() const {
    return fpr;
}

double blockedBloomFilter::BlockedBloom::Bpi() const {
    return bits_per_item;
}

void blockedBloomFilter::BlockedBloom::Info() const {
    std::cout << "Blocked Bloom Filter" << std::endl;
    std::cout << "------------------------------- \n" << std::endl;
    std::cout << "Number of keys inserted  : " << n << std::endl;
    std::cout << "False positive rate  : " << fpr << std::endl;
    std::cout << "Number of Hash functions used  : " << m << std::endl;
    std::cout << "Size of a filter in bits  : " << (int)ceil(size) << std::endl;
    std::cout << "Bits per Item (Theoretical) : " << log2(1/fpr) << std::endl;
    std::cout << "Bits per Item : " << Bpi() << std::endl;
    std::cout << "Space overhead : " << 100 * (bits_per_item - log2(1/fpr)) / log2(1/fpr) << "% " << std::endl;
}

double blockedBloomFilter::BlockedBloom::_getBpi() const {
    return -log2(fpr) / log(2);
}

size_t blockedBloomFilter::BlockedBloom::_getSize() const {
    return ceil(bits_per_item * n);
}

void blockedBloomFilter::BlockedBloom::_insertKeys() {
    std::vector<uint64_t> random = util::generateUniqueKeys(n);
    for (int i = 0; i < n; i++) {
        Add((int)random[i]);
    }
}

uint16x8_t blockedBloomFilter::BlockedBloom::_makeMask(const uint16_t hashed) {
    const uint16x8_t ones = {1,1,1,1,1,1,1,1};
    const uint16x8_t rehash = {0x79d8, 0xe722, 0xf2fb, 0x21ec, 0x121b, 0x2302, 0x705a, 0x6e87};
    uint16x8_t hash_data = {hashed,hashed,hashed,hashed,hashed,hashed,hashed,hashed};
    uint16x8_t mask = vmulq_u16(hash_data, rehash);
    mask = vshrq_n_u16(mask,12);
    mask = vshlq_u16(ones, vreinterpretq_s16_u16(mask));
    return mask;
}


