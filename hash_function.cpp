//
// Created by Jayden on 11/02/2024.
//

#include "hash_function.h"

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
}
