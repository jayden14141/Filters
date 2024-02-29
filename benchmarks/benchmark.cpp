//
// Created by Jayden on 20/02/2024.
//


#include <algorithm>
#include <chrono>
#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>

#include "../src/bloom.h"
#include "../src/hash_function.h"

using namespace std;
using namespace hash_function;
using namespace bloomFilter;
using namespace chrono;

struct Sample {
    string name = "";
    string type = "";
    uint64_t start = 0;
    uint64_t finish = 0;
    uint64_t time = 0;
    uint64_t bytes = 0;


    string csv() const {
        std::ofstream result;
        result.open("./benchmark.csv");
        result << "Test Writing\n";
        result << "A,B,C,\n";
        result.close();
    }
};

void benchmark() {
//    auto now = high_resolution_clock::now();
//    auto a = now.time_since_epoch();
    using Clock = high_resolution_clock;
    long long start;
    long long end;
    for (int i = 0; i < 5; i++) {
        cout << "\n\nIteration #: " << i + 1 << endl;
        start = Clock::now().time_since_epoch().count();
        BloomFilter b(100000000,0.01, true);
        end = Clock::now().time_since_epoch().count();
        cout << "Initialisation:\n" << end - start << "(ns)" << endl;

        b.Info();

        start = Clock::now().time_since_epoch().count();
        b.Add(1);
        end = Clock::now().time_since_epoch().count();
        cout << "Insertion of a single key:\n" << end - start << "(ns)" << endl;
    }

}

int main() {
    benchmark();
    return 0;
}