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
#include "../src/cuckoo.h"
#include "../src/hash_function.h"
#include "../src/CSVWriter.h"

using namespace std;
using namespace hash_function;
using namespace bloomFilter;
using namespace cuckooFilter;
using namespace chrono;

//struct Sample {
//    string name = "";
//    string type = "";
//    uint64_t start = 0;
//    uint64_t finish = 0;
//    uint64_t time = 0;
//    uint64_t bytes = 0;
//
//
//    string csv() const {
//        std::ofstream result;
//        result.open("./benchmark.csv");
//        result << "Test Writing\n";
//        result << "A,B,C,\n";
//        result.close();
//        return "";
//    }
//};

void writeHeader(CSVWriter &c, const std::vector<std::string>& row) {
    c.writeRow(row);
}

void write(CSVWriter &c, const std::string& filterType, double x_axis, double y_axis,
           double add, int iteration, bool iter) {
    if (iter) {
        std::vector<std::string> row = {
                filterType,
                std::to_string(x_axis),
                std::to_string(y_axis),
                std::to_string(add),
                std::to_string(iter)
        };
        c.writeRow(row);
    } else {
        std::vector<std::string> row = {
                filterType,
                std::to_string(x_axis),
                std::to_string(y_axis),
                std::to_string(add),
                std::to_string(iter)
        };
        c.writeRow(row);
    }
}

void time() {
//    auto now = high_resolution_clock::now();
//    auto a = now.time_since_epoch();
    using Clock = high_resolution_clock;
    long long start;
    long long end;
    for (int i = 0; i < 1; i++) {
//        cout << "\n\nIteration #: " << i + 1 << endl;
//        start = Clock::now().time_since_epoch().count();
//        BloomFilter b(10000000,0.01, true);
//        end = Clock::now().time_since_epoch().count();
//        cout << "Initialisation:\n\n" << end - start << "(ns)" << endl;
//
//        b.Info();
//
//        start = Clock::now().time_since_epoch().count();
//        b.Add(1);
//        end = Clock::now().time_since_epoch().count();
//        cout << "Insertion of a single key:\n" << end - start << "(ns)" << endl;


        start = Clock::now().time_since_epoch().count();
        CuckooFilter c(1000000,0.01, true);
        end = Clock::now().time_since_epoch().count();
        cout << "Initialisation:\n\n" << end - start << "(ns)" << endl;

        c.Info();

        start = Clock::now().time_since_epoch().count();
        c.Add(1);
        end = Clock::now().time_since_epoch().count();
        cout << "Insertion of a single key:\n" << end - start << "(ns)" << endl;
    }
}

void fpr_bpi() {
    CSVWriter csv("bpi_fpr.csv");
    int n = 1000000;
    double fpr = 0.0001;

    writeHeader(csv, {"FilterType", "n", "fpr", "bpi","iteration"});
    for (int i = 0; i < 4; i++) {
        BloomFilter B(n,fpr,true);
        write(csv, "Bloom", n, fpr, B.Bpi(), -1, false);
        CuckooFilter C(n, fpr, true);
        write(csv, "Cuckoo", n, fpr, C.Bpi(), -1, false);
        fpr *= 10;
    }
}

void benchmark() {
    fpr_bpi();
//    time();
}


int main() {
    benchmark();
    return 0;
}