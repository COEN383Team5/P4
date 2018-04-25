#ifndef RANDOMPAGEREPLACER_H
#define RANDOMPAGEREPLACER_H

#include "PageTable.h"
#include "MemoryReference.h"

struct RandomPageReplacer : public PageTable {
    std::pair<bool, MemoryReference> algImpl(const int &pageNum, const int &id);
public:
    RandomPageReplacer();
};

#endif
