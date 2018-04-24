#include <cstring>
#include "Process.h"

const int pageSizes[] = {5, 11, 17, 31};

Process::Process() {
    id = -1;    
    totalPageSize = -1;
    duration = -1;
    arrivalTime = -1;
    lastReference = -1;
    runTime = 0;
    timeRemaing = 0;
}

Process::Process(const int &id, const int &totalPageSize, const double &arrivalTime, const int &duration) {
    this->id = id;
    this->totalPageSize = totalPageSize;
    this->arrivalTime = arrivalTime;
    this->duration = duration;
    this->timeRemaing = duration;
    runTime = 0;
    lastReference = -1;
}

int Process::getId() {
    return id;
}

int Process::getTotalPageSize() {
    return totalPageSize;
}

double Process::getArrivalTime() {
    return arrivalTime;
}

int Process::getDuration() {
    return duration;
}

bool Process::giveTime() {
    // TODO
}

std::ostream &operator<<(std::ostream &os, const Process &p) {
    char stringToPrint[256];
    snprintf(stringToPrint, 256, "Process %3d:\tatime=%2.1f\ttotalPageSize=%2d\tduration=%d\t", p.id, p.arrivalTime, p.totalPageSize, p.duration);
    os << stringToPrint;
    return os;
}

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

void quickSort(std::vector<Process> *vec, const int &low, const int &high) {
    if(low < high) {
        int pivot = partition(vec, low, high);
        quickSort(vec, low, pivot-1);
        quickSort(vec, pivot+1, high);
    }
}

void sortByArrivalTime(std::vector<Process> *vec) {
    quickSort(vec, 0, vec->size());
}

std::vector<Process> generateProcesses() {
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
        retval.emplace_back(i, pageSizes[pageSizeIndex], arrivalTime, duration);
    }
    sortByArrivalTime(&retval);
    return retval;
}
