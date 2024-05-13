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
#include "../src/blocked_bloom.h"
#include "../src/blocked_bloom_test.h"
#include "../src/cuckoo.h"
#include "../src/cuckoo_test.h"
#include "../src/cuckoo_test_fingerprint.h"
#include "../src/xor_test.h"
#include "../src/hash_function.h"
#include "../src/xor.h"
#include "../src/CSVWriter.h"

using namespace std;
using namespace hash_function;
using namespace bloomFilter;
using namespace blockedBloomFilter;
using namespace cuckooFilter;
using namespace xorFilter;
using namespace chrono;


//-----------------------------------------------------------------------------------------------------
//------------------------------CSV Writing Helper Functions     --------------------------------------
//-----------------------------------------------------------------------------------------------------


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

//-----------------------------------------------------------------------------------------------------
//---------------------------------         Benchmarks        -----------------------------------------
//-----------------------------------------------------------------------------------------------------

void fpr_bpi() {
    CSVWriter csv("bpi_fpr.csv");
    int n = 10000;
    vector<double> fpr = {0.0001, 0.0005, 0.001, 0.005, 0.01, 0.05, 0.1, 0.5};

    writeHeader(csv, {"FilterType", "n", "fpr", "bpi","iteration"});
    for (int i = 0; i < 1; i++) {
        for (int j = 0; j < fpr.size(); j++) {
            BloomFilter B(n,fpr[j],false);
            write(csv, "Bloom", n, fpr[j], B.Bpi(), -1, false);
            CuckooFilter C(n, fpr[j], false);
            write(csv, "Cuckoo", n, fpr[j], C.Bpi(), -1, false);
            XorFilter X(n, fpr[j], false);
            write(csv, "Xor", n, fpr[j], X.Bpi(), -1, false);
            BlockedBloom BB(n,fpr[j],false);
            write(csv, "Blocked Bloom", n, fpr[j], BB.Bpi(), -1, false);

            write(csv, "Lower Bound", n, fpr[j], log2(1/fpr[j]), -1, false);
        }
    }
}

void lf_posQueryTime() {
    CSVWriter csv("lf_posQueryTime.csv");
    int n = 1000000;
    int k = 10000;
    vector<double> loadFactor = {0, 0.25, 0.5, 0.75, 1};
    double fpr = 0.01;

    int size_25 = n * 0.25;
    int size_50 = n * 0.5;
    int size_75 = n * 0.75;

    std::vector<uint64_t> keys = util::generateUniqueKeys(n);
    std::vector<uint64_t> keys_0lf(keys.begin(), keys.begin() + k);
    std::vector<uint64_t> keys_25lf(keys.begin(), keys.begin() + size_25);
    std::vector<uint64_t> keys_50lf(keys.begin(), keys.begin() + size_50);
    std::vector<uint64_t> keys_75lf(keys.begin(), keys.begin() + size_75);
    std::vector<std::vector<uint64_t>> keyLf = {keys_0lf, keys_25lf, keys_50lf, keys_75lf, keys};


    using Clock = high_resolution_clock;
    long long start;
    long long end;
    double interval;

    writeHeader(csv, {"FilterType", "lf", "queryTime", "n","iteration"});
    for (int i = 0; i < 5; i++) {
        for (int j = 1; j <= 10; j++) {
            BlockedBloom BB(n, fpr, false);
            BB.AddAll(keyLf[i]);
            start = Clock::now().time_since_epoch().count();
            for (int t = 0; t < k; t++)  BB.Member(keys[t]);
            end = Clock::now().time_since_epoch().count();
            interval = static_cast<double> (((double)end - (double)start) / k);
            write(csv, "Blocked Bloom", loadFactor[i], interval, n, j, true);


            BloomFilter B(n,fpr, false);
            B.AddAll(keyLf[i]);
            start = Clock::now().time_since_epoch().count();
            for (int t = 0; t < k; t++) B.Member(keys[t]);
            end = Clock::now().time_since_epoch().count();
            interval = static_cast<double> (((double)end - (double)start) / k);
            write(csv, "Bloom", loadFactor[i], interval, n, j, true);

            CuckooFilter C(n, fpr, false);
            C.AddAll(keyLf[i]);
            start = Clock::now().time_since_epoch().count();
            for (int t = 0; t < k; t++)  C.Member(keys[t]);
            end = Clock::now().time_since_epoch().count();
            interval = static_cast<double> (((double)end - (double)start) / k);
            write(csv, "Cuckoo", loadFactor[i], interval, n, j, true);


            XorFilter X(n, fpr, false);
            X.AddAll(keyLf[i]);
            start = Clock::now().time_since_epoch().count();
            for (int t = 0; t < k; t++)  X.Member(keys[t]);
            end = Clock::now().time_since_epoch().count();
            interval = static_cast<double> (((double)end - (double)start) / k);
            write(csv, "Xor", loadFactor[i], interval, n, j, true);
        }

//        n *= 10;
    }
}

