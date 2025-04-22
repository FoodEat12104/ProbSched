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
        processes[i].burst_time = exponential_distribution(10) + 1; // Mínimo 1
        processes[i].priority = uniform_distribution(1, 10);
        processes[i].remaining_time = processes[i].burst_time;
        processes[i].completion_time = -1;  // -1 indica não completado
        processes[i].waiting_time = 0;
        
        if (real_time) {
            processes[i].deadline = processes[i].arrival_time + 
                                  uniform_distribution(processes[i].burst_time, 
                                                      processes[i].burst_time * 3);
            processes[i].period = (uniform_distribution(1, 3) == 1) ? 0 : 
                                uniform_distribution(processes[i].burst_time * 2, 
                                                    processes[i].burst_time * 4);
        } else {
            processes[i].deadline = 0;
            processes[i].period = 0;
        }
    }
    
    return processes;
}

void print_process(Process p) {
    printf("PID: %3d | Chegada: %3d | Burst: %3d | Prioridade: %2d | Restante: %3d",
           p.pid, p.arrival_time, p.burst_time, p.priority, p.remaining_time);
    
    if (p.deadline > 0 || p.period > 0) {
        printf(" | Deadline: %3d | Período: %3d", p.deadline, p.period);
    }
    
    if (p.completion_time != -1) {
        printf(" | Finalizado: %3d | Espera: %3d", 
               p.completion_time, p.waiting_time);
    }
    
    printf("\n");
}

void print_processes(Process *processes, int n) {
    printf("\n%-5s %-8s %-6s %-10s %-8s %-9s %-7s %-10s %-6s\n",
           "PID", "Chegada", "Burst", "Prioridade", "Restante", 
           "Deadline", "Período", "Finalizado", "Espera");
    printf("-----------------------------------------------------------");
    printf("-----------------------\n");
    
    for (int i = 0; i < n; i++) {
        printf("%-5d %-8d %-6d %-10d %-8d ",
               processes[i].pid, processes[i].arrival_time,
               processes[i].burst_time, processes[i].priority,
               processes[i].remaining_time);
        
        if (processes[i].deadline > 0 || processes[i].period > 0) {
            printf("%-9d %-7d ", processes[i].deadline, processes[i].period);
        } else {
            printf("%-9s %-7s ", "N/A", "N/A");
        }
        
        if (processes[i].completion_time != -1) {
            printf("%-10d %-6d\n", processes[i].completion_time, 
                   processes[i].waiting_time);
        } else {
            printf("%-10s %-6s\n", "N/A", "N/A");
        }
    }
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
