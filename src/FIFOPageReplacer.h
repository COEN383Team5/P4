#ifndef FIFOPAGEREPLACER_H
#define FIFOPAGEREPLACER_H

#include "PageTable.h"
#include "MemoryReference.h"

struct FIFOPageReplacer : public PageTable {
    std::pair<bool, MemoryReference> algImpl(const int &pageNum, const int &id, const double &timeStamp);
public:
    FIFOPageReplacer();
    FIFOPageReplacer(const FIFOPageReplacer &other);
    FIFOPageReplacer &operator=(const FIFOPageReplacer &other);
    ~FIFOPageReplacer();
};

#endif
