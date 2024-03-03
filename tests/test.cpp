//
// Created by Jayden on 20/02/2024.
//

#include "../src/bloom.h"
#include "../src/cuckoo.h"
#include "../src/table.h"
#include "../src/hash_function.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <random>

#include "util.h"

using namespace std;
using namespace hash_function;
using namespace bloomFilter;
using namespace cuckooFilter;

void test_fpr() {
    int n = 20;
    for (int j = 0; j < 8; j++) {
        BloomFilter b(n,0.01, false);
        vector<uint64_t> keys = util::generateUniqueKeys(n);
        int count = 0;
        // Add half of the random generated keys
        for (int i = 0; i < n / 2; i++) {
            b.Add(keys[i]);
        }

        // Test false positive rate by querying the other half that hasn't been added
        for (int i = n / 2; i < n; i++) {
            if (b.Member(keys[i])) count++;
        }

        double fpr_actual =  (double)count / ((double)n / 2);
        cout << "---------------------------------------" << endl;
        cout << "Added keys : " << n/2 << endl;
        cout << "Queried keys that has not been added : " << n/2 << endl;
        cout << "Target false positive rate : " << b.Fpr() << endl;
        cout << "Actual false positive rate : " << fpr_actual << endl;
        n *= 10;
    }
}

void test_fnr() {
    cout << "False Negative test" << endl;
    cout << "---------------------------------------" << endl;
    int n = 100000000;
    cout << "Added keys : " << n << endl;
    BloomFilter b(n,0.01, false);
    vector<uint64_t> keys = util::generateUniqueKeys(n);
    for (int i = 0; i < n; i++) {
        b.Add(keys[i]);
    }
    // Test the filter always outputs "Yes" when querying added keys
    for (int i = 0; i < n; i++) {
        assert(b.Member(keys[i]));
    }
    cout << "False negative test passed" << endl;
}

void test_hash() {
    int a = 11;
    SimpleMixHashing hasher;
    cout << "TESTS" << endl;
    cout << hasher(a) << endl;
}

void test_bloom() {
    BloomFilter b(100000000,0.01, true);
    std::cout << "Size:" << b.Size() << std::endl;
    std::cout << "Adding item 1" << std::endl;
    int one = random();
    int two = random();
    b.Add(one);

    std::cout << "Querying item 1:" << std::endl;
    std::cout << std::boolalpha << b.Member(one) << std::endl;

    std::cout << "Querying item 2:" << std::endl;
    std::cout << std::boolalpha << b.Member(two) << std::endl;

    std::cout << "Adding item 2" << std::endl;
    b.Add(two);

    std::cout << "Querying item 2:" << std::endl;
    std::cout << std::boolalpha << b.Member(two) << std::endl;
}

void test_cuckoo() {
    CuckooFilter c(100000000, 0.01, true);
    std::cout << "Size:" << c.Size() << std::endl;
    std::cout << "Adding item 1" << std::endl;
    int one = random();
    int two = random();
    c.Add(one);

    std::cout << "Querying item 1:" << std::endl;
    std::cout << std::boolalpha << c.Member(one) << std::endl;

    std::cout << "Querying item 2:" << std::endl;
    std::cout << std::boolalpha << c.Member(two) << std::endl;

    std::cout << "Adding item 2" << std::endl;
    c.Add(two);

    std::cout << "Querying item 2:" << std::endl;
    std::cout << std::boolalpha << c.Member(two) << std::endl;
}

void test_correctness() {
//    test_bloom();
    test_cuckoo();
}


int main() {
//    test_hash();
    test_correctness();
//    test_fpr();
//    test_fnr();
    return 0;
}