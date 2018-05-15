#include <cstring>
#include "PageTable.h"

int reqNum = 1;

int PageTable::isInTable(const int &pageNum, const int &id) const {
    for(int i = 0; i < NUM_PAGE_TABLE_ENTRIES; i++) {
        if(table[i].valid && table[i].ownerId == id && table[i].ownerPage == pageNum) {
            return i;
        }
    }
    return -1;
}

PageTableEntry *PageTable::getFreePage() {
    if(freePages != NULL) {
        FreePage *temp = freePages;
        freePages = freePages->next;
        numFree--;
        return temp->entry;
    }
    return NULL;
}

void PageTable::setPage(PageTableEntry *page, const int &pageNum, const int &id) {
    page->referenced = true;
    page->numRefs++;
    page->refTime = curTime;
    page->ownerId = id;
    page->ownerPage = pageNum;
    page->valid = true;
    if(page->numRefs < 2) { //for every unique page request
	reqNum++;
	page->requestNum = reqNum;
    }
    else {
	for (size_t i = 1; i < NUM_PAGE_TABLE_ENTRIES; ++i) {
		if(table[i].ownerPage == page->ownerPage) {
    			page->requestNum = table[i].requestNum;
			//if this page has been referenced before, match the requestNum
			break;
		}
	}
    }
}

void PageTable::addToTail(PageTableEntry *page) {
    if(tailPtr == NULL) {
        freePages = new FreePage();
        tailPtr = freePages;
    } else {
        tailPtr->next = new FreePage();
        tailPtr = tailPtr->next;
    }
    tailPtr->entry = page;
    tailPtr->next = NULL;
    numFree++;
}

PageTable::PageTable() {
    table = new PageTableEntry[NUM_PAGE_TABLE_ENTRIES];
    freePages = new FreePage();
    freePages->entry = &table[0];
    freePages->next = NULL;
    FreePage **temp = &freePages->next;
    for(int i = 1; i < NUM_PAGE_TABLE_ENTRIES; i++) {
        *temp = new FreePage();
        (*temp)->entry = &table[i];
        (*temp)->next = NULL;
        if(i == NUM_PAGE_TABLE_ENTRIES-1) {
            tailPtr = *temp;
        }
        temp = &((*temp)->next);
    }
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
    freePages = other.freePages;
    FreePage *thisTemp = freePages, *otherTemp = other.freePages;
    while(otherTemp != NULL) {
        thisTemp->entry = otherTemp->entry;
        thisTemp->next = otherTemp->next;
        thisTemp = thisTemp->next;
        otherTemp = otherTemp->next;
    }
    return *this;
}

PageTable::~PageTable() {
    delete[] table;
    while(freePages != NULL) {
        FreePage *temp = freePages->next;
        delete freePages;
        freePages = temp;
    }
    freePages = NULL;
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

std::pair<bool, MemoryReference> PageTable::reference(const int &pageNum, const int &id, const double &timeStamp) {
    if(pageNum == 0) {
        return std::pair<bool, MemoryReference>(true, MemoryReference(timeStamp, id, pageNum, -1, -1, 0));
    }
    std::pair<bool, MemoryReference> retval;
    int index;
    std::unique_lock<std::mutex> lock(tableMut);
    if((index = isInTable(pageNum, id)) == -1) {
        PageTableEntry *freePage;
        if((freePage = getFreePage()) == NULL) {
            // page fault, need to replace a page
            retval = algImpl(pageNum, id, timeStamp);
            std::cout << retval.second << std::endl;
        } else {
            freePage->numRefs = 0;
            setPage(freePage, pageNum, id);
            index = isInTable(freePage->ownerPage, freePage->ownerId);
            retval = std::pair<bool, MemoryReference>(false, MemoryReference(timeStamp, id, pageNum, -1, -1, index));
        }
    } else {
        table[index].numRefs++;
        retval = std::pair<bool, MemoryReference>(true, MemoryReference(timeStamp, id, pageNum, -1, -1, index));
    }
    return retval;
}

void PageTable::swapOff(const int &pid) {
    for(int i = 0; i < NUM_PAGE_TABLE_ENTRIES; i++) {
        if(table[i].ownerId == pid) {
            table[i].valid = false;
            addToTail(&table[i]);
        }
    }
}
