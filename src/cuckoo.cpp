//
// Created by Jayden on 28/02/2024.
//

#include "cuckoo.h"

cuckooFilter::CuckooFilter::CuckooFilter(int n, int f, double fpr) {
    this->n = n;
    this->f = f;
    this->fpr = fpr;
    this->m = _getM();

    this->data = new Table(b,f,m);
    _insertKeys();
}

cuckooFilter::CuckooFilter::~CuckooFilter() {
    delete[] data;
}

bool cuckooFilter::CuckooFilter::Add(const int &item) {
    uint64_t hashed_one = hasher(item);
    size_t fingerPrint = fingerprint(item);
    // Placeholder for fingerprint (using 4 bits from the least significant bit)
    fingerPrint &= 15;
    uint64_t hashed_two = hasher(fingerPrint);
    hashed_two ^= hashed_one;

    size_t victim;
    uint64_t new_pos = hashed_one;
    if (data->HasEmptyEntry(hashed_one)) {
        data->InsertTag(hashed_one, fingerPrint, false, victim);
        return true;
    } else if (data->HasEmptyEntry(hashed_two)) {
        data->InsertTag(hashed_two, fingerPrint, false, victim);
        return true;
    } else {
        // TODO : Set appropriate values for iteration
        for (int i = 0; i < 5; i++) {
            data->InsertTag(new_pos, fingerPrint, true, victim);
            new_pos ^= hasher(victim);
            if (data->HasEmptyEntry(new_pos)) {
                data-> InsertTag(new_pos, fingerPrint, false, victim);
                return true;
            } else continue;
        }
        return false;
    }
}

bool cuckooFilter::CuckooFilter::Member(const int &item) const {
    uint64_t hashed_one = hasher(item);
    size_t fingerPrint = fingerprint(item);
    // Placeholder for fingerprint (using 4 bits from the least significant bit)
    fingerPrint &= 15;
    uint64_t hashed_two = hasher(fingerPrint);
    hashed_two ^= hashed_one;
    return data->FindTag(hashed_one, hashed_two, fingerPrint);
}

size_t cuckooFilter::CuckooFilter::Size() const {
    return size;
}

//TODO: Implement me!
void cuckooFilter::CuckooFilter::Info() const {

}

//TODO: Implement me!
size_t cuckooFilter::CuckooFilter::_getM() const {
    return 0;
}

//TODO: Implement me!
size_t cuckooFilter::CuckooFilter::_getSize() const {
    return 0;
}

//TODO: Implement me!
void cuckooFilter::CuckooFilter::_insertKeys() {

}
