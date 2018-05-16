#include <cstring>
#include <chrono>
#include "Process.h"

const int pageSizes[] = {5, 11, 17, 31};

bool Process::requestPage() {
    int page;
    if(lastReference == -1) {
        // first reference
        page = INIT_PAGE;
    } else if(lastReference == 0) {
        // second reference
        page = rand()%(totalPageSize)+1;
    } else {
        // subsequent references
        int offset;
        if(rand()%10 < 7) {
            offset = (rand()%3)-1; // number from -1 to 1
        } else {
            offset = (rand()%(totalPageSize-1))+2;
        } 
        // page will always be in the range from [1, totalPageSize]
        if(lastReference+offset == 0) {
            page = (totalPageSize)-(lastReference+offset); 
        } else if(lastReference+offset > totalPageSize) {
            page = (lastReference+offset)-(totalPageSize);
        } else {
            page = lastReference+offset;
        }
    }
    lastReference = page;
    std::pair<bool, MemoryReference> refVal = ptHandler->reference(page, id); 
    references.push_back(refVal.second);
    return refVal.first;
}

bool Process::giveTime() {
    requestPage() ? hits++ : misses++;
    runTime += .1;
    return (int)runTime != duration;
}

Process::Process() {
    id = -1;    
    totalPageSize = -1;
    duration = -1;
    arrivalTime = -1;
    lastReference = -1;
    runTime = 0;
    hits = 0;
    misses = 0;
    ptHandler = NULL;
}

Process::Process(const int &id, const int &totalPageSize, const double &arrivalTime, const int &duration, PageTable *pt) {
    this->id = id;
    this->totalPageSize = totalPageSize;
    this->arrivalTime = arrivalTime;
    this->duration = duration;
    runTime = 0;
    lastReference = -1;
    hits = 0;
    misses = 0;
    ptHandler = pt;
}

int Process::getId() const {
    return id;
}

int Process::getTotalPageSize() const {
    return totalPageSize;
}

double Process::getArrivalTime() const {
    return arrivalTime;
}

int Process::getDuration() const {
    return duration;
}

double Process::getTimeRemaining() const {
    return duration-runTime;
}

int Process::getHits() const {
    return hits;
}

int Process::getMisses() const {
    return misses;
}

double Process::getRunTime() const {
    return runTime;
}

std::vector<MemoryReference> Process::getReferences() const {
    return references;
}

void Process::start(double curTime) {
    int iters = 0;
    printSwapStuff(ptHandler->getMemoryMap());
    while(giveTime()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        iters++;
    }
    ptHandler->swapOff(id);
    printSwapStuff(ptHandler->getMemoryMap());
}

void Process::printSwapStuff(const std::string &memoryMap) const {
    std::string enterExit;
    if(getTimeRemaining() > 0) {
        enterExit = "enter";
    } else {
        enterExit = "exit";
    }
    char stringToPrint[512];
    snprintf(stringToPrint, 512, "%2.1f: Process %3d %s pages=%2d duration=%dseconds\n", getTime(), id, enterExit.c_str(), totalPageSize, duration);
    std::unique_lock<std::mutex> lock(stdoutMut);
    std::cout << stringToPrint << memoryMap << std::endl;
}

void Process::printMemoryReferences() const {
    std::unique_lock<std::mutex> lock(stdoutMut);
    std::cout << "Process " << id << " references:\n ";
    for(unsigned int i = 0; i < references.size(); i++) {
        std::cout << references[i] << std::endl;
    }
}

std::ostream &operator<<(std::ostream &os, const Process &p) {
    char stringToPrint[512];
    snprintf(stringToPrint, 512, "Process %3d:\tatime=%2.1f\ttotalPageSize=%2d\tduration=%d\trunTime=%2.1f\thits=%3d\tmisses=%3d\thits/misses=%.4f\tlastReference=%d", p.id, p.arrivalTime, p.totalPageSize, p.duration, p.runTime, p.hits, p.misses, p.hits/(double)p.misses, p.lastReference);
    std::unique_lock<std::mutex> lock(stdoutMut);
    os << stringToPrint;
    return os;
}

/* helper function to quick sort  
 */
int partition(Process *vec, const int &low, const int &high) {
    Process temp;
    int i = (low-1); 
    double pivotVal = vec[high].getArrivalTime();
    for(int j = low; j < high; j++) {
        if(vec[j].getArrivalTime() <= pivotVal) {
            i++;
            temp = vec[j];
            vec[j] = vec[i];
            vec[i] = temp;
        }
    }
    temp = vec[i+1];
    vec[i+1] = vec[high];
    vec[high] = temp;
    return i+1;
}

/* sorts vec by arrival time, sorts inbetween low and high
 * @param vec
 *      a reference to a vector of processes to sort by arrival time
 * @param low
 *      the low index to sort from 
 * @param high
 *      the high index to sort to
 */
void quickSort(Process *vec, const int &low, const int &high) {
    if(low < high) {
        int pivot = partition(vec, low, high);
        quickSort(vec, low, pivot-1);
        quickSort(vec, pivot+1, high);
    }
}

/* convience function to call quicksort
 */
inline void sortByArrivalTime(Process *vec) {
    quickSort(vec, 0, NUM_PROCS_TO_MAKE-1);
}

Process *generateProcesses(PageTable *pt) {
    Process *retval = new Process[NUM_PROCS_TO_MAKE];
    unsigned long long pageSizeIndex;
    double  arrivalTime;
    int duration;
    // these are used to achieve "even distribution"
    // based upon uniform random distribution used for universial hashing
    unsigned long long a = rand()%(PRIME_FOR_UNIFORMITY-2)+1, b = rand()%PRIME_FOR_UNIFORMITY;
    for(int i = 0; i < NUM_PROCS_TO_MAKE; i++) {
        // processes can be created at anytime before RUN_TIME-5.1 seconds with 100ms resolution
        arrivalTime = (((a*rand()+b)%PRIME_FOR_UNIFORMITY)%((RUN_TIME*10)-510))/10.0;
        std::cout << "atime " << arrivalTime << std::endl;
        duration = (((a*rand()+b)%PRIME_FOR_UNIFORMITY)%5)+1;
        pageSizeIndex = ((a*rand()+b)%PRIME_FOR_UNIFORMITY)%NUM_PAGE_OPTIONS;
        retval[i] = Process(i, pageSizes[pageSizeIndex], arrivalTime, duration, pt);
    }
    sortByArrivalTime(retval);
    return retval;
}
