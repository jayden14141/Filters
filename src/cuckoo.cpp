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

void cuckooFilter::CuckooFilter::Add(const int &item) {

}

bool cuckooFilter::CuckooFilter::Member(const int &item) const {
    return false;
}

size_t cuckooFilter::CuckooFilter::Size() const {
    return size;
}

void cuckooFilter::CuckooFilter::Info() const {

}

size_t cuckooFilter::CuckooFilter::_getM() const {
    return 0;
}

size_t cuckooFilter::CuckooFilter::_getSize() const {
    return 0;
}

void cuckooFilter::CuckooFilter::_insertKeys() {

}
