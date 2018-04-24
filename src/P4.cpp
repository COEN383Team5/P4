#include <iostream>
#include <vector>
#include "Process.h"

int main(int argc, char *argv[]) {
    std::vector<Process> procs = generateProcesses();
    for(unsigned int i = 0; i < procs.size(); i++) {
        std::cout << procs[i] << std::endl;
    }
    return 0;
}
