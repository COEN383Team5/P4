#include <cstdlib>
#include <iostream>
#include <vector>
#include "MemoryReference.h"
#include "Process.h"
#include "RandomPageReplacer.h"

// in seconds
#define RUN_TIME 60

double curTime;

// TODO define other handlers
PageTable *parseArguments(int argc, char *argv[]) {
    PageTable *handler;
    handler = new RandomPageReplacer();
    return handler;
}

void runAlg(PageTable *handler, Process *procs) {
    int procsIndex = 0, numCompleted = 0;
    std::vector<Process *> runningProcs;
    for(unsigned int i = 0; i < NUM_PROCS_TO_MAKE; i++) {
        std::cout << procs[i] << std::endl;
    }
    // curTime is in seconds with 100ms resolution
    for(curTime = 0; curTime < RUN_TIME; curTime += .1) {
        while(handler->getNumFree() >= 4 && procsIndex < NUM_PROCS_TO_MAKE && procs[procsIndex].getArrivalTime() <= curTime) {
            // while there are free pages, and the next proc has arrived
            procs[procsIndex].printSwapStuff(curTime, handler->getMemoryMap());
            runningProcs.push_back(&procs[procsIndex++]);
        }
        if(runningProcs.size() > 0) {
            // if there are procs that are paged in and need to run, run them
            for(std::vector<Process *>::iterator it = runningProcs.begin(); it != runningProcs.end(); ++it) {
                if((*it)->getTimeRemaining() > 0 && (*it)->giveTime()) {
                    // the process has run its duration
                    (*it)->printSwapStuff(curTime, handler->getMemoryMap());
                    numCompleted++;
                }
            }
            std::vector<Process *> newRunningProcs;
            for(unsigned int i = 0; i < runningProcs.size(); i++) {
                if(runningProcs[i]->getTimeRemaining() > 0) {
                    newRunningProcs.push_back(runningProcs[i]);
                }
            }
            runningProcs = newRunningProcs;
        }
    }
    std::cout << "Number of completed processes=" << numCompleted << std::endl;
    for(int i = 0; i < NUM_PROCS_TO_MAKE; i++) {
        std::cout << procs[i] << std::endl;
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    PageTable *handler = parseArguments(argc, argv);
    Process *procs = generateProcesses(handler);
    runAlg(handler, procs);
    delete handler;
    delete[] procs;
    return 0;
}
