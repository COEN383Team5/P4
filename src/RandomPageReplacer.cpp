#include "RandomPageReplacer.h"

std::pair<bool, MemoryReference> RandomPageReplacer::algImpl(const int &pageNum, const int &id) {
    // avoids replacing page 0
    int page = rand()%(NUM_PAGE_TABLE_ENTRIES-1)+1;
    MemoryReference m(id, pageNum, table[page].ownerPage, table[page].ownerId, page);
    table[page].numRefs = 0;
    setPage(page, pageNum, id);
    return std::pair<bool, MemoryReference>(false, m);
}

RandomPageReplacer::RandomPageReplacer() : PageTable() {}

RandomPageReplacer::RandomPageReplacer(const RandomPageReplacer &other) {
    if(this == &other) {
        return;
    }
    *this = other;
}

RandomPageReplacer &RandomPageReplacer::operator=(const RandomPageReplacer &other) {
    PageTable::operator=(other);
    return *this;
}

RandomPageReplacer::~RandomPageReplacer() {}
