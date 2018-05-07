#include <cstdlib>
#include <iostream>
#include <vector>
#include <mutex>
#include "Log.h"
#include "MemoryReference.h"
#include "Process.h"
#include "RandomPageReplacer.h"

// in seconds
#define RUN_TIME 60

double curTime;
std::mutex stdoutMut;

// TODO run 5 times and print averages
// TODO must say what processes stole which pages from which processes
// TODO define other handlers
PageTable *parseArguments(int argc, char *argv[]) {
    PageTable *handler;
    handler = new RandomPageReplacer();
    return handler;
}

void runAlg(PageTable *handler, Process *procs) {
    int procsIndex = 0, numCompleted = 0;
    std::vector<std::thread> threads;
    for(int i = 0; i < NUM_PROCS_TO_MAKE; i++) {
        // invokes the ostream operator that is a friend of Process
        // which locks stdoutMut before printing
        std::cout << procs[i] << std::endl;
    }
    // curTime is in seconds with 100ms resolution
    for(curTime = 0; curTime < RUN_TIME; curTime += .1) {
        while(handler->getNumFree() >= 4 // there are enough free pages
            && procsIndex < NUM_PROCS_TO_MAKE // not all the process have been run
            && procs[procsIndex].getArrivalTime() <= curTime // the process has arrived
            && curTime+procs[procsIndex].getDuration() < RUN_TIME) { // there is time to run the process
            threads.push_back(std::thread(&Process::start, &procs[procsIndex++], curTime));
        }
    }
    for(unsigned i = 0; i < threads.size(); i++) {
        threads[i].join();
        numCompleted++;
    }
    stdoutMut.lock();
    std::cout << "Number of completed processes=" << numCompleted << std::endl;
    stdoutMut.unlock();
    for(int i = 0; i < NUM_PROCS_TO_MAKE; i++) {
        std::cout << procs[i] << std::endl;
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    PageTable *handler = parseArguments(argc, argv);
    Process *procs = generateProcesses(handler);
    runAlg(handler, procs);
    delete handler; //the warning this causes can never happen as handler is a non abstract instance of a class that inherits from PageTable
    delete[] procs;
    return 0;
}
