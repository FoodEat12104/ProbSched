#include "stats.h"
#include "process.h"
#include <math.h>
#include <stdio.h>

SchedulerStats calculate_stats(Process *processes, int n, int total_time) {
    SchedulerStats stats = {0};
    
    if (total_time <= 0) {
        return stats;  // Evita divisão por zero
    }

    // Throughput
    int completed = 0;
    for (int i = 0; i < n; i++) {
        if (processes[i].completion_time > 0) completed++;
    }
    stats.throughput = (float)completed / total_time;

    // CPU Utilization
    int busy_time = 0;
    for (int i = 0; i < n; i++) {
        busy_time += processes[i].burst_time;
    }
    stats.cpu_utilization = (float)busy_time / total_time * 100;

    // Waiting/Turnaround Time
    float total_waiting = 0, total_turnaround = 0;
    int deadline_misses = 0;
    int valid_processes = 0;

    for (int i = 0; i < n; i++) {
        if (processes[i].completion_time > 0) {
            int turnaround = processes[i].completion_time - processes[i].arrival_time;
            total_turnaround += turnaround;
            total_waiting += processes[i].waiting_time;
            deadline_misses += processes[i].deadline_misses;
            valid_processes++;
        }
    }

    if (valid_processes > 0) {
        stats.avg_waiting_time = total_waiting / valid_processes;
        stats.avg_turnaround_time = total_turnaround / valid_processes;
    }
    stats.deadline_misses = deadline_misses;

    return stats;
}

void print_stats(SchedulerStats stats) {
    printf("\n=== Estatísticas da Simulação ===\n");
    printf("- Tempo médio de espera: %.2f\n", stats.avg_waiting_time);
    printf("- Tempo médio de turnaround: %.2f\n", stats.avg_turnaround_time);
    printf("- Utilização da CPU: %.2f%%\n", stats.cpu_utilization);
    printf("- Throughput: %.2f processos/unidade de tempo\n", stats.throughput);
    printf("- Deadlines perdidos: %d\n", stats.deadline_misses);
}