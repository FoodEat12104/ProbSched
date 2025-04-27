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
    int deadline_misses;  // Adicionado para tempo real
} Process;

Process *generate_processes(int n, bool real_time);
void free_processes(Process *processes);
void reset_processes(Process *processes, int n);

#endif