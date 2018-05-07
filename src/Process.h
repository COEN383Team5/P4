#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include "MemoryReference.h"
#include "PageTable.h"

#define NUM_PROCS_TO_MAKE 150
#define NUM_PAGE_OPTIONS 4
// http://primes.utm.edu/lists/2small/0bit.html
// this is the prime (2^8)-5
#define PRIME_FOR_UNIFORMITY 251
#define INIT_PAGE 0

extern const int pageSizes[];

// pages for this process are always numbered from 1 to totalPageSize
// page 0 is reserved and will always hit
class Process {
    int id, totalPageSize;
    // in seconds
    int duration;
    // the last page requested by this process
    // -1 if no page has been requested yet
    int lastReference;
    // in seconds
    double arrivalTime, runTime;
    int hits, misses;
    std::vector<MemoryReference> references;
    PageTable *ptHandler;

    /* Generates a page request and asks the memory manager for the page
     * and fills references
     * @retval true if the page was already in memory
     *      false if the requested page caused a miss
     */
    bool requestPage();
public:
    Process();
    Process(const int &id, const int &totalPageSize, const double &arrivalTime, const int &duration, PageTable *pt);

    int getId() const;
    int getTotalPageSize() const;
    double getArrivalTime() const;
    int getDuration() const;
    double getTimeRemaining() const;
    int getHits() const;
    int getMisses() const;
    double getRunTime() const;
    std::vector<MemoryReference> getReferences() const;

    /* decrements time remaining and generates a page request
     * @retval true if the process has no time remaining
     *      false if the process still needs to run
     */
    bool giveTime();

    /* prints the information about a process when it enters or exits memory
     * this should be run by the simulator when the process begins and finishes execution
     * @param timestamp
     *      the current time for the simulator
     * @param memoryMap
     *      the string representing the state of memory 
     *      this should be generated from the memory controller
     */
    void printSwapStuff(const double &timestamp, const std::string &memoryMap) const;

    /* prints the information about a process' memory references
     */
    void printMemoryReferences() const;
    friend std::ostream &operator<<(std::ostream &o, const Process &p);
};

/* generates processes according to the project description
 * sorts them by arrival time
 * @retval the generated processes
 */
extern Process *generateProcesses(PageTable *pt);

#endif
