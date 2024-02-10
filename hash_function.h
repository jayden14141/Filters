//
// Created by Jayden on 04/02/2024.
//

#ifndef FILTERS_HASH_FUNCTION_H
#define FILTERS_HASH_FUNCTION_H

#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <sys/types.h>
#include <tkDecls.h>

#include <string>

#include <random>

namespace hash_function {
// See Mikkel Thorup,
// "High Speed Hashing for Integers and Strings"

class SimpleMixHashing {
  public:
    uint64_t seed;

    SimpleMixHashing() {
        seed = (static_cast<uint64_t>(random()) << 32) | random();
    }

    inline static uint64_t murmur64(uint64_t h) {
        h ^= h >> 33;
        h *= UINT64_C(0xff51afd7ed558ccd);
        h ^= h >> 33;
        h *= UINT64_C(0xc4ceb9fe1a85ec53);
        h ^= h >> 33;
        return h;
    }

    inline uint64_t operator()(uint64_t key) const {
        return murmur64(key + seed);
    }
};

class

}

#endif //FILTERS_HASH_FUNCTION_H
