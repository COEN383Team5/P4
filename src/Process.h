#ifndef PROCESS_H
#define PROCESS_H

#define NUM_PROCS_TO_MAKE 150
#define NUM_PAGE_OPTIONS 4
// http://primes.utm.edu/lists/2small/0bit.html
// this is the prime (2^8)-5
#define PRIME_FOR_UNIFORMITY 251
#define INIT_PAGE 0

#include <iostream>
#include <vector>
#include <pair>
#include <string>
#include "MemoryReference.h"

extern const int pageSizes[];

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
    // this is a function pointer to what will handle a page request
    // TODO figure out where it should point
    std::pair<bool, MemoryReference> (*pageRequestHandle)(const int &pageNum, const int &procId);

    /* Generates a page request and asks the memory manager for the page
     * and fills references
     * @retval true if the page was already in memory
     *      false if the requested page caused a miss
     */
    bool requestPage();
public:
    Process();
    Process(const int &id, const int &totalPageSize, const double &arrivalTime, const int &duration, const std::pair<bool, MemoryReference>(*pageRequestHandle)(const int &pageNum, const int &procId));

    int getId() const;
    int getTotalPageSize() const;
    double getArrivalTime() const;
    int getDuration() const;
    int getTimeRemaining() const;
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
    friend std::ostream &operator<<(std::ostream &o, const Process &p);
};

/* generates processes according to the project description
 * sorts them by arrival time
 * @retval the generated processes
 */
extern std::vector<Process> generateProcesses();

#endif
