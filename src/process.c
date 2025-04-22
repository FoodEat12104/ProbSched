#include "process.h"
#include "distributions.h"
#include <stdlib.h>
#include <stdio.h>

Process *generate_processes(int n) {
    Process *processes = malloc(n * sizeof(Process));
    
    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        processes[i].arrival_time = poisson_distribution(5);
        processes[i].burst_time = exponential_distribution(10);
        processes[i].priority = uniform_distribution(1, 10);
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].deadline = processes[i].arrival_time + processes[i].burst_time * 2;
        processes[i].period = processes[i].burst_time * 3;
    }
    
    return processes;
}

void print_process(Process p) {
    printf("PID: %d | Chegada: %d | Burst: %d | Prioridade: %d | Deadline: %d\n",
           p.pid, p.arrival_time, p.burst_time, p.priority, p.deadline);
}

void print_processes(Process *processes, int n) {
    for (int i = 0; i < n; i++) {
        print_process(processes[i]);
    }
}
