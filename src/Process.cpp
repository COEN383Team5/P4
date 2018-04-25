#include <cstring>
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
    std::pair<bool, MemoryReference> refVal = pageRequestHandle(page, id); // TODO figure out how reference works
    references.push_back(refVal.second());
    return refVal.first();
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
    pageRequestHandle = NULL;
}

Process::Process(const int &id, const int &totalPageSize, const double &arrivalTime, const int &duration, const std::pair<bool, MemoryReference>(*pageRequestHandle)(const int &pageNum, const int &procId)) {
    this->id = id;
    this->totalPageSize = totalPageSize;
    this->arrivalTime = arrivalTime;
    this->duration = duration;
    runTime = 0;
    lastReference = -1;
    hits = 0;
    misses = 0;
    this->pageRequestHandle = pageRequestHandle
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

int Process::getTimeRemaining() const {
    return duration-runTime;
}

int Process:getHits() const {
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

bool Process::giveTime() {
    requestPage() ? hits++ : misses++;
    runTime += .1;
    return runTime == duration;
}

void Process::printSwapStuff(const double &timestamp, const std::string &memoryMap) const {
    std::string enterExit;
    if(getTimeRemaining() > 0) {
        enterExit = "enter";
    } else {
        enterExit = "exit";
    }
    char stringToPrint[512];
    snprintf(stringToPrint, 512, "%2.1f: Process %3d %s pages=%2d duration=%dseconds", timestamp, id, enterExit.c_str(), totalPageSize, duration);
    std::cout << stringToPrint << memoryMap;
}

void Process:printMemoryReferences() const {
    std::cout << "Process " << id << " references:\n ";
    for(unsigned int i = 0; i < references.size(); i++) {
        std::cout << references[i] << std::endl;
    }
}

std::ostream &operator<<(std::ostream &os, const Process &p) {
    char stringToPrint[512];
    snprintf(stringToPrint, 512, "Process %3d:\tatime=%2.1f\ttotalPageSize=%2d\tduration=%d\trunTime=%2.1f\thits=%3d\tmisses=%3d\tlastReference=%d", p.id, p.arrivalTime, p.totalPageSize, p.duration, p.runTime, p.hits, p.misses, p.lastReference);
    os << stringToPrint;
    return os;
}

/* helper function to quick sort  
 */
int partition(std::vector<Process> *vec, const int &low, const int &high) {
    Process temp;
    int i = (low-1); 
    double pivotVal = (*vec)[high].getArrivalTime();
    for(int j = low; j < high; j++) {
        if((*vec)[j].getArrivalTime() <= pivotVal) {
            i++;
            temp = (*vec)[j];
            (*vec)[j] = (*vec)[i];
            (*vec)[i] = temp;
        }
    }
    temp = (*vec)[i+1];
    (*vec)[i+1] = (*vec)[high];
    (*vec)[high] = temp;
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
void quickSort(std::vector<Process> *vec, const int &low, const int &high) {
    if(low < high) {
        int pivot = partition(vec, low, high);
        quickSort(vec, low, pivot-1);
        quickSort(vec, pivot+1, high);
    }
}

/* convience function to call quicksort
 */
inline void sortByArrivalTime(std::vector<Process> *vec) {
    quickSort(vec, 0, vec->size());
}

std::vector<Process> generateProcesses(const std::pair<bool, MemeoryReference>(*pageRequestHandle)(const int &pageNum, const int &procId)) {
    std::vector<Process> retval;
    unsigned long long pageSizeIndex;
    double  arrivalTime;
    int duration;
    // these are used to achieve "even distribution"
    // based upon uniform random distribution used for universial hashing
    unsigned long long a = rand()%(PRIME_FOR_UNIFORMITY-2)+1, b = rand()%PRIME_FOR_UNIFORMITY;
    for(int i = 0; i < NUM_PROCS_TO_MAKE; i++) {
        // 60 seconds with 100ms resolution
        arrivalTime = (((a*rand()+b)%PRIME_FOR_UNIFORMITY)%600)/10.0;
        duration = (((a*rand()+b)%PRIME_FOR_UNIFORMITY)%5)+1;
        pageSizeIndex = ((a*rand()+b)%PRIME_FOR_UNIFORMITY)%NUM_PAGE_OPTIONS;
        retval.emplace_back(i, pageSizes[pageSizeIndex], arrivalTime, duration, pageRequestHandle);
    }
    sortByArrivalTime(&retval);
    return retval;
}
