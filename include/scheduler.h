#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"

// Estrutura para estatísticas
typedef struct {
    float avg_waiting_time;
    float avg_turnaround_time;
    float cpu_utilization;
    float throughput;
    int deadline_misses;
} SchedulerStats;

// Declarações de funções para algoritmos básicos
void run_fcfs(Process *processes, int n);
void run_sjf_nonpreemptive(Process *processes, int n);
void run_sjf_preemptive(Process *processes, int n);

// Funções para Priority Scheduling
void run_priority_nonpreemptive(Process *processes, int n);
void run_priority_preemptive(Process *processes, int n);

// Round Robin
void run_rr(Process *processes, int n, int quantum);

// Algoritmos de tempo real
void run_rate_monotonic(Process *processes, int n);
void run_edf(Process *processes, int n);

// Funções auxiliares
SchedulerStats calculate_stats(Process *processes, int n, int total_time);
int gcd(int a, int b);
int lcm(int a, int b);
void print_stats(SchedulerStats stats);

#endif
