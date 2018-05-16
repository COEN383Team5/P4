#include <cstdlib>
#include <iostream>
#include <vector>
#include <mutex>
#include <chrono>
#include "Log.h"
#include "MemoryReference.h"
#include "Process.h"
#include "MFUPageReplacer.h"
#include "LFUPageReplacer.h"
#include "LRUPageReplacer.h"
#include "RandomPageReplacer.h"

// in seconds
#define RUN_TIME 60

double startTime;
std::mutex stdoutMut;

const char *helpText =
        "Usage:\n"
        "\tP4 [algType | numRuns]\n"
        "Where:\n"
        "\talgType is the name of an algorith to run. Possible options are: FIFO, LRU, LFU, MFU, RAND\n\n"
        "\tnumRuns is the number of times you want to run each algorithm in a row. In this case it will run all of the algoritms\n\n"
        "Only one of the arguments is required.\n";

template<typename Type> void makeRun() {
    Type handler;
    Process *procs = generateProcesses(&handler);
    runAlg(&handler, procs);
    delete[] procs;
}

void printBarrier(const std::string& algName, int num) {
    std::cout << "################################################" << std::endl;
    std::cout << "######################## " << algName << " RUN " << num << " ########################" << std::endl;
    std::cout << "################################################" << std::endl;
}

void parseArguments(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << helpText << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string arg(argv[1]);

    try {
        int numRuns = std::stoi(arg);
        for (int i = 0; i < numRuns; ++i) {
            // printBarrier("FIFO", i);
            //makeRun<FIFOPageReplacer>();  TODO uncomment
            printBarrier("LRU", i);
            makeRun<LRUPageReplacer>();

            printBarrier("LFU", i);
            makeRun<LFUPageReplacer>();

            printBarrier("MFU", i);
            makeRun<MFUPageReplacer>();

            printBarrier("Random", i);
            makeRun<RandomPageReplacer>();
        }
    } catch (const std::invalid_argument&) {
        if (arg == "FIFO") {
            //printBarrier("FIFO", 0);
            // makeRun<FIFOPageReplacer>(); TODO uncomment
        } else if (arg == "LRU") {
            printBarrier("LRU", 0);
            makeRun<LRUPageReplacer>();
        } else if (arg == "LFU") {
            printBarrier("LFU", 0);
            makeRun<LFUPageReplacer>();
        } else if (arg == "MFU") {
            printBarrier("MFU", 0);
            makeRun<MFUPageReplacer>();
        } else if (arg == "RAND") {
            printBarrier("Random", 0);
            makeRun<RandomPageReplacer>();
        } else {
            std::cout << helpText << std::endl;
            exit(EXIT_FAILURE);
        }
    }
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

    double averageHitMissRatio = 0;
    for(int i = 0; i < NUM_PROCS_TO_MAKE; i++) {
        std::cout << procs[i] << std::endl;
        if (procs[i].getMisses() != 0) {
            averageHitMissRatio += procs[i].getHits() / (double)procs[i].getMisses();
        }
    }
    averageHitMissRatio /= NUM_PROCS_TO_MAKE;
    stdoutMut.lock();
    std::cout << "Average Hit/Miss Ratio=" << averageHitMissRatio << std::endl;
    stdoutMut.unlock();
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
