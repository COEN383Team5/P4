#ifndef MEMORYREFERENCE_H
#define MEMORYREFERENCE_H

struct MemoryReference {
    // in seconds
    double timeStamp;
    // pageReferenced is the page according to the process
    int processId, pageReferenced, whoKickedOut;
    // pageInMemory is the physical page according to the simulated OS
    int pageInMemory;
};

#endif
