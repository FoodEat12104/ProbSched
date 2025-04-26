#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "process.h"
#include "stats.h"

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
int gcd(int a, int b);
int lcm(int a, int b);

#endif