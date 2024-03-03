//
// Created by Jayden on 28/02/2024.
//

#ifndef FILTERS_UTIL_H
#define FILTERS_UTIL_H

#include <cstdlib>
#include <vector>
#include <random>

#include "util.h"

class util {
public:
    static uint64_t fastRange64(uint64_t word, uint64_t p);
    static size_t fastRangeSize(size_t word, size_t p);
    static std::vector<uint64_t> generateUniqueKeys(size_t count);
};


#endif //FILTERS_UTIL_H