void lf_halfQueryTime() {
    CSVWriter csv("lf_halfQueryTime.csv");
    int n = 1000000;
    int k = 10000;
    vector<double> loadFactor = {0, 0.25, 0.5, 0.75, 1};
    double fpr = 0.01;

    int size_25 = n * 0.25;
    int size_50 = n * 0.5;
    int size_75 = n * 0.75;

    std::vector<uint64_t> keys = util::generateUniqueKeys(n + k);
    std::vector<uint64_t> keysNeg(keys.begin() + n, keys.end());
    std::vector<uint64_t> keys_0lf(keys.begin(), keys.begin() + k);
    std::vector<uint64_t> keys_25lf(keys.begin(), keys.begin() + size_25);
    std::vector<uint64_t> keys_50lf(keys.begin(), keys.begin() + size_50);
    std::vector<uint64_t> keys_75lf(keys.begin(), keys.begin() + size_75);
    std::vector<uint64_t> keys_100lf(keys.begin(), keys.begin() + n);
    std::vector<std::vector<uint64_t>> keyLf = {keys_0lf, keys_25lf, keys_50lf, keys_75lf, keys_100lf};

    std::vector<uint64_t> testSet(keys.begin(), keys.begin() + k/2);
    for (int i = 0; i < k/2; i++) testSet.push_back(keysNeg[i]);


    using Clock = high_resolution_clock;
    long long start;
    long long end;
    double interval;

    writeHeader(csv, {"FilterType", "lf", "queryTime", "n","iteration"});
    for (int i = 0; i < 5; i++) {
        for (int j = 1; j <= 10; j++) {
//            BloomFilter B(n,fpr, false);
//            B.AddAll(keyLf[i]);
//            start = Clock::now().time_since_epoch().count();
//            for (int t = 0; t < k; t++) B.Member(testSet[t]);
//            end = Clock::now().time_since_epoch().count();
//            interval = static_cast<double> (((double)end - (double)start) / k);
//            write(csv, "Bloom", loadFactor[i], interval, n, j, true);
////
//            CuckooFilter C(n, fpr, false);
//            C.AddAll(keyLf[i]);
//            start = Clock::now().time_since_epoch().count();
//            for (int t = 0; t < k; t++)  C.Member(testSet[t]);
//            end = Clock::now().time_since_epoch().count();
//            interval = static_cast<double> (((double)end - (double)start) / k);
//            write(csv, "Cuckoo", loadFactor[i], interval, n, j, true);
//
//
//            BlockedBloom BB(n, fpr, false);
//            BB.AddAll(keyLf[i]);
//            start = Clock::now().time_since_epoch().count();
//            for (int t = 0; t < k; t++)  BB.Member(testSet[t]);
//            end = Clock::now().time_since_epoch().count();
//            interval = static_cast<double> (((double)end - (double)start) / k);
//            write(csv, "Blocked Bloom", loadFactor[i], interval, n, j, true);
//
            XorFilter X(n, fpr, false);
            X.AddAll(keyLf[i]);
            start = Clock::now().time_since_epoch().count();
            for (int t = 0; t < k; t++)  X.Member(testSet[t]);
            end = Clock::now().time_since_epoch().count();
            interval = static_cast<double> (((double)end - (double)start) / k);
            write(csv, "Xor", loadFactor[i], interval, n, j, true);
        }

//        n *= 10;
    }
}

void lf_negQueryTime() {
    CSVWriter csv("lf_negQueryTime.csv");
    int n = 1000000;
    int k = 10000;
    vector<double> loadFactor = {0, 0.25, 0.5, 0.75, 1};
    double fpr = 0.01;

    int size_25 = n * 0.25;
    int size_50 = n * 0.5;
    int size_75 = n * 0.75;

    std::vector<uint64_t> keys = util::generateUniqueKeys(n + k);
    std::vector<uint64_t> keysNeg(keys.begin() + n, keys.end());
    std::vector<uint64_t> keys_0lf(keys.begin(), keys.begin() + k);
    std::vector<uint64_t> keys_25lf(keys.begin(), keys.begin() + size_25);
    std::vector<uint64_t> keys_50lf(keys.begin(), keys.begin() + size_50);
    std::vector<uint64_t> keys_75lf(keys.begin(), keys.begin() + size_75);
    std::vector<uint64_t> keys_100lf(keys.begin(), keys.begin() + n);
    std::vector<std::vector<uint64_t>> keyLf = {keys_0lf, keys_25lf, keys_50lf, keys_75lf, keys_100lf};


    using Clock = high_resolution_clock;
    long long start;
    long long end;
    double interval;

    writeHeader(csv, {"FilterType", "lf", "queryTime", "n","iteration"});
    for (int i = 0; i < 5; i++) {
        for (int j = 1; j <= 10; j++) {
            BloomFilter B(n,fpr, false);
            B.AddAll(keyLf[i]);
            start = Clock::now().time_since_epoch().count();
            for (int t = 0; t < k; t++) B.Member(keysNeg[t]);
            end = Clock::now().time_since_epoch().count();
            interval = static_cast<double> (((double)end - (double)start) / k);
            write(csv, "Bloom", loadFactor[i], interval, n, j, true);


            CuckooFilter C(n, fpr, false);
            C.AddAll(keyLf[i]);
            start = Clock::now().time_since_epoch().count();
            for (int t = 0; t < k; t++)  C.Member(keysNeg[t]);
            end = Clock::now().time_since_epoch().count();
            interval = static_cast<double> (((double)end - (double)start) / k);
            write(csv, "Cuckoo", loadFactor[i], interval, n, j, true);


            BlockedBloom BB(n, fpr, false);
            BB.AddAll(keyLf[i]);
            start = Clock::now().time_since_epoch().count();
            for (int t = 0; t < k; t++)  BB.Member(keysNeg[t]);
            end = Clock::now().time_since_epoch().count();
            interval = static_cast<double> (((double)end - (double)start) / k);
            write(csv, "Blocked Bloom", loadFactor[i], interval, n, j, true);

            XorFilter X(n, fpr, false);
            X.AddAll(keyLf[i]);
            start = Clock::now().time_since_epoch().count();
            for (int t = 0; t < k; t++)  X.Member(keysNeg[t]);
            end = Clock::now().time_since_epoch().count();
            interval = static_cast<double> (((double)end - (double)start) / k);
            write(csv, "Xor", loadFactor[i], interval, n, j, true);
        }

//        n *= 10;
    }
}

void n_buildTime() {
    CSVWriter csv("buildTime_n.csv");
    int n = 1000000;
    double fpr = 0.1;

    using Clock = high_resolution_clock;
    long long start;
    long long end;
    double interval;

    writeHeader(csv, {"FilterType", "n", "constTime", "fpr","iteration"});
    for (int i = 0; i < 1; i++) {
        std::vector<uint64_t> keys = util::generateUniqueKeys(n);
        for (int j = 1; j <= 10; j++) {
//            BloomFilter B(n,fpr, false);
//            start = Clock::now().time_since_epoch().count();
//            B.AddAll(keys);
//            end = Clock::now().time_since_epoch().count();
//            interval = static_cast<double> (((double)end - (double)start) / n);
//            write(csv, "Bloom", n, interval, fpr, j, true);
//
//            CuckooFilter C(n, fpr, false);
//            start = Clock::now().time_since_epoch().count();
//            C.AddAll(keys);
//            end = Clock::now().time_since_epoch().count();
//            interval = static_cast<double> (((double)end - (double)start) / n);
//            write(csv, "Cuckoo", n, interval, fpr, j, true);
////
//            BlockedBloom BB(n, fpr, false);
//            start = Clock::now().time_since_epoch().count();
//            BB.AddAll(keys);
//            end = Clock::now().time_since_epoch().count();
//            interval = static_cast<double> (((double)end - (double)start) / n);
//            write(csv, "Blocked Bloom", n, interval, fpr, j, true);


            XorFilter X(n, fpr, false);
            start = Clock::now().time_since_epoch().count();
            X.AddAll(keys);
            end = Clock::now().time_since_epoch().count();
            interval = static_cast<double> (((double)end - (double)start) / n);
            write(csv, "Xor", n, interval, fpr, j, true);

        }

//        n *= 10;
    }
}

