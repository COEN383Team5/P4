#ifndef PAGETABLE_H
#define PAGETABLE_H

#include <string>
#include <utility>
#include <mutex>
#include "MemoryReference.h"

#define NUM_PAGE_TABLE_ENTRIES 100

struct PageTableEntry {
    bool valid, dirty, referenced;
    double refTime, firstRefTime;
    int ownerId, ownerPage, numRefs;
};

struct FreePage {
    PageTableEntry *entry;
    FreePage *next;
};

class PageTable {
protected:
    std::mutex tableMut;
    // table and freePages share memory
    PageTableEntry *table;
    FreePage *freePages, *tailPtr;
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
     * @retval returns a pointer to a free page in table, or NULL if there are no free pages
     */
    PageTableEntry *getFreePage();

    /* sets the relavent variables for a page being referenced for the first time
     * @param page 
     *      a pointer to a page in table to be referenced
     * @param pageNum
     *      the virtual pageNumber, the owner's page number
     * @param id
     *      the id of the requesting process
     */
    void setPage(PageTableEntry *page, const int &pageNum, const int &id);

    /* adds the page to the end of freePages
     * @param page
     *      a pointer to the page to add to the end of freePages
     */
    void addToTail(PageTableEntry *page);

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
     * This function has a critical section which may alter pageTable
     * @param pageNum
     *      the virtual page number that the processes is requesting for
     * @param id
     *      the id of the process
     * @retval a pair containing a bool that represents if the page was already in memory
     *      and a MemoryReference object that has all the information about the reference
     */
    std::pair<bool, MemoryReference> reference(const int &pageNum, const int &idp);

    /* invalidates all pages belonging to doneProcess and adds them to the free list
     * @param doneProcess
     *      a pointer to the process who is done executing and will have its pages free
     */
    void swapOff(const int &pid); 
};

#endif
