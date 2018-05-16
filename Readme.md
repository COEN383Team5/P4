# COEN383 P4

This repo has code to simulate different page replacement algorithms algorithms. It was made to satisfy an assignment for COEN383: Advanced Operating Systems at Santa Clara University. 

To compile on a unix system:

    make

Usage:

        P4 [algType | numRuns]
Where:

        algType is the name of an algorith to run. Possible options are: FIFO, LRU, LFU, MFU, RAND

        numRuns is the number of times you want to run each algorithm in a row. In this case it will run all of the algoritms

Only one of the arguments is required.

Most complicated headers have comments for their functions. The assignment description is included in Project-4.pdf.


## Results 
The average hit/miss ratios (per run and overall) are included in the file report.txt  
The number of completed processes per run can be found in the full output (warning, large!), which is included as fulloutput.txt