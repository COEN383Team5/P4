#include <iostream>
#include "FIFOPageReplacer.h"

std::pair<bool, MemoryReference> FIFOPageReplacer::algImpl(const int &pageNum, const int &id, const double &timeStamp) {

    int page = 0;
    for (size_t i = 1; i < NUM_PAGE_TABLE_ENTRIES; ++i) {
        if (table[i].firstRefTime < table[page].firstRefTime) {
            page = i;
        }
    }
    
    MemoryReference m(timeStamp, id, pageNum, table[page].ownerPage, table[page].ownerId, page);
    table[page].numRefs = 0;
    setPage(&table[page], pageNum, id);
    return std::pair<bool, MemoryReference>(false, m);
}

FIFOPageReplacer::FIFOPageReplacer() : PageTable() {}

FIFOPageReplacer::FIFOPageReplacer(const FIFOPageReplacer &other) {
    if(this == &other) {
        return;
    }
    *this = other;
}

FIFOPageReplacer &FIFOPageReplacer::operator=(const FIFOPageReplacer &other) {
    PageTable::operator=(other);
    return *this;
}

FIFOPageReplacer::~FIFOPageReplacer() {}