void target_actual_fpr() {
    CSVWriter csv("target_actual_fpr.csv");
    int n = 2000000;
    int size = ceil(n/2);
    double fpr = 0.01;

    writeHeader(csv, {"FilterType", "actual", "target", "n","iteration"});
    for (int i = 0; i < 15; i++) {
        BloomFilter B(size,fpr,false);
        CuckooFilter C(size, fpr, false);
        BlockedBloom BB(size, fpr, false);
        XorFilter X(size, fpr, false);
//        XorFilter_fixed XF(n, fpr, false);
        vector<uint64_t> keys = util::generateUniqueKeys(n);
        vector<uint64_t> inserted(keys.begin(), keys.begin() + ceil(n / 2));
        vector<uint64_t> notInserted(keys.begin() + ceil(n / 2), keys.end());
        int count = 0;
        // Add half of the random generated keys
        for (int i = 0; i < n / 2; i++) {
            B.Add(keys[i]);
            C.Add(keys[i]);
            BB.Add(keys[i]);
        }
        X.Add(inserted);

        // Test false positive rate by querying the other half that hasn't been added
        for (int i = n / 2; i < n; i++) {
            if (B.Member(keys[i])) count++;
        }

        double actual =  (double)count / ((double)n / 2);
        write(csv, "Bloom", actual, fpr, n, i, true);
        count = 0;

        for (int i = n / 2; i < n; i++) {
            if (C.Member(keys[i])) count++;
        }
        actual =  (double)count / ((double)n / 2);
        write(csv, "Cuckoo", actual, fpr, n, i, true);
        count = 0;
        for (int i = 0; i < n / 2; i++) {
        }

        for (int i = 0; i < notInserted.size(); i++) {
            if (BB.Member(notInserted[i])) count++;
        }
        actual =  (double)count / ((double)n / 2);
        write(csv, "Blocked Bloom", actual, fpr, n, i, true);
        count = 0;

        // Test false positive rate by querying the other half that hasn't been added
        for (int i = 0; i < notInserted.size(); i++) {
            if (X.Member(notInserted[i])) count++;
        }
        actual =  (double)count / ((double)n / 2);
        write(csv, "Xor", actual, fpr, n, i, true);
    }
}

void target_actual_fpr_fp() {
    CSVWriter csv("target_actual_fpr_fp.csv");
    int n = 2000000;
    int size = ceil(n/2);
    double fpr = 0.008;

    writeHeader(csv, {"FilterType", "actual", "target", "n","iteration"});
    for (int i = 0; i < 15; i++) {
        BloomFilter B(size,fpr,false);
        CuckooFilter C(size, fpr, false);
        BlockedBloom BB(size, fpr, false);
        XorFilter X(size, fpr, false);
//        XorFilter_fixed XF(n, fpr, false);
        vector<uint64_t> keys = util::generateUniqueKeys(n);
        vector<uint64_t> inserted(keys.begin(), keys.begin() + ceil(n / 2));
        vector<uint64_t> notInserted(keys.begin() + ceil(n / 2), keys.end());
        int count = 0;
        // Add half of the random generated keys
        for (int i = 0; i < n / 2; i++) {
            B.Add(keys[i]);
            C.Add(keys[i]);
            BB.Add(keys[i]);
        }
        X.Add(inserted);

        // Test false positive rate by querying the other half that hasn't been added
        for (int i = n / 2; i < n; i++) {
            if (B.Member(keys[i])) count++;
        }

        double actual =  (double)count / ((double)n / 2);
        write(csv, "Bloom", actual, fpr, n, i, true);
        count = 0;

        for (int i = n / 2; i < n; i++) {
            if (C.Member(keys[i])) count++;
        }
        actual =  (double)count / ((double)n / 2);
        write(csv, "Cuckoo", actual, fpr, n, i, true);
        count = 0;
        for (int i = 0; i < n / 2; i++) {
        }

        for (int i = 0; i < notInserted.size(); i++) {
            if (BB.Member(notInserted[i])) count++;
        }
        actual =  (double)count / ((double)n / 2);
        write(csv, "Blocked Bloom", actual, fpr, n, i, true);
        count = 0;

        // Test false positive rate by querying the other half that hasn't been added
        for (int i = 0; i < notInserted.size(); i++) {
            if (X.Member(notInserted[i])) count++;
        }
        actual =  (double)count / ((double)n / 2);
        write(csv, "Xor", actual, fpr, n, i, true);
    }
}

