//
// Created by Jayden on 20/02/2024.
//

#include "../src/bloom.h"
#include "../src/hash_function.h"
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace hash_function;
using namespace bloomFilter;

void test_hash() {
    int a = 11;
    SimpleMixHashing hasher;
    cout << "TESTS" << endl;
    cout << hasher(a) << endl;
}

void test_bloom() {
    BloomFilter b(1,10,0.01);
    std::cout << "Size:" << b.Size() << std::endl;
    std::cout << "Adding item 1" << std::endl;
    b.Add(1);

    std::cout << "Querying item 1:" << std::endl;
    std::cout << std::boolalpha << b.Member(1) << std::endl;

    std::cout << "Querying item 2:" << std::endl;
    std::cout << std::boolalpha << b.Member(2) << std::endl;

    std::cout << "Adding item 2" << std::endl;
    b.Add(2);

    std::cout << "Querying item 2:" << std::endl;
    std::cout << std::boolalpha << b.Member(2) << std::endl;

}

int main() {
//    test_hash();
    test_bloom();
    return 0;
}