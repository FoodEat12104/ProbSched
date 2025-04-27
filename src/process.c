#include "process.h"
#include "distributions.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

Process *generate_processes(int n, bool real_time) {
    Process *processes = malloc(n * sizeof(Process));
    if (!processes) {
        perror("Erro ao alocar memória para processos");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        processes[i].arrival_time = poisson_distribution(5);
        processes[i].burst_time = normal_distribution(1, 10);
        processes[i].priority = uniform_distribution(1, 10);
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].completion_time = -1;  // -1 indica não completado
        processes[i].waiting_time = -1;     // -1 indica não calculado
        processes[i].deadline_misses = 0;
        
        if (real_time) {
            processes[i].period = normal_distribution(1, 10);
            processes[i].deadline = processes[i].arrival_time + processes[i].period;
        } else {
            processes[i].deadline = 0;
            processes[i].period = 0;
        }
    }
    
    return processes;
}

void free_processes(Process *processes) {
    free(processes);
}

void reset_processes(Process *processes, int n) {
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].completion_time = -1;
        processes[i].waiting_time = 0;
    }
}
