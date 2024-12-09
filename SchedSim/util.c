#define _GNU_SOURCE
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<errno.h>

#include "util.h"
#include "process.h"

/**
 * Returns an array of process that are parsed from
 * the input file descriptor passed as argument
 * CAUTION: You need to free up the space that is allocated
 * by this function
 */
// Function to parse the input file and initialize the process list
ProcessType* parse_file(FILE *fp, int *n) {
    // First, count the number of lines (processes)
    int count = 0;
    int pid, bt, art, x1, x2, pri;
    while (fscanf(fp, "%d %d %d %d %d %d", &pid, &bt, &art, &x1, &x2, &pri) == 6) {
        count++;
    }

    // Allocate memory for processes
    ProcessType *plist = (ProcessType*) malloc(count * sizeof(ProcessType));
    if (!plist) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    // Reset file pointer to beginning
    rewind(fp);

    // Read process data
    int i = 0;
    while (fscanf(fp, "%d %d %d %d %d %d", &pid, &bt, &art, &x1, &x2, &pri) == 6) {
        plist[i].pid = pid;
        plist[i].bt = bt;
        plist[i].art = art;
        plist[i].pri = pri;
        plist[i].wt = 0;  // Initialize waiting time
        plist[i].tat = 0; // Initialize turnaround time
        i++;
    }

    *n = count;
    return plist;
}

