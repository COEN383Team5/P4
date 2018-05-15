#ifndef PROCESS_H
#define PROCESS_H

#include <iostream>
#include <vector>
#include <utility>
#include <string>
#include <thread>
#include "Log.h"
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

    /* decrements time remaining and generates a page request
     * @retval false if the process has no time remaining
     *      true if the process still needs to run
     */
    bool giveTime();
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

    /* begins the processes thread of execution, which references pages every 100ms
     * @param curTime
     *      the current time when this process is started
     */
    void start(double curTime);

    /* prints the information about a process when it enters or exits memory
     * this should be run by the simulator when the process begins and finishes execution
     * makes use of stdoutMut defined in Log.h and initialized in P4
     * @param memoryMap
     *      the string representing the state of memory 
     *      this should be generated from the memory controller
     */
    void printSwapStuff(const std::string &memoryMap) const;

    /* prints the information about a process' memory references
     * makes use of stdoutMut defined in Log.h and initialized in P4
     */
    void printMemoryReferences() const;

    /* prints all of the information pertaining to this process
     * makes use of stdoutMut defined in Log.h and initialized in P4
     */
    friend std::ostream &operator<<(std::ostream &o, const Process &p);
};

/* generates processes according to the project description
 * sorts them by arrival time
 * @retval the generated processes
 */
extern Process *generateProcesses(PageTable *pt);

#endif
