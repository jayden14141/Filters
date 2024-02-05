//
// Created by Jayden on 04/02/2024.
//

#ifndef FILTERS_HASH_FUNCTION_H
#define FILTERS_HASH_FUNCTION_H

#include <cstdint>
#include <ctime>
#include <random>
#include <tkDecls.h>

namespace hash_function {
// See Mikkel Thorup,
// "High Speed Hashing for Integers and Strings"

class Modulo {
  public:
    uint64_t seed1, seed2;
    uint64_t q = 100;
    uint64_t m = 20;

    Modulo() {
        seed1 = (static_cast<uint64_t>(random()) << 32) | random();
        seed2 = (static_cast<uint64_t>(random()) << 32) | random();
    }

    inline static uint64_t mul_mod_prime(uint64_t a, uint64_t b, uint64_t x, uint64_t q) {
        uint64_t p = 0;
        //TODO : Consider potential overflow when calculating ax
        uint64_t ax_b = a*x+b;
        p >>= q;
        p -= 1;
        uint64_t y = (ax_b&p) + (ax_b>>q);
        if (y >= p) y -= p;
       return y;
    }

    inline uint64_t operator()(uint64_t key) const {
        uint64_t ax_mod_p = mul_mod_prime(seed1, seed2, key, q);
        return 1 + ax_mod_p % m;
    }
};

}

#endif //FILTERS_HASH_FUNCTION_H