//-----------------------------------------------------------------------------------------------------
//-----------------------------        Empirical Testing      -----------------------------------------
//-----------------------------------------------------------------------------------------------------


void bB_everything() {
    int n = 2000000;
    double fpr = 0.01;
    BlockedBloom(n/2, fpr, false);
}

void bloom_fpr() {
    CSVWriter csv("lf_posQueryTime.csv");
    int n = 1000000;
    int k = 100;
    vector<double> loadFactor = {0, 0.25, 0.5, 0.75, 1};
    double fpr = 0.01;

    int size_25 = n * 0.25;
    int size_50 = n * 0.5;
    int size_75 = n * 0.75;

    std::vector<uint64_t> keys = util::generateUniqueKeys(n);

    writeHeader(csv, {"FilterType", "lf", "queryTime", "n","iteration"});
}

void bloom_m_fpr() {
    CSVWriter csv("bloom_m_fpr.csv");
    vector<double> targetFpr = {0.001, 0.01, 0.1};
    vector<int> m_var = {-2, -1, 0, 1, 2};
    int n = 2000000;
    int half = (floor)(n / 2);
    writeHeader(csv, {"FilterType", "m", "fpr", "targetFpr","iteration"});
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            for (int a = 0; a < 15; a++) {
                BloomFilter B(half,targetFpr[i],false);
                B.m += m_var[j];
                vector<uint64_t> keys = util::generateUniqueKeys(n);
                vector<uint64_t> keysAdded(keys.begin(), keys.begin() + half);
                vector<uint64_t> keysNotAdded(keys.begin() + half, keys.end());
                int count = 0;
                for (uint64_t k : keysAdded) B.Add(k);
                for (uint64_t k : keysNotAdded) {
                    if (B.Member(k)) count++;
                }
                double actualFpr = (double)count/half;
                write(csv, "Bloom", B.m, actualFpr, targetFpr[i], a, true);
            }
        }
    }
}

void bloom_m_constTime() {
    CSVWriter csv("bloom_m_constTime.csv");
    vector<double> targetFpr = {0.001, 0.01, 0.1};
    vector<int> m_var = {-2, -1, 0, 1, 2};
    int n = 2000000;
    int half = (floor)(n / 2);
    using Clock = high_resolution_clock;
    long long start;
    long long end;
    double interval;

    writeHeader(csv, {"FilterType", "m", "constTime", "targetFpr","iteration"});
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            for (int a = 0; a < 15; a++) {
                BloomFilter B(half,targetFpr[i],false);
                B.m += m_var[j];
                vector<uint64_t> keys = util::generateUniqueKeys(half);
                start = Clock::now().time_since_epoch().count();
                B.AddAll(keys);
                end = Clock::now().time_since_epoch().count();
                interval = static_cast<double> (((double)end - (double)start) / half);

                write(csv, "Bloom", B.m, interval, targetFpr[i], a, true);
            }
        }
    }
}

void xor_constTime() {
    CSVWriter csv("xor_constTime.csv");
    double fpr = 0.01;
    int n = 1000000;
    using Clock = high_resolution_clock;
    long long start;
    long long end;
    double interval;

    writeHeader(csv, {"FilterType", "fpr", "constTime", "targetFpr","iteration"});
    for (int a = 0; a < 15; a++) {
        vector<uint64_t> keys = util::generateUniqueKeys(n);

        XorFilter_test XL(n, fpr, 1.23, false, true);
        start = Clock::now().time_since_epoch().count();
        XL.AddAll(keys);
        end = Clock::now().time_since_epoch().count();
        interval = static_cast<double> (((double) end - (double) start) / n);
        write(csv, "Local", fpr, interval, 0, a, true);


        XorFilter_test XG(n,fpr,1.23,false,false);
        start = Clock::now().time_since_epoch().count();
        XG.AddAll(keys);
        end = Clock::now().time_since_epoch().count();
        interval = static_cast<double> (((double)end - (double)start) / n);
        write(csv, "Global", fpr, interval, 0, a, true);
    }
}

