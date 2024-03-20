//
// Created by Jayden on 28/02/2024.
//

#include "cuckoo.h"
#include "util.h"

cuckooFilter::CuckooFilter::CuckooFilter(int n, double fpr, bool construct) {
    this->n = n;

    this->fpr = fpr;
    this->f = _getF();
    this->m = _getM();
    this->size = _getSize();
    fingerPrinter.set(f);

    this->data = new Table(b,f,(int)m);
    this->bits_per_item = (double)f/(double)load_factor;
    if (construct) _insertKeys();
}

cuckooFilter::CuckooFilter::~CuckooFilter() {
    delete data;
}

bool cuckooFilter::CuckooFilter::Add(const int &item) {
    uint64_t hashed_one = hasher(item);
    size_t fingerPrint = fingerPrinter(item);
    uint64_t hashed_two = hasher(fingerPrint);
    hashed_two ^= hashed_one;

    size_t victim;
    uint64_t new_pos = hashed_one;
    if (data->HasEmptyEntry(util::fastRangeSize(hashed_one, m))) {
        data->InsertTag(util::fastRangeSize(hashed_one, m), fingerPrint, false, victim);
        return true;
    } else if (data->HasEmptyEntry(util::fastRangeSize(hashed_two, m))) {
        data->InsertTag(util::fastRangeSize(hashed_two, m), fingerPrint, false, victim);
        return true;
    } else {
        // TODO : Set appropriate values for iteration
        for (int i = 0; i < 5; i++) {
            data->InsertTag(util::fastRangeSize(new_pos, m), fingerPrint, true, victim);
            new_pos ^= hasher(victim);
            if (data->HasEmptyEntry(util::fastRangeSize(new_pos, m))) {
                data-> InsertTag(util::fastRangeSize(new_pos, m), fingerPrint, false, victim);
                eviction++;
                return true;
            } else continue;
        }
        return false;
    }
}

bool cuckooFilter::CuckooFilter::Member(const int &item) const {
    uint64_t hashed_one = hasher(item);
    size_t fingerPrint = fingerPrinter(item);
    uint64_t hashed_two = hasher(fingerPrint);
    hashed_two ^= hashed_one;
    return data->FindTag(util::fastRangeSize(hashed_one, m), util::fastRangeSize(hashed_two, m), fingerPrint);
}

size_t cuckooFilter::CuckooFilter::Size() const {
    return size;
}

double cuckooFilter::CuckooFilter::Fpr() const {
    return fpr;
}

void cuckooFilter::CuckooFilter::Info() const {
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

size_t cuckooFilter::CuckooFilter::_getM() const {
    return static_cast<size_t> (ceil(n/load_factor));
}

int cuckooFilter::CuckooFilter::_getF() const {
    return ceil(log2(1/fpr) + log2(2*b));
}

size_t cuckooFilter::CuckooFilter::_getSize() const {
    return (int)f*b*m;
}

void cuckooFilter::CuckooFilter::_insertKeys() {
    //TODO: Generate random strings
    std::vector<uint64_t> random = util::generateUniqueKeys(n);
    for (int i = 0; i < n; i++) {
        Add((int)random[i]);
    }
}

double cuckooFilter::CuckooFilter::Bpi() const {
    return bits_per_item;
}
