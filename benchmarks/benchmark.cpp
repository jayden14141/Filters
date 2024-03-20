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
                std::to_string(iteration)
        };
        c.writeRow(row);
    } else {
        std::vector<std::string> row = {
                filterType,
                std::to_string(x_axis),
                std::to_string(y_axis),
                std::to_string(add),
                std::to_string(iteration)
        };
        c.writeRow(row);
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

        write(csv, "Lower Bound", n, fpr, log2(1/fpr), -1, false);
        fpr *= 10;
    }
}

void n_buildTime() {
    CSVWriter csv("buildTime_n.csv");
    int n = 10000;
    double fpr = 0.01;

    using Clock = high_resolution_clock;
    long long start;
    long long end;
    double interval;

    writeHeader(csv, {"FilterType", "n", "constTime", "fpr","iteration"});
    for (int i = 0; i < 4; i++) {
        start = Clock::now().time_since_epoch().count();
        BloomFilter B(n,fpr, true);
        end = Clock::now().time_since_epoch().count();
        interval = static_cast<double> (((double)end - (double)start) / n);
        write(csv, "Bloom", n, interval, fpr, -1, false);


        start = Clock::now().time_since_epoch().count();
        CuckooFilter C(n, fpr, true);
        end = Clock::now().time_since_epoch().count();
        interval = static_cast<double> (((double)end - (double)start) / n);
        write(csv, "Cuckoo", n, interval, fpr, -1, false);

        n *= 10;
    }
}

void target_actual_fpr() {
    CSVWriter csv("target_actual_fpr.csv");
    int n = 2000000;
    double fpr = 0.01;

    writeHeader(csv, {"FilterType", "actual", "target", "n","iteration"});
    for (int i = 0; i < 5; i++) {
        BloomFilter B(n,fpr,false);
        vector<uint64_t> keys = util::generateUniqueKeys(n);
        int count = 0;
        // Add half of the random generated keys
        for (int i = 0; i < n / 2; i++) {
            B.Add(keys[i]);
        }
        // Test false positive rate by querying the other half that hasn't been added
        for (int i = n / 2; i < n; i++) {
            if (B.Member(keys[i])) count++;
        }
        double actual =  (double)count / ((double)n / 2);
        write(csv, "Bloom", actual, fpr, n, i, true);

        CuckooFilter C(n, fpr, false);
        count = 0;
        for (int i = 0; i < n / 2; i++) {
            C.Add(keys[i]);
        }
        // Test false positive rate by querying the other half that hasn't been added
        for (int i = n / 2; i < n; i++) {
            if (C.Member(keys[i])) count++;
        }
        write(csv, "Cuckoo", actual, fpr, n, i, true);
    }
}

void a() {
    for (int i = 0; i < 100; i++) {
        int n = 1000000;
        CuckooFilter c (n, 0.01, true);
        std::cout << c.eviction << std::endl;
    }
}

void benchmark() {
//    fpr_bpi();
//    n_buildTime();
//    target_actual_fpr();
    a();
}


int main() {
    benchmark();
    return 0;
}