#include <cstdlib>
#include <iostream>
#include <vector>
#include <mutex>
#include <chrono>
#include <cstring>
#include "Log.h"
#include "MemoryReference.h"
#include "Process.h"
#include "MFUPageReplacer.h"
#include "LFUPageReplacer.h"
#include "LRUPageReplacer.h"
#include "RandomPageReplacer.h"
#include "FIFOPageReplacer.h"


double startTime;
std::mutex stdoutMut;

const char *helpText =
        "Usage:\n"
        "\tP4 [algType | numRuns]\n"
        "Where:\n"
        "\talgType is the name of an algorith to run. Possible options are: FIFO, LRU, LFU, MFU, RAND\n\n"
        "\tnumRuns is the number of times you want to run each algorithm in a row. In this case it will run all of the algoritms\n\n"
        "Only one of the arguments is required.\n";

template<typename Type> Process *makeRun(Process *procs) {
    Type handler;
    if(procs == NULL) {
        procs = generateProcesses(&handler);
    }
    Process *retval = new Process[NUM_PROCS_TO_MAKE];;
    memcpy(retval, procs, sizeof(Process*)*NUM_PROCS_TO_MAKE);
    runAlg(&handler, procs);
    delete[] procs;
    return retval;
}

void printBarrier(const std::string& algName, int num) {
    std::cerr << "################################################" << std::endl;
    std::cerr << "######################## " << algName << " RUN " << num << " ########################" << std::endl;
    std::cerr << "################################################" << std::endl;
}

void parseArguments(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << helpText << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string arg(argv[1]);

    Process *procs = NULL;
    try {
        int numRuns = std::stoi(arg);
        for (int i = 0; i < numRuns; ++i) {
            printBarrier("FIFO", i);
            procs = makeRun<FIFOPageReplacer>(procs);

            printBarrier("LRU", i);
            procs = makeRun<LRUPageReplacer>(procs);

            printBarrier("LFU", i);
            procs = makeRun<LFUPageReplacer>(procs);

            printBarrier("MFU", i);
            procs = makeRun<MFUPageReplacer>(procs);

            printBarrier("Random", i);
            procs = makeRun<RandomPageReplacer>(procs);
        }
    } catch (const std::invalid_argument&) {
        if (arg == "FIFO") {
            printBarrier("FIFO", 0);
            procs = makeRun<FIFOPageReplacer>(procs); 
        } else if (arg == "LRU") {
            printBarrier("LRU", 0);
            procs = makeRun<LRUPageReplacer>(procs);
        } else if (arg == "LFU") {
            printBarrier("LFU", 0);
            procs = makeRun<LFUPageReplacer>(procs);
        } else if (arg == "MFU") {
            printBarrier("MFU", 0);
            procs = makeRun<MFUPageReplacer>(procs);
        } else if (arg == "RAND") {
            printBarrier("Random", 0);
            procs = makeRun<RandomPageReplacer>(procs);
        } else {
            std::cout << helpText << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    delete[] procs;
}

void runAlg(PageTable *handler, Process *procs) {
    int procsIndex = 0, numCompleted = 0;
    std::vector<std::thread> threads;
    for(int i = 0; i < NUM_PROCS_TO_MAKE; i++) {
        // invokes the ostream operator that is a friend of Process
        // which locks stdoutMut before printing
        std::cout << procs[i] << std::endl;
    }
    startTime = 0;
    startTime = getTime();
    double curTime;
    while((curTime = getTime()) < RUN_TIME) {
        while(handler->getNumFree() >= 4 // there are enough free pages
            && procsIndex < NUM_PROCS_TO_MAKE // not all the process have been run
            && procs[procsIndex].getArrivalTime() <= curTime
            && curTime+procs[procsIndex].getDuration() <= RUN_TIME) { // there is time to run the process
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

    double averageHitMissRatio = 0;
    for(int i = 0; i < NUM_PROCS_TO_MAKE; i++) {
        std::cout << procs[i] << std::endl;
        if (procs[i].getMisses() != 0) {
            averageHitMissRatio += procs[i].getHits() / (double)procs[i].getMisses();
        }
    }
    averageHitMissRatio /= NUM_PROCS_TO_MAKE;
    std::cerr << "Average Hit/Miss Ratio=" << averageHitMissRatio << std::endl;
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    parseArguments(argc, argv);
    // PageTable *handler = parseArguments(argc, argv);
    // Process *procs = generateProcesses(handler);
    // runAlg(handler, procs);
    // delete handler;
    // delete[] procs;
    return 0;
}
