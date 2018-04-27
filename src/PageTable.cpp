#include <cstring>
#include "PageTable.h"

int PageTable::isInTable(const int &pageNum, const int &id) const {
    for(int i = 0; i < NUM_PAGE_TABLE_ENTRIES; i++) {
        if(table[i].valid && table[i].ownerId == id && table[i].ownerPage == pageNum) {
            return i;
        }
    }
    return -1;
}

int PageTable::getFreePage() const {
    for(int i = 0; i < NUM_PAGE_TABLE_ENTRIES; i++) {
        if(!table[i].valid) {
            return i;
        }
    }
    return -1;
}

void PageTable::setPage(const int &page, const int &pageNum, const int &id) {
    table[page].referenced = true;
    table[page].numRefs++;
    table[page].refTime = curTime;
    table[page].ownerId = id;
    table[page].ownerPage = pageNum;
    table[page].valid = true;
}

PageTable::PageTable() {
    table = new PageTableEntry[NUM_PAGE_TABLE_ENTRIES];
    numFree = NUM_PAGE_TABLE_ENTRIES;
}

PageTable::PageTable(const PageTable &other) {
    if(this == &other) 
        return;
    *this = other;
}

PageTable &PageTable::operator=(const PageTable &other) {
    if(this == &other) 
        return *this;
    table = new PageTableEntry[NUM_PAGE_TABLE_ENTRIES];
    memcpy(table, other.table, NUM_PAGE_TABLE_ENTRIES*sizeof(PageTableEntry));
    numFree = other.numFree;
    return *this;
}

PageTable::~PageTable() {
    delete[] table;
}

std::string PageTable::getMemoryMap() const {
    std::string retval;
    for(int i = 0; i < NUM_PAGE_TABLE_ENTRIES; i++) {
        if(table[i].valid) {
            char temp[6];
            snprintf(temp, 6, "%d ", table[i].ownerId);
            retval += std::string(temp);
        } else {
            retval += ". ";
        }
    }
    return retval;
}

int PageTable::getNumFree() const {
    return numFree;
}

std::pair<bool, MemoryReference> PageTable::reference(const int &pageNum, const int &id) {
    if(pageNum == 0) {
        return std::pair<bool, MemoryReference>(true, MemoryReference(id, pageNum, -1, -1, 0));
    }
    int index;
    if((index = isInTable(pageNum, id)) == -1) {
        int freePage;
        if((freePage = getFreePage()) == -1) {
            // page fault, need to replace a page
            return algImpl(pageNum, id);
        } else {
            table[freePage].numRefs = 0;
            setPage(freePage, pageNum, id);
            return std::pair<bool, MemoryReference>(false, MemoryReference(id, pageNum, -1, -1, freePage));
        }
    } else {
        table[index].numRefs++;
        return std::pair<bool, MemoryReference>(true, MemoryReference(id, pageNum, -1, -1, index));
    }
}
