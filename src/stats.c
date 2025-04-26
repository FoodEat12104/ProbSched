#include "stats.h"
#include "process.h"
#include <math.h>
#include <stdio.h>

SchedulerStats calculate_stats(Process *processes, int n, int total_time) {
    SchedulerStats stats = {0};
    
    // Calculate throughput
    int completed = 0;
    for (int i = 0; i < n; i++) {
        if (processes[i].completion_time > 0) {
            completed++;
        }
    }
    stats.throughput = (float)completed / total_time;
    
    // Calculate CPU utilization
    int busy_time = 0;
    for (int i = 0; i < n; i++) {
        busy_time += processes[i].burst_time;
    }
    stats.cpu_utilization = (float)busy_time / total_time * 100;
    
    // Calculate average waiting and turnaround times
    float total_waiting = 0;
    float total_turnaround = 0;
    int deadline_misses = 0;
    int count = 0;
    
    for (int i = 0; i < n; i++) {
        if (processes[i].completion_time > 0) {
            int turnaround = processes[i].completion_time - processes[i].arrival_time;
            int waiting = processes[i].waiting_time;
            
            total_waiting += waiting;
            total_turnaround += turnaround;
            count++;
            
            if (processes[i].deadline > 0 && processes[i].completion_time > processes[i].deadline) {
                deadline_misses++;
            }
        }
    }
    
    if (count > 0) {
        stats.avg_waiting_time = total_waiting / count;
        stats.avg_turnaround_time = total_turnaround / count;
    }
    
    stats.deadline_misses = deadline_misses;
    
    return stats;
}

void print_stats(SchedulerStats stats) {
    printf("\n=== Estatísticas da Simulação ===\n");
    printf("Tempo médio de espera: %.2f\n", stats.avg_waiting_time);
    printf("Tempo médio de turnaround: %.2f\n", stats.avg_turnaround_time);
    printf("Utilização da CPU: %.2f%%\n", stats.cpu_utilization);
    printf("Throughput: %.2f processos/unidade de tempo\n", stats.throughput);
    printf("Deadlines perdidos: %d\n", stats.deadline_misses);
}