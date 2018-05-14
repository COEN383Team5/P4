#ifndef LFUPAGEREPLACER_H
#define LFUPAGEREPLACER_H

#include "PageTable.h"
#include "MemoryReference.h"

struct LFUPageReplacer : public PageTable {
    std::pair<bool, MemoryReference> algImpl(const int &pageNum, const int &id, const double &timeStamp);
public:
    LFUPageReplacer();
    LFUPageReplacer(const LFUPageReplacer &other);
    LFUPageReplacer &operator=(const LFUPageReplacer &other);
    ~LFUPageReplacer();
};

#endif
