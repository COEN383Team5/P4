#include <cstring>
#include "MemoryReference.h"

MemoryReference::MemoryReference() {
    timeStamp = -1;
    processId = -1;
    pageReferenced = -1;
    evicted = -1;
    pageInMemory = -1;
} 

MemoryReference::MemoryReference(const double &timeStamp, const int &pageReferenced, const int &processId, const int &evictedPage, const int &evictedProc, const int &pageInMemory) {
    this->timeStamp = timeStamp;
    this->processId = processId;
    this->pageReferenced = pageReferenced;
    if(evictedPage != -1) {
        char temp[64];
        snprintf(temp, 64, "Process %d page %d", evictedProc, evictedPage);
        evicted = std::string(temp);
    } else {
        evicted = std::string("None");
    }
    this->pageInMemory = pageInMemory;
}

std::ostream &operator<<(std::ostream &os, const MemoryReference &m) {
    std::unique_lock<std::mutex> lock(stdoutMut);
    os << m.timeStamp << ": Process " << m.processId 
        << " referenced virtual page " << m.pageReferenced 
        << " which maps to physical page " << m.pageInMemory 
        << " and evicted " << m.evicted;
    return os;
}
