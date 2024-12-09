#ifndef UTIL_H
#define UTIL_H

#include "process.h"

/**
 * Utility function file
 */

ProcessType *parse_file(FILE *, int *);
void findWaitingTimeFCFS(ProcessType plist[], int n);
void findWaitingTimeSJF(ProcessType plist[], int n);
int my_comparer(const void *this, const void *that);
void findavgTimePriority(ProcessType plist[], int n);
void findWaitingTime(ProcessType plist[], int n);
void findTurnAroundTime(ProcessType plist[], int n);
void findavgTimeFCFS(ProcessType plist[], int n); 
void findavgTimeSJF(ProcessType plist[], int n);
void findavgTimeRR(ProcessType plist[], int n, int quantum); 
void findavgTimePriority(ProcessType plist[], int n);
void printMetrics(ProcessType plist[], int n);
ProcessType* initProc(char *filename, int *n);

#endif				// UTIL_H
