//
// Created by Jayden on 11/02/2024.
//

#include "hash_function.h"
#include "util.h"

namespace hash_function {
    SimpleMixHashing::SimpleMixHashing() {
        this->seed = (static_cast<uint64_t>(random()) << 32) | random();
    }

    uint64_t SimpleMixHashing::murmur64(uint64_t h) {
        h ^= h >> 33;
        h *= UINT64_C(0xff51afd7ed558ccd);
        h ^= h >> 33;
        h *= UINT64_C(0xc4ceb9fe1a85ec53);
        h ^= h >> 33;
        return h;
    }

    uint64_t SimpleMixHashing::operator()(uint64_t key) const {
        return murmur64(key + seed);
    }

    FingerPrinting::FingerPrinting() {
        this->seed = (static_cast<uint64_t>(random()) << 32) | random();
    }

    void FingerPrinting::set(int bit) {
        this->outputBit = bit;
    }

    // https://github.com/aappleby/smhasher/tree/master
    uint64_t FingerPrinting::murmur64(uint64_t h) {
        h ^= h >> 33;
        h *= UINT64_C(0xff51afd7ed558ccd);
        h ^= h >> 33;
        h *= UINT64_C(0xc4ceb9fe1a85ec53);
        h ^= h >> 33;
        return h;
    }

    uint64_t FingerPrinting::trimToOutputBits(uint64_t hash) const {
        if (outputBit == 64) {
            return hash;
        }
        uint64_t mask = (1ULL << outputBit) - 1;
        return hash & mask;
    }

    uint64_t FingerPrinting::operator()(uint64_t key) const {
        uint64_t hash = murmur64(key + seed);
        return trimToOutputBits(hash);
    }

    Ranged_SimpleMixHashing::Ranged_SimpleMixHashing() {}

    Ranged_SimpleMixHashing::Ranged_SimpleMixHashing(uint64_t start, uint64_t size) : start(start), size(size) {}

    uint64_t Ranged_SimpleMixHashing::operator()(uint64_t key) const {
        uint64_t hashed = hasher(key);
        return start + util::fastRange64(hashed, size);
    }
}
