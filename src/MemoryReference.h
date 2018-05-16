#ifndef MEMORYREFERENCE_H
#define MEMORYREFERENCE_H

#include <iostream>
#include <string>
#include "Log.h"

// defined in P4, used for timestamping
extern double startTime;
extern double getTime();

struct MemoryReference {
    // in seconds
    double timeStamp;
    // pageReferenced is the page according to the process
    int processId, pageReferenced;
    std::string evicted;
    // pageInMemory is the physical page according to the simulated OS
    int pageInMemory;

    MemoryReference();

    /* This constructor will set timeStamp to curTime
     */
    MemoryReference(const int &pageReferenced, const int &processId, const int &evictedPage, const int &evictedProc, const int &pageInMemory); 

    /* makes use of stdoutMut defined in Log.h and initialized in P4
     */
    friend std::ostream &operator<<(std::ostream &o, const MemoryReference &m);
};

#endif
