//
// Created by Jayden on 28/02/2024.
//

#include <cstdlib>
#include <unordered_set>
#include "util.h"

// Calculates the modulo operation in a faster way
uint64_t util::fastRange64(uint64_t word, uint64_t p)  {
    // http://lemire.me/blog/2016/06/27/a-fast-alternative-to-the-modulo-reduction/
#ifdef __SIZEOF_INT128__ // then we know we have a 128-bit int
    return (uint64_t)(((__uint128_t)word * (__uint128_t)p) >> 64);
#elif defined(_MSC_VER) && defined(_WIN64)
    // supported in Visual Studio 2005 and better
        uint64_t highProduct;
        _umul128(word, p, &highProduct); // ignore output
        return highProduct;
        unsigned __int64 _umul128(
            unsigned __int64 Multiplier,
            unsigned __int64 Multiplicand,
            unsigned __int64 *HighProduct
        );
    #else
        return word % p; // fallback
#endif // __SIZEOF_INT128__
}

size_t util::fastRangeSize(size_t word, size_t p) {
    return (size_t)util::fastRange64(word, p);
}

int util::fastRangeInt(size_t word, size_t p) {
    return (int)util::fastRange64(word, p);
}


// Generates large amount of unique keys without collision
std::vector<uint64_t> util::generateUniqueKeys(size_t count) {
    std::vector<uint64_t> keys;
    keys.reserve(count);

    std::unordered_set<uint64_t> seenKeys;
    seenKeys.reserve(count);

    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<uint64_t> dis;

    while (keys.size() < count) {
        uint64_t key = dis(gen);
        if (seenKeys.insert(key).second) {
            keys.push_back(key);
        }
    }
//
//    for (size_t i = 0; i < count; ++i) {
//    }

    return keys;
}
