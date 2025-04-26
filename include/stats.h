#ifndef STATS_H
#define STATS_H

#include "process.h"

typedef struct {
    float avg_waiting_time;
    float avg_turnaround_time;
    float cpu_utilization;
    float throughput;
    int deadline_misses;
} SchedulerStats;

SchedulerStats calculate_stats(Process *processes, int n, int total_time);
void print_stats(SchedulerStats stats);

#endif