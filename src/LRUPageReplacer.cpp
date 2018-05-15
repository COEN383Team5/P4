#include <iostream>
#include "LRUPageReplacer.h"

std::pair<bool, MemoryReference> LRUPageReplacer::algImpl(const int &pageNum, const int &id) {
    int page = 0;
    
    for (size_t i = 1; i < NUM_PAGE_TABLE_ENTRIES; ++i) {
        if (table[i].refTime < table[page].refTime) {
            page = i;
        }
    }

    MemoryReference m(id, pageNum, table[page].ownerPage, table[page].ownerId, page);
    table[page].numRefs = 0;
    setPage(&table[page], pageNum, id);
    return std::pair<bool, MemoryReference>(false, m);
}

LRUPageReplacer::LRUPageReplacer() : PageTable() {}

LRUPageReplacer::LRUPageReplacer(const LRUPageReplacer &other) {
    if(this == &other) {
        return;
    }
    *this = other;
}

LRUPageReplacer &LRUPageReplacer::operator=(const LRUPageReplacer &other) {
    PageTable::operator=(other);
    return *this;
}

LRUPageReplacer::~LRUPageReplacer() {}