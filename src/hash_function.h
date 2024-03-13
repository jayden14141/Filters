//
// Created by Jayden on 11/02/2024.
//

#ifndef FILTERS_HASH_FUNCTION_H
#define FILTERS_HASH_FUNCTION_H

#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <string>
#include <random>


namespace hash_function {
// See Mikkel Thorup,
// "High Speed Hashing for Integers and Strings"

    class SimpleMixHashing {
    public:
        uint64_t seed;

        SimpleMixHashing();
        static uint64_t murmur64(uint64_t h);
        uint64_t operator()(uint64_t key) const;
    };

    class FingerPrinting {
    public:
        uint64_t seed;
        int outputBit = 64;
        void set(int bit);
        FingerPrinting();
        uint64_t operator()(uint64_t key) const;

        static uint64_t murmur64(uint64_t h);
        uint64_t trimToOutputBits(uint64_t hash) const;
    };
}
#endif //FILTERS_HASH_FUNCTION_H
