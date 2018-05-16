#ifndef MFUPAGEREPLACER_H
#define MFUPAGEREPLACER_H

#include "PageTable.h"
#include "MemoryReference.h"

struct MFUPageReplacer : public PageTable {
    std::pair<bool, MemoryReference> algImpl(const int &pageNum, const int &id);
public:
    MFUPageReplacer();
    MFUPageReplacer(const MFUPageReplacer &other);
    MFUPageReplacer &operator=(const MFUPageReplacer &other);
    ~MFUPageReplacer();
};

#endif