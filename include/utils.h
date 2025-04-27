#ifndef UTILS_H
#define UTILS_H

#include "process.h"

void print_initial_state(Process *processes, int n);

void print_non_preemptive(Process *processes, int n);
void print_priority_preemptive(Process *processes, int n);
void print_rr(Process *processes, int n, int quantum);
void print_rm(Process *processes, int n);
void print_edf(Process *processes, int n);

void print_final_results(Process *processes, int n);

#endif