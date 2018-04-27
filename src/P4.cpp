#include <cstdlib>
#include <iostream>
#include <vector>
#include "MemoryReference.h"
#include "Process.h"
#include "RandomPageReplacer.h"

// in seconds
#define RUN_TIME 60

double curTime;

int main(int argc, char *argv[]) {
    srand(time(NULL));
    RandomPageReplacer pageTable;
    int procsIndex = 0, numCompleted = 0;
    // TODO define handler
    Process *procs = generateProcesses(&pageTable);
    std::vector<Process *> runningProcs;
    for(unsigned int i = 0; i < NUM_PROCS_TO_MAKE; i++) {
        std::cout << procs[i] << std::endl;
    }
    // curTime is in seconds with 100ms resolution
    for(curTime = 0; curTime < RUN_TIME; curTime += .1) {
        while(pageTable.getNumFree() >= 4 && procsIndex < NUM_PROCS_TO_MAKE && procs[procsIndex].getArrivalTime() <= curTime) {
            procs[procsIndex].printSwapStuff(curTime, pageTable.getMemoryMap());
            runningProcs.push_back(&procs[procsIndex++]);
        }
        if(runningProcs.size() > 0) {
            std::vector<std::vector<Process *>::iterator> finishedProcs;
            for(std::vector<Process *>::iterator it = runningProcs.begin(); it != runningProcs.end(); ++it) {
                if((*it)->giveTime()) {
                    // the process has run its duration
                    (*it)->printSwapStuff(curTime, pageTable.getMemoryMap());
                    finishedProcs.push_back(it);
                    numCompleted++;
                }
            }
            for(unsigned int i = 0; i < finishedProcs.size(); i++) {
                runningProcs.erase(finishedProcs[i]);
            }
        }
    }
    std::cout << "Number of completed processes=" << numCompleted;
    delete[] procs;
    return 0;
}
