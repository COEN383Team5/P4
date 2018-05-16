#ifndef LRUPAGEREPLACER_H
#define LRUPAGEREPLACER_H

#include "PageTable.h"
#include "MemoryReference.h"

struct LRUPageReplacer : public PageTable {
    std::pair<bool, MemoryReference> algImpl(const int &pageNum, const int &id);
public:
    LRUPageReplacer();
    LRUPageReplacer(const LRUPageReplacer &other);
    LRUPageReplacer &operator=(const LRUPageReplacer &other);
    ~LRUPageReplacer();
};

#endif
