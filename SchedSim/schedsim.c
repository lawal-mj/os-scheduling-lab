// schedsim.c
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>

// Define the Process structure
typedef struct Process {
    int process_id;   // Process Identifier
    int burst_time;   // Burst Time
    int arrival_time; // Arrival Time
    int wait_time;    // Waiting Time
    int turnaround;   // Turnaround Time
    int priority;     // Priority
} Process;

// Function prototypes
Process* parse_file(FILE *file_ptr, int *count);

// Function to calculate waiting time for FCFS and Priority Scheduling
void findWaitingTime(Process processes[], int count)
{
    processes[0].wait_time = processes[0].arrival_time;

    for (int idx = 1; idx < count; idx++) {
        processes[idx].wait_time = processes[idx - 1].burst_time + processes[idx - 1].wait_time;
    }
}

// Function to calculate waiting time for FCFS Scheduling
void findWaitingTimeFCFS(Process processes[], int count)
{
    processes[0].wait_time = processes[0].arrival_time;

    for (int idx = 1; idx < count; idx++) {
        processes[idx].wait_time = processes[idx - 1].burst_time + processes[idx - 1].wait_time;
    }
}

// Function to compute turnaround time
void findTurnAroundTime(Process processes[], int count)
{
    for (int idx = 0; idx < count; idx++) {
        processes[idx].turnaround = processes[idx].burst_time + processes[idx].wait_time;
    }
}

// Function to calculate waiting time using Shortest Remaining Time First (SRTF)
void findWaitingTimeSJF(Process processes[], int count)
{
    int remaining_bt[count];
    for (int idx = 0; idx < count; idx++)
        remaining_bt[idx] = processes[idx].burst_time;

    int completed = 0, current_time = 0, finish_time;
    int shortest = -1, min_burst = INT_MAX;
    int flag = 0;

    // Initialize all waiting times to zero
    for (int idx = 0; idx < count; idx++)
        processes[idx].wait_time = 0;

    while (completed != count) {
        min_burst = INT_MAX;
        shortest = -1;
        flag = 0;

        for (int idx = 0; idx < count; idx++) {
            if (processes[idx].arrival_time <= current_time && remaining_bt[idx] > 0 && remaining_bt[idx] < min_burst) {
                min_burst = remaining_bt[idx];
                shortest = idx;
                flag = 1;
            }
        }

        if (!flag) {
            current_time++;
            continue;
        }

        remaining_bt[shortest]--;

        if (remaining_bt[shortest] == 0) {
            completed++;
            finish_time = current_time + 1;
            processes[shortest].wait_time = finish_time - processes[shortest].burst_time - processes[shortest].arrival_time;
            if (processes[shortest].wait_time < 0)
                processes[shortest].wait_time = 0;
        }
        current_time++;
    }
}

// Comparator for Priority Scheduling
int priorityComparator(const void *a, const void *b)
{
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;

    if (p1->priority > p2->priority)
        return -1;
    else if (p1->priority < p2->priority)
        return 1;
    else {
        if (p1->process_id < p2->process_id)
            return -1;
        else if (p1->process_id > p2->process_id)
            return 1;
        else
            return 0;
    }
}

// Function to calculate average time for Priority Scheduling
void findavgTimePriority(Process processes[], int count)
{
    qsort(processes, count, sizeof(Process), priorityComparator);

    findWaitingTime(processes, count);
    findTurnAroundTime(processes, count);

    printf("\n*********\nPriority\n");
}

// Function to calculate waiting time using Round Robin Scheduling
void findWaitingTimeRR(Process processes[], int count, int quantum)
{
    int remaining_bt[count];
    for (int idx = 0; idx < count; idx++)
        remaining_bt[idx] = processes[idx].burst_time;

    int current_time = 0;
    int done = 0;

    // Initialize all waiting times to zero
    for (int idx = 0; idx < count; idx++)
        processes[idx].wait_time = 0;

    while (1) {
        done = 1;

        for (int idx = 0; idx < count; idx++) {
            if (remaining_bt[idx] > 0) {
                done = 0;

                if (remaining_bt[idx] > quantum) {
                    current_time += quantum;
                    remaining_bt[idx] -= quantum;

                    for (int j = 0; j < count; j++) {
                        if (j != idx && remaining_bt[j] > 0)
                            processes[j].wait_time += quantum;
                    }
                }
                else {
                    current_time += remaining_bt[idx];
                    processes[idx].wait_time = current_time - processes[idx].burst_time;
                    remaining_bt[idx] = 0;
                }
            }
        }

        if (done)
            break;
    }
}

// Function to calculate average time for FCFS Scheduling
void findavgTimeFCFS(Process processes[], int count)
{
    findWaitingTimeFCFS(processes, count);
    findTurnAroundTime(processes, count);

    printf("\n*********\nFCFS\n");
}

// Function to calculate average time for SJF Scheduling
void findavgTimeSJF(Process processes[], int count)
{
    findWaitingTimeSJF(processes, count);
    findTurnAroundTime(processes, count);

    printf("\n*********\nSJF\n");
}

// Function to calculate average time for Round Robin Scheduling
void findavgTimeRR(Process processes[], int count, int quantum)
{
    findWaitingTimeRR(processes, count, quantum);
    findTurnAroundTime(processes, count);

    printf("\n*********\nRR Quantum = %d)\n", quantum);
}

// Function to display metrics
void printMetrics(Process processes[], int count)
{
    int total_wait = 0, total_turn = 0;

    printf("\tProcesses\tBurst time\tWaiting Time\tTurn around time\n");

    for (int idx = 0; idx < count; idx++) {
        total_wait += processes[idx].wait_time;
        total_turn += processes[idx].turnaround;
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", processes[idx].process_id, processes[idx].burst_time, processes[idx].wait_time, processes[idx].turnaround);
    }

    float avg_wait = (float)total_wait / count;
    float avg_turn = (float)total_turn / count;

    printf("\nAverage waiting time = %.2f", avg_wait);
    printf("\nAverage turnaround time = %.2f\n", avg_turn);
}

// Function to initialize process list from file
Process* initProcList(char *file_path, int *count)
{
    FILE *file = fopen(file_path, "r");
    if (!file) {
        fprintf(stderr, "Error: Unable to open file %s\n", file_path);
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    Process *processes = parse_file(file, count);
    fclose(file);
    return processes;
}

// Main function
int main(int argc, char *argv[])
{
    if (argc < 2) {
        fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
        fflush(stdout);
        return 1;
    }

    int num_processes;
    int time_quantum = 2;
    Process *proc_list;

    // FCFS Scheduling
    proc_list = initProcList(argv[1], &num_processes);
    findavgTimeFCFS(proc_list, num_processes);
    printMetrics(proc_list, num_processes);
    free(proc_list);

    // SJF Scheduling (Implemented as SRTF)
    proc_list = initProcList(argv[1], &num_processes);
    findavgTimeSJF(proc_list, num_processes);
    printMetrics(proc_list, num_processes);
    free(proc_list);

    // Priority Scheduling
    proc_list = initProcList(argv[1], &num_processes);
    findavgTimePriority(proc_list, num_processes);
    printMetrics(proc_list, num_processes);
    free(proc_list);

    // Round Robin Scheduling
    proc_list = initProcList(argv[1], &num_processes);
    findavgTimeRR(proc_list, num_processes, time_quantum);
    printMetrics(proc_list, num_processes);
    free(proc_list);

    return 0;
}