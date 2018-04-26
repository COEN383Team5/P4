#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <string>
#include <utility>
#include "MemoryReference.h"

#define NUM_PAGE_TABLE_ENTRIES 100

struct PageTableEntry {
    bool valid, dirty, referenced;
    double refTime;
    int ownerId, ownerPage, numRefs;
};

class PageTable {
protected:
    PageTableEntry *table;
    int numFree;
    
    /* determines if the page requested by the process is in the table
     * @param pageNum
     *      the virtual page number that the processes is requesting for
     * @param id
     *      the id of the process
     * @retval the index that has the page, or -1 if it is not in the table
     */
    int isInTable(const int &pageNum, const int &id) const;

    /* finds a free page in the page table
     * @retval returns the index to a free page in table, or -1 if there are no free pages
     */
    int getFreePage() const;

    /* sets the relavent variables for a page being referenced for the first time
     * @param page 
     *      the index in table for the page being referenced
     * @param pageNum
     *      the virtual pageNumber, the owner's page number
     * @param id
     *      the id of the requesting process
     */
    void setPage(const int &page, const int &pageNum, const int &id);

    /* This function needs to be implemented in classes that inherit this one
     * and needs to preform the tasks of an algorithm (FIFO, LRU, LFU, MFU, random)
     * @param pageNum
     *      the virtual page number that the processes is requesting for
     * @param id
     *      the id of the process
     * @retval a pair containing a bool that represents if the page was already in memory
     *      that bool will always be false because this function only occurs when there is a page fault
     *      the second part of the pair is a MemoryReference object that has all the information about the reference
     */
    virtual std::pair<bool, MemoryReference> algImpl(const int &pageNum, const int &id) = 0;

public:
    PageTable();
    PageTable(const PageTable &other);
    PageTable &operator=(const PageTable &other);
    ~PageTable();
    std::string getMemoryMap() const;
    int getNumFree() const;

    /* handles a reference to a page by a process
     * a pointer to this function should be passed to the constructor of Process
     * @param pageNum
     *      the virtual page number that the processes is requesting for
     * @param id
     *      the id of the process
     * @retval a pair containing a bool that represents if the page was already in memory
     *      and a MemoryReference object that has all the information about the reference
     */
    std::pair<bool, MemoryReference> reference(const int &pageNum, const int &id);
};

#endif