void xor_queryTime() {
    CSVWriter csv("xor_queryTime.csv");
    double fpr = 0.01;
    int n = 2000000;
    int half = (floor)(n / 2);
    using Clock = high_resolution_clock;
    long long start;
    long long end;
    double interval;

    writeHeader(csv, {"FilterType", "fpr", "constTime", "targetFpr","iteration"});
    for (int a = 0; a < 15; a++) {
        vector<uint64_t> keys = util::generateUniqueKeys(n);
        vector<uint64_t> keysAdded(keys.begin(), keys.begin() + half);

//        XorFilter_test XL(half, fpr, 1.23, false, true);
//        XL.AddAll(keysAdded);
//        start = Clock::now().time_since_epoch().count();
//        for (int i = 500000; i < 1500000; i++) XL.Member(keys[i]);
//        end = Clock::now().time_since_epoch().count();
//        interval = static_cast<double> (((double) end - (double) start) / half);
//        write(csv, "Local", fpr, interval, 0, a, true);


        XorFilter_test XG(half,fpr,1.23,false,false);
        XG.AddAll(keysAdded);
        start = Clock::now().time_since_epoch().count();
        for (int i = 500000; i < 1500000; i++) XG.Member(keys[i]);
        end = Clock::now().time_since_epoch().count();
        interval = static_cast<double> (((double)end - (double)start) / half);
        write(csv, "Global", fpr, interval, 0, a, true);
    }
}

void bloom_m_queryTime() {
    CSVWriter csv("bloom_m_queryTime.csv");
    vector<double> targetFpr = {0.001, 0.01, 0.1};
    vector<int> m_var = {-2, -1, 0, 1, 2};
    int n = 2000000;
    int half = (floor)(n / 2);
    using Clock = high_resolution_clock;
    long long start;
    long long end;
    double interval;

    writeHeader(csv, {"FilterType", "m", "constTime", "targetFpr","iteration"});
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 5; j++) {
            for (int a = 0; a < 15; a++) {
                BloomFilter B(half,targetFpr[i],false);
                B.m += m_var[j];
                vector<uint64_t> keys = util::generateUniqueKeys(n);

                for (int i = 0; i < 1000000; i++) B.Add(keys[i]);
                start = Clock::now().time_since_epoch().count();
                for (int i = 500000; i < 1500000; i++) B.Member(keys[i]);
                end = Clock::now().time_since_epoch().count();
                interval = static_cast<double> (((double)end - (double)start) / half);

                write(csv, "Bloom", B.m, interval, targetFpr[i], a, true);
            }
        }
    }
}

void bloom_n_fpr() {
    CSVWriter csv("bloom_n_fpr.csv");
    vector<int> n = {10000, 100000, 1000000, 10000000, 100000000};
    vector<double> targetFpr = {0.001, 0.01, 0.1};
    writeHeader(csv, {"FilterType", "n", "fpr", "targetFpr","iteration"});
//    vector<uint64_t> keys = util::generateUniqueKeys(200000000);
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 10; k++) {
                BloomFilter B(n[i], targetFpr[j], false);
                int count = 0;
                for (int a = 0; a < n[i]; a++) B.Add(a);
                for (int a = n[i]; a < 2*n[i]; a++) {
                    if (B.Member(a)) count++;
                }
                double actualFpr = (double)count/n[i];
                write(csv, "Bloom", n[i], actualFpr, targetFpr[j], k, true);
            }
        }
    }
}

void xor_hash_fpr() {
    CSVWriter csv("xor_fpr.csv");
    int n = 1000000;
    double fpr = 0.01;
    writeHeader(csv, {"FilterType", "n", "fpr", "targetFpr","iteration"});
    vector<uint64_t> keys = util::generateUniqueKeys(2000000);
    vector<uint64_t> added(keys.begin(), keys.begin() + n);
    vector<uint64_t> notAdded(keys.begin() + n, keys.end());
    for (int i = 0; i < 15; i++ ){
        XorFilter_test XL(n, fpr, 1.23, false, true);
        int count = 0;
        XL.Add(added);
        for (auto &k : notAdded) {
            if (XL.Member(k)) count++;
        }
        double actualFpr = (double)count/n;
        write(csv, "Local", n, actualFpr, fpr, i, true);

        XorFilter_test XG(n, fpr, 1.23, false, false);
        count = 0;
        XG.Add(added);
        for (auto &k : notAdded) {
            if (XG.Member(k)) count++;
        }
        actualFpr = (double)count/n;
        write(csv, "Global", n, actualFpr, fpr, i, true);
    }
}

void cuckoo_b_fpr() {
    CSVWriter csv("cuckoo_b_fpr.csv");
    vector<int> b = {1, 2, 4, 8};
    vector<int> f_var = {-2, -1, 0, 1, 2};
    vector<int> n = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
//    int n = 1000000;
    double fpr = 0.01;
    writeHeader(csv, {"FilterType", "n", "fpr", "targetFpr", "b"});
    for (int q = 0; q < n.size(); q++) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 10; j++) {
                CuckooFilter_test C(n[q], fpr, false, b[i]);
                int count = 0;
                for (int a = 0; a < n[q]; a++) C.Add(a);
                for (int a = n[q]; a < 2*n[q]; a++) {
                    if (C.Member(a)) count++;
                }
                double actualFpr = (double)count/n[q];
                write(csv, "Bloom", n[q], actualFpr, fpr, b[i], true);
            }
        }
    }
}

void cuckoo_b_queryTime() {
    CSVWriter csv("cuckoo_b_queryTime.csv");
    vector<int> b = {1, 2, 4, 8};
    vector<int> n = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
    double fpr = 0.01;
    using Clock = high_resolution_clock;
    long long start;
    long long end;
    double interval;

    writeHeader(csv, {"FilterType", "b", "queryTime", "targetFpr", "n"});
    for (int i = 0; i < b.size(); i++) {
        for (int p = 0; p < n.size(); p++) {
            for (int a = 0; a < 15; a++) {
                CuckooFilter_test C(n[p], fpr, false, b[i]);

                for (int k = 0; k < n[p]; k++) C.Add(k);
                start = Clock::now().time_since_epoch().count();
                for (int k = n[p]/2; k < 3*n[p]/2; k++) C.Member(k);
                end = Clock::now().time_since_epoch().count();
                interval = static_cast<double> (((double)end - (double)start) / n[p]);

                write(csv, "Cuckoo", b[i], interval, 0.01, n[p], false);
            }
        }
    }
}

void cuckoo_b_alpha() {
    CSVWriter csv("cuckoo_b_alpha_n.csv");
    vector<int> b = {1, 2, 4, 8};
    vector<int> n = {10000, 50000, 100000, 500000, 1000000, 5000000, 10000000};
//    int n = 1000000;
    double fpr = 0.01;
    writeHeader(csv, {"FilterType", "b", "alpha", "n", "iteration"});
    for (int q = 0; q < n.size(); q++) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 10; j++) {
                CuckooFilter_test C(n[q], fpr, false, b[i]);

                double alpha = 0;
                bool ended = false;
                vector<uint64_t> keys = util::generateUniqueKeys(n[q]);
                for (int k = 0; k < n[q]; k++) {
                    if (!C.Add(keys[k]) & !ended) {
                        alpha = C.load_factor;
                        write(csv, "Cuckoo", b[i], alpha, n[q], j, true);
                        ended = true;
                    }
                }
                if (!ended) write(csv, "Cuckoo", b[i], 1, n[q], j, true);
                else continue;
            }
        }
    }
}

void cuckoo_b_failure() {
    CSVWriter csv("cuckoo_b_failure.csv");
    vector<int> b = {1, 2, 4};
    int n = 1000000;
    int count = 0;
    double fpr = 0.5;
    writeHeader(csv, {"FilterType", "b", "failure", "n", "calculated"});
    for (int q = 0; q < 3; q++) {
        for (int a = 0; a < 10; a++) {
            CuckooFilter_test C(n, fpr, false, b[q]);
            for (int i =0; i < n; i++) C.Add(i);
            count = 0;
            for (int j = n; j < n+10000000; j++) {
                if (!C.Add(j)) count++;
            }
            double failure = (double)count / 10000000;
            write(csv, "Cuckoo", b[q], failure, n, 0, true);
        }
    }
}

void bB_beta_fpr() {
    CSVWriter csv("bB_beta_fpr.csv");
    double fpr = 0.01;
    vector<double> betas = {1.14,1.15,1.16,1.17,1.18,1.19,1.20,1.21,1.22,1.23,1.24,1.25,1.26,1.27,1.28,1.29,1.30};
    int n = 2000000;
    int half = (floor)(n / 2);
    writeHeader(csv, {"FilterType", "beta", "fpr", "targetFpr","iteration"});
    for (int i = 0; i < betas.size(); i++) {
        for (int a = 0; a < 15; a++) {
            BlockedBloom_test B(half, fpr, false, betas[i]);
            vector<uint64_t> keys = util::generateUniqueKeys(n);
            vector<uint64_t> keysAdded(keys.begin(), keys.begin() + half);
            vector<uint64_t> keysNotAdded(keys.begin() + half, keys.end());
            int count = 0;
            for (uint64_t k : keysAdded) B.Add(k);
            for (uint64_t k : keysNotAdded) {
                if (B.Member(k)) count++;
            }
            double actualFpr = (double)count/half;
            write(csv, "Bloom", betas[i], actualFpr, fpr, a, true);
        }
    }
}

void cuckoo_f_alpha() {
    CSVWriter csv("cuckoo_f_alpha.csv");
    int n = 1000000;
    vector<int> f_var = {-2,-1,0,1,2};
    vector<double> fpr = {0.001, 0.01, 0.1};
    writeHeader(csv, {"FilterType", "f", "alpha", "targetFpr", "n"});
    for (int q = 0; q < fpr.size(); q++) {
        for (int i = 0; i < f_var.size(); i++) {
            for (int j = 0; j < 10; j++) {
                int f = ceil(log2(1/fpr[q]) + 3);
                f += f_var[i];
                CuckooFilter_test_fingerprint C(n, fpr[q], false, f);
                double alpha = 0;
                bool ended = false;
                vector<uint64_t> keys = util::generateUniqueKeys(n);
                for (int k = 0; k < n; k++) {
                    if (!C.Add(keys[k]) & !ended) {
                        alpha = C.load_factor;
                        write(csv, "Cuckoo", f, alpha, fpr[q], true, n);
                        ended = true;
                    }
                }
                if (!ended) write(csv, "Cuckoo", f, alpha, fpr[q], true, n);
                else continue;
            }
        }
    }
}

void cuckoo_f_fpr() {
    CSVWriter csv("cuckoo_f_fpr.csv");
    int n = 1000000;
    vector<int> f_var = {-2,-1,0,1,2};
    vector<double> fpr = {0.001, 0.01, 0.1};
    writeHeader(csv, {"FilterType", "f", "fpr", "targetFpr", "n"});
    for (int i = 0; i < f_var.size(); i++) {
        for (int k = 0; k < fpr.size(); k++) {
            for (int j = 0; j < 10; j++) {
                int f = ceil(log2(1/fpr[k]) + 3);
                f += f_var[i];
                CuckooFilter_test_fingerprint C(n, fpr[k], false, f);
                int count = 0;
                for (int a = 0; a < 1000000; a++) C.Add(a);
                for (int a = 1000000; a < 2000000; a++) {
                    if (C.Member(a)) count++;
                }
                double actualFpr = (double)count/n;
                write(csv, "Cuckoo", f, actualFpr, fpr[k], n, true);
            }
        }
    }
}

void xor_hash_attempt() {
    CSVWriter csv("xor_hash_attempt.csv");
    int n = 1000000;
    double fpr = 0.01;
    int failedL = 0;
    int failedG = 0;
    writeHeader(csv, {"FilterType", "n", "fails", "percent", "n"});
    vector<uint64_t> keys = util::generateUniqueKeys(n);
    for (int j = 0; j < 10; j++) {
        failedL = 0;
        failedG = 0;
        for (int i = 0; i < 100; i++) {
            XorFilter_test XL(n,fpr,1.222, false, false);
            if (!XL.Add(keys)) failedL++;

            XorFilter_test X(n,fpr,1.222, false, true);
            if (!X.Add(keys)) failedG++;
        }
        write(csv, "Local", 1000000, failedL, failedL/n, j, true);
        write(csv, "Global", 1000000, failedG, failedG/n, j, true);
    }
}

void benchmark() {
//    fpr_bpi();
//    n_buildTime();
//    lf_posQueryTime();
    lf_halfQueryTime();
//    lf_negQueryTime();
//    target_actual_fpr();
//    target_actual_fpr_fp();
}

void empirical() {
//    bloom_m_fpr();
//    bloom_m_constTime();
//    bB_beta_fpr();
//    bloom_m_queryTime();
//    xor_hash_attempt();
//    xor_queryTime();
    xor_hash_fpr();
//    xor_constTime();
//    bB_everything();
//    cuckoo_b_alpha();
//    cuckoo_b_fpr();
//    cuckoo_b_queryTime();
//    cuckoo_f_fpr();
//    cuckoo_f_alpha();
//    cuckoo_b_failure();
// The one below takes lone time
//    bloom_n_fpr();
}


int main() {
    empirical();
//    benchmark();
    return 0;
}