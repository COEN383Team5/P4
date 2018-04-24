#ifndef PROCESS_H
#define PROCESS_H

#define NUM_PROCS_TO_MAKE 150
#define NUM_PAGE_OPTIONS 4
// http://primes.utm.edu/lists/2small/0bit.html
// this is the prime (2^8)-5
#define PRIME_FOR_UNIFORMITY 251

#include <iostream>
#include <vector>
#include "MemoryReference.h"

extern const int pageSizes[];

class Process {
    int id, totalPageSize;
    // in seconds
    int duration, timeRemaing;
    // the last page requested by this process
    // -1 if no page has been requested yet
    int lastReference;
    // in seconds
    double arrivalTime;
    std::vector<MemoryReference> references;

public:
    // in seconds
    double runTime;

    Process();
    Process(const int &id, const int &totalPageSize, const double &arrivalTime, const int &duration);
    int getId();
    int getTotalPageSize();
    double getArrivalTime();
    int getDuration();
    bool giveTime();
    friend std::ostream &operator<<(std::ostream &o, const Process &p);
};

extern std::vector<Process> generateProcesses();

#endif
