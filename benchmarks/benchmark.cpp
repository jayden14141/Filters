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
#include "../src/cuckoo.h"
#include "../src/cuckoo_test.h"
#include "../src/hash_function.h"
#include "../src/xor.h"
#include "../src/xor_fixed.h"
#include "../src/CSVWriter.h"

using namespace std;
using namespace hash_function;
using namespace bloomFilter;
using namespace blockedBloomFilter;
using namespace cuckooFilter;
using namespace xorFilter;
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
        BloomFilter B(n,fpr,false);
        write(csv, "Bloom", n, fpr, B.Bpi(), -1, false);
        CuckooFilter C(n, fpr, false);
        write(csv, "Cuckoo", n, fpr, C.Bpi(), -1, false);
        XorFilter X(n, fpr, false);
        write(csv, "Xor", n, fpr, X.Bpi(), -1, false);
        BlockedBloom BB(n,fpr,false);
        write(csv, "Blocked Bloom", n, fpr, BB.Bpi(), -1, false);


        write(csv, "Lower Bound", n, fpr, log2(1/fpr), -1, false);
        fpr *= 10;
    }
}

void lf_posQueryTime() {
    CSVWriter csv("lf_posQueryTime.csv");
    int n = 1000000;
    int k = 100;
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
            BloomFilter B(n,fpr, false);
            CuckooFilter C(n, fpr, false);
            BlockedBloom BB(n, fpr, false);
            XorFilter X(n, fpr, false);

            B.AddAll(keyLf[i]);
            C.AddAll(keyLf[i]);
            BB.AddAll(keyLf[i]);
            X.AddAll(keyLf[i]);

            start = Clock::now().time_since_epoch().count();
            for (int t = 0; t < k; t++) B.Member(keys[t]);
            end = Clock::now().time_since_epoch().count();
            interval = static_cast<double> (((double)end - (double)start) / k);
            write(csv, "Bloom", loadFactor[i], interval, n, j, true);


            start = Clock::now().time_since_epoch().count();
            for (int t = 0; t < k; t++)  C.Member(keys[t]);
            end = Clock::now().time_since_epoch().count();
            interval = static_cast<double> (((double)end - (double)start) / k);
            write(csv, "Cuckoo", loadFactor[i], interval, n, j, true);

            start = Clock::now().time_since_epoch().count();
            for (int t = 0; t < k; t++)  BB.Member(keys[t]);
            end = Clock::now().time_since_epoch().count();
            interval = static_cast<double> (((double)end - (double)start) / k);
            write(csv, "Blocked Bloom", loadFactor[i], interval, n, j, true);

            start = Clock::now().time_since_epoch().count();
            for (int t = 0; t < k; t++)  X.Member(keys[t]);
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
    int k = 100;
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
            CuckooFilter C(n, fpr, false);
            BlockedBloom BB(n, fpr, false);
            XorFilter X(n, fpr, false);

            B.AddAll(keyLf[i]);
            C.AddAll(keyLf[i]);
            BB.AddAll(keyLf[i]);
            X.AddAll(keyLf[i]);

            start = Clock::now().time_since_epoch().count();
            for (int t = 0; t < k; t++) B.Member(keysNeg[t]);
            end = Clock::now().time_since_epoch().count();
            interval = static_cast<double> (((double)end - (double)start) / k);
            write(csv, "Bloom", loadFactor[i], interval, n, j, true);


            start = Clock::now().time_since_epoch().count();
            for (int t = 0; t < k; t++)  C.Member(keysNeg[t]);
            end = Clock::now().time_since_epoch().count();
            interval = static_cast<double> (((double)end - (double)start) / k);
            write(csv, "Cuckoo", loadFactor[i], interval, n, j, true);

            start = Clock::now().time_since_epoch().count();
            for (int t = 0; t < k; t++)  BB.Member(keysNeg[t]);
            end = Clock::now().time_since_epoch().count();
            interval = static_cast<double> (((double)end - (double)start) / k);
            write(csv, "Blocked Bloom", loadFactor[i], interval, n, j, true);

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
    int n = 10000;
    double fpr = 0.01;

    using Clock = high_resolution_clock;
    long long start;
    long long end;
    double interval;

    std::vector<uint64_t> keys = util::generateUniqueKeys(n);
    writeHeader(csv, {"FilterType", "n", "constTime", "fpr","iteration"});
    for (int i = 0; i < 5; i++) {
        std::vector<uint64_t> keys = util::generateUniqueKeys(n);
        for (int j = 1; j <= 10; j++) {
            BloomFilter B(n,fpr, false);
            CuckooFilter C(n, fpr, false);
            BlockedBloom BB(n, fpr, false);
            XorFilter X(n, fpr, false);
//            XorFilter_fixed XF(n, fpr, false);

            start = Clock::now().time_since_epoch().count();
            B.AddAll(keys);
            end = Clock::now().time_since_epoch().count();
            interval = static_cast<double> (((double)end - (double)start) / n);
            write(csv, "Bloom", n, interval, fpr, j, true);


            start = Clock::now().time_since_epoch().count();
            C.AddAll(keys);
            end = Clock::now().time_since_epoch().count();
            interval = static_cast<double> (((double)end - (double)start) / n);
            write(csv, "Cuckoo", n, interval, fpr, j, true);

            start = Clock::now().time_since_epoch().count();
            BB.AddAll(keys);
            end = Clock::now().time_since_epoch().count();
            interval = static_cast<double> (((double)end - (double)start) / n);
            write(csv, "Blocked Bloom", n, interval, fpr, j, true);

            start = Clock::now().time_since_epoch().count();
            X.AddAll(keys);
            end = Clock::now().time_since_epoch().count();
            interval = static_cast<double> (((double)end - (double)start) / n);
            write(csv, "Xor", n, interval, fpr, j, true);

//            start = Clock::now().time_since_epoch().count();
//            XF.AddAll(keys);
//            end = Clock::now().time_since_epoch().count();
//            interval = static_cast<double> (((double)end - (double)start) / n);
//            write(csv, "Xor fixed", n, interval, fpr, j, true);
        }

        n *= 10;
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

void c() {
    int n = 10000000;
    double fpr = 0.01;
    using Clock = high_resolution_clock;
    long long start;
    long long end;
    double interval;

    for (int i = 0; i < 2; i ++) {
        BloomFilter B(n, fpr, false);
        BlockedBloom BB(n, fpr, false);
        std::vector<uint64_t> keys = util::generateUniqueKeys(n);

        start = Clock::now().time_since_epoch().count();
        B.AddAll(keys);
        end = Clock::now().time_since_epoch().count();
        interval = static_cast<double> (((double)end - (double)start) / n);
        std::cout << "Bloom: " << interval << std::endl;

        start = Clock::now().time_since_epoch().count();
        BB.AddAll(keys);
        end = Clock::now().time_since_epoch().count();
        interval = static_cast<double> (((double)end - (double)start) / n);
        std::cout << "Blocked Bloom: " << interval << std::endl;
    }
}

void b() {
    int n = 1000000;
    double fpr = 0.01;
    BloomFilter B(n, fpr, false);
//    std::vector<uint64_t> keys = util::generateUniqueKeys(n);
//    BB.AddAll(keys);
    std::cout << B.Offset() << std::endl;
}

void a() {
//    int n = 1000000;
//    XorFilter x(n, 0.01, true);
//    x.Info();
    int n = 1000000;
    double fpr = 0.01;

    using Clock = high_resolution_clock;
    long long start;
    long long end;
    double interval;
    std::vector<uint64_t> keys = util::generateUniqueKeys(n);
    CuckooFilter C(n, fpr, false);
    CuckooFilter_test CT(n, fpr, false, 4);
//
    start = Clock::now().time_since_epoch().count();
    C.AddAll(keys);
    end = Clock::now().time_since_epoch().count();
    interval = static_cast<double> (((double)end - (double)start) / n);
    std::cout << interval << std::endl;

    start = Clock::now().time_since_epoch().count();
    CT.AddAll(keys);
    end = Clock::now().time_since_epoch().count();
    interval = static_cast<double> (((double)end - (double)start) / n);
    std::cout << interval << std::endl;
}

void d() {
    int n = 20000;
    int size = ceil(n/2);
    for (int k = 0; k < 4; k++) {
        double fpr = 0.0001;
        for (int j = 0; j < 4; j++) {
            for (int i = 0; i < 1; i++) {
                BloomFilter B(int(n/2),fpr,false);
                vector<uint64_t> keys = util::generateUniqueKeys(n);
                vector<uint64_t> inserted(keys.begin(), keys.begin() + ceil(n / 2));
                vector<uint64_t> notInserted(keys.begin() + ceil(n / 2), keys.end());
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
                std::cout << "N: " << n << ", fpr: " << fpr <<", actual: " << actual << std::endl;
            }
            fpr *= 10;
        }
        n *= 10;
    }
}

void e() {
    int n = 1000000;
    double fpr = 0.01;
    BloomFilter B(n / 2 ,fpr,false);
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
    std::cout << "N: " << n << ", fpr: " << fpr <<", actual: " << actual << std::endl;
    B.Info();
}

void xor_attempt() {
    int n = 1000000;
    double fpr = 0.01;
    int failed = 0;
    for (int i = 0; i < 10; i++) {
        XorFilter X(n, fpr, false);
        vector<uint64_t> keys = util::generateUniqueKeys(n);
        X.Add(keys);
        failed += X.attempt;
    }
    std::cout << failed << std::endl;
}

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

void cuckoo_b_alpha() {
    CSVWriter csv("cuckoo_b_alpha_n.csv");
    vector<int> b = {1, 2, 4, 8};
    vector<int> n = {10000, 100000, 1000000, 10000000};
//    int n = 1000000;
    double fpr = 0.01;
    writeHeader(csv, {"FilterType", "b", "alpha", "n", "iteration"});
    for (int q = 0; q < 4; q++) {
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 15; j++) {
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
void benchmark() {
//    fpr_bpi();
//    n_buildTime();
//    lf_posQueryTime();
//    lf_negQueryTime();
//    target_actual_fpr();
//    a();
    b();
//    c();
//    d();
//    e();
}

void empirical() {
//    bloom_m_fpr();
    bloom_m_constTime();
//    bloom_m_queryTime();
//    xor_attempt();
//    bB_everything();
//    cuckoo_b_alpha();
//    cuckoo_b_failure();
// The one below takes lone time
//    bloom_n_fpr();
}


int main() {
    empirical();
//    benchmark();
    return 0;
}