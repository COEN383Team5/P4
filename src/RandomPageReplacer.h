#ifndef RANDOMPAGEREPLACER_H
#define RANDOMPAGEREPLACER_H

#include "PageTable.h"
#include "MemoryReference.h"

struct RandomPageReplacer : public PageTable {
    std::pair<bool, MemoryReference> algImpl(const int &pageNum, const int &id, const double &timeStamp);
public:
    RandomPageReplacer();
    RandomPageReplacer(const RandomPageReplacer &other);
    RandomPageReplacer &operator=(const RandomPageReplacer &other);
    ~RandomPageReplacer();
};

#endif
