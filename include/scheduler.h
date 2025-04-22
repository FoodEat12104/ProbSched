#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

// Estrutura para estatísticas
typedef struct {
    float avg_waiting;
    float avg_turnaround;
    float cpu_utilization;
    float throughput;
} SchedulerStats;

// Declarações explícitas de todas as funções
void run_fcfs(Process *processes, int n);
void run_sjf(Process *processes, int n);
void run_priority(Process *processes, int n);
void run_rr(Process *processes, int n, int quantum);
void run_edf(Process *processes, int n);
SchedulerStats calculate_stats(Process *processes, int n, int total_time);

#endif
