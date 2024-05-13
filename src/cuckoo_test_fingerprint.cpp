//
// Created by Jayden on 10/05/2024.
//

#include "cuckoo_test_fingerprint.h"
#include "cuckoo_test.h"
#include "util.h"

cuckooFilter::CuckooFilter_test_fingerprint::CuckooFilter_test_fingerprint(int n, double fpr, bool construct, int f) {
    this->n = n;

    this->fpr = fpr;
    this->b = 4;
    this->f = f;
    this->m = _getM();
    this->size = _getSize();
    fingerPrinter.set(f);

    this->data = new Table(b, f, (int)m);
    this->bits_per_item = (double)f / load_factor;
    if (construct) _insertKeys();
}

cuckooFilter::CuckooFilter_test_fingerprint::~CuckooFilter_test_fingerprint() {
    delete data;
}

bool cuckooFilter::CuckooFilter_test_fingerprint::Add(const uint64_t &item) {
    uint64_t hashed_one = hasher(item);
    size_t fingerPrint = fingerPrinter(item);
    uint64_t hashed_two = hasher(fingerPrint);
    hashed_two ^= hashed_one;

    size_t victim;
    uint64_t new_pos = hashed_one;
    if (data->HasEmptyEntry(util::fastRangeSize(hashed_one, m))) {
        if (count == n - 1) return true;
        data->InsertTag(util::fastRangeSize(hashed_one, m), fingerPrint, false, victim);
        count++;
        return true;
    } else if (data->HasEmptyEntry(util::fastRangeSize(hashed_two, m))) {
        if (count == n - 1) return true;
        data->InsertTag(util::fastRangeSize(hashed_two, m), fingerPrint, false, victim);
        count++;
        return true;
    } else {
        // TODO : Set appropriate values for iteration
        for (int i = 0; i < 5; i++) {
            data->InsertTag(util::fastRangeSize(new_pos, m), fingerPrint, true, victim);
            new_pos ^= hasher(victim);
            if (data->HasEmptyEntry(util::fastRangeSize(new_pos, m))) {
                if (count == n - 1) return true;
                data-> InsertTag(util::fastRangeSize(new_pos, m), fingerPrint, false, victim);
                count++;
                return true;
            } else continue;
        }
        this->load_factor = (double)count/ n;
        return false;
    }
}

bool cuckooFilter::CuckooFilter_test_fingerprint::AddAll(std::vector<uint64_t> &keys) {
    for (int i = 0; i < keys.size(); i++) {
        Add(keys[i]);
    }
    return true;
}

bool cuckooFilter::CuckooFilter_test_fingerprint::Member(const uint64_t &item) const {
    uint64_t hashed_one = hasher(item);
    size_t fingerPrint = fingerPrinter(item);
    uint64_t hashed_two = hasher(fingerPrint);
    hashed_two ^= hashed_one;
    return data->FindTag(util::fastRangeSize(hashed_one, m), util::fastRangeSize(hashed_two, m), fingerPrint);
}

size_t cuckooFilter::CuckooFilter_test_fingerprint::Size() const {
    return size;
}

double cuckooFilter::CuckooFilter_test_fingerprint::Fpr() const {
    return fpr;
}

void cuckooFilter::CuckooFilter_test_fingerprint::Info() const {
    std::cout << "Cuckoo Filter" << std::endl;
    std::cout << "------------------------------- \n" << std::endl;
    std::cout << "Number of keys inserted  : " << n << std::endl;
    std::cout << "False positive rate  : " << fpr << std::endl;
    std::cout << "Size of a fingerprint function  : " << f << std::endl;
    std::cout << "Shape of a filter  : " << "(" << m << ", " << b << ")" << std::endl;
    std::cout << "Size of a filter in bits  : " << (int)ceil(Size()) << std::endl;
    std::cout << "Bits per Item (Theoretical) : " << log2(1/fpr) << std::endl;
    std::cout << "Bits per Item : " << Bpi() << std::endl;
    std::cout << "Space overhead : " << 100 * (bits_per_item - log2(1/fpr)) / log2(1/fpr) << "% " << std::endl;

}

size_t cuckooFilter::CuckooFilter_test_fingerprint::_getM() const {
    return static_cast<size_t> (ceil(n/(0.94*b)));
}

int cuckooFilter::CuckooFilter_test_fingerprint::_getF() const {
    return ceil(log2(1/fpr) + log2(2*b));
}

size_t cuckooFilter::CuckooFilter_test_fingerprint::_getSize() const {
    return (int)f*b*m;
}

void cuckooFilter::CuckooFilter_test_fingerprint::_insertKeys() {
    //TODO: Generate random strings
    std::vector<uint64_t> random = util::generateUniqueKeys(n);
    for (int i = 0; i < n; i++) {
        Add((int)random[i]);
    }
}

double cuckooFilter::CuckooFilter_test_fingerprint::Bpi() const {
    return bits_per_item;
}