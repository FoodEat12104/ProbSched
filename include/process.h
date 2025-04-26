#ifndef PROCESS_H
#define PROCESS_H

#include <stdbool.h>

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int priority;
    int deadline;
    int period;
    int completion_time;  
    int waiting_time;
    int deadline_misses;  // Added for real-time scheduling statistics
} Process;

Process *generate_processes(int n, bool real_time);
void print_process(Process p);
void print_processes(Process *processes, int n);
void free_processes(Process *processes);
void reset_processes(Process *processes, int n);

#endif