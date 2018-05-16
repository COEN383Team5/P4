#include <iostream>
#include "MFUPageReplacer.h"

std::pair<bool, MemoryReference> MFUPageReplacer::algImpl(const int &pageNum, const int &id) {
    int page = 0;
    
    for (size_t i = 1; i < NUM_PAGE_TABLE_ENTRIES; ++i) {
        if (table[i].numRefs > table[page].numRefs) {
            page = i;
        }
    }

    MemoryReference m(id, pageNum, table[page].ownerPage, table[page].ownerId, page);
    table[page].numRefs = 0;
    setPage(&table[page], pageNum, id);
    return std::pair<bool, MemoryReference>(false, m);
}

MFUPageReplacer::MFUPageReplacer() : PageTable() {}

MFUPageReplacer::MFUPageReplacer(const MFUPageReplacer &other) {
    if(this == &other) {
        return;
    }
    *this = other;
}

MFUPageReplacer &MFUPageReplacer::operator=(const MFUPageReplacer &other) {
    PageTable::operator=(other);
    return *this;
}

MFUPageReplacer::~MFUPageReplacer() {}