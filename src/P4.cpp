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

void printBarrier(const std::string& algName, int num) {
    std::cerr << "################################################" << std::endl;
    std::cerr << "######################## " << algName << " RUN " << num << " ########################" << std::endl;
    std::cerr << "################################################" << std::endl;
}

/* return value is the average hit ratio for this run */
double runAlg(PageTable *handler, Process *procs) {
    Process *procCopy = new Process[NUM_PROCS_TO_MAKE];
    memcpy(procCopy, procs, sizeof(Process)*NUM_PROCS_TO_MAKE);
    for(int i = 0; i < NUM_PROCS_TO_MAKE; i++) {
        procCopy[i].setPTHandler(handler);
    }
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
            && procCopy[procsIndex].getArrivalTime() <= curTime
            && curTime+procCopy[procsIndex].getDuration() <= RUN_TIME) { // there is time to run the process
            threads.push_back(std::thread(&Process::start, &procCopy[procsIndex++], curTime));
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
        std::cout << procCopy[i] << std::endl;
        if (procCopy[i].getMisses() != 0) {
            averageHitMissRatio += procCopy[i].getHits() / (double)procCopy[i].getMisses();
        }
    }
    averageHitMissRatio /= NUM_PROCS_TO_MAKE;
    std::cerr << "Average Hit/Miss Ratio=" << averageHitMissRatio << std::endl;
    delete[] procCopy;
    return averageHitMissRatio;
}

void parseArguments(int argc, char *argv[]) {
    if (argc != 2) {
        std::cout << helpText << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string arg(argv[1]);

    Process *procs = generateProcesses();
    try {
        int numRuns = std::stoi(arg);
        double aveHolder[5] = {0, 0, 0, 0, 0};
        for (int i = 0; i < numRuns; ++i) {
            FIFOPageReplacer fifoHandler = FIFOPageReplacer();
            LRUPageReplacer lruHandler = LRUPageReplacer();
            LFUPageReplacer lfuHandler = LFUPageReplacer();
            MFUPageReplacer mfuHandler = MFUPageReplacer();
            RandomPageReplacer randomHandler = RandomPageReplacer();
            printBarrier("FIFO", i);
            aveHolder[0] += runAlg(&fifoHandler, procs);

            printBarrier("LRU", i);
            aveHolder[1] += runAlg(&lruHandler, procs);

            printBarrier("LFU", i);
            aveHolder[2] += runAlg(&lfuHandler, procs);

            printBarrier("MFU", i);
            aveHolder[3] += runAlg(&mfuHandler, procs);

            printBarrier("Random", i);
            aveHolder[4] += runAlg(&randomHandler, procs);
        }
        for(int i = 0; i < 5; i++) {
            aveHolder[i] /= numRuns;
        }
        std::cerr   << "Fifo average hit/miss " << aveHolder[0] << std::endl
                    << "LRU average hit/miss " << aveHolder[1] << std::endl
                    << "LFU average hit/miss " << aveHolder[2] << std::endl
                    << "MFU average hit/miss " << aveHolder[3] << std::endl
                    << "Random average hit/miss " << aveHolder[4] << std::endl;
    } catch (const std::invalid_argument&) {
        if (arg == "FIFO") {
            FIFOPageReplacer fifoHandler = FIFOPageReplacer();
            printBarrier("FIFO", 0);
            runAlg(&fifoHandler, procs);
        } else if (arg == "LRU") {
            LRUPageReplacer lruHandler = LRUPageReplacer();
            printBarrier("LRU", 0);
            runAlg(&lruHandler, procs);
        } else if (arg == "LFU") {
            LFUPageReplacer lfuHandler = LFUPageReplacer();
            printBarrier("LFU", 0);
            runAlg(&lfuHandler, procs);
        } else if (arg == "MFU") {
            MFUPageReplacer mfuHandler = MFUPageReplacer();
            printBarrier("MFU", 0);
            runAlg(&mfuHandler, procs);
        } else if (arg == "RAND") {
            RandomPageReplacer randomHandler = RandomPageReplacer();
            printBarrier("Random", 0);
            runAlg(&randomHandler, procs);
        } else {
            std::cout << helpText << std::endl;
            exit(EXIT_FAILURE);
        }
    }
    delete[] procs;
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
