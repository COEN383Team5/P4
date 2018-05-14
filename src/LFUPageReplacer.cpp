#include <iostream>
#include "LFUPageReplacer.h"

std::pair<bool, MemoryReference> LFUPageReplacer::algImpl(const int &pageNum, const int &id, const double &timeStamp) {
    int page = 0;
    
    for (size_t i = 1; i < NUM_PAGE_TABLE_ENTRIES; ++i) {
        if (table[i].numRefs < table[page].numRefs) {
            page = i;
        }
    }

    MemoryReference m(timeStamp, id, pageNum, table[page].ownerPage, table[page].ownerId, page);
    table[page].numRefs = 0;
    setPage(&table[page], pageNum, id);
    return std::pair<bool, MemoryReference>(false, m);
}

LFUPageReplacer::LFUPageReplacer() : PageTable() {}

LFUPageReplacer::LFUPageReplacer(const LFUPageReplacer &other) {
    if(this == &other) {
        return;
    }
    *this = other;
}

LFUPageReplacer &LFUPageReplacer::operator=(const LFUPageReplacer &other) {
    PageTable::operator=(other);
    return *this;
}

LFUPageReplacer::~LFUPageReplacer() {}