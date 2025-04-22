#include "scheduler.h"
#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Implementação do FCFS
void run_fcfs(Process *processes, int n) {
    printf("\nExecutando FCFS (First-Come, First-Served)\n");
    
    int current_time = 0;
    float total_waiting = 0;
    float total_turnaround = 0;
    
    for (int i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        
        int waiting_time = current_time - processes[i].arrival_time;
        int turnaround_time = waiting_time + processes[i].burst_time;
        
        total_waiting += waiting_time;
        total_turnaround += turnaround_time;
        
        printf("Processo %d: Espera=%d, Turnaround=%d\n",
               processes[i].pid, waiting_time, turnaround_time);
        
        current_time += processes[i].burst_time;
    }
    
    printf("\nMédias: Espera=%.2f, Turnaround=%.2f\n",
           total_waiting/n, total_turnaround/n);
}

// Implementação do SJF
void run_sjf(Process *processes, int n) {
    printf("\nExecutando SJF (Shortest Job First)\n");
    
    // Ordenação por burst time
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            if (processes[j].burst_time < processes[i].burst_time) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
    
    run_fcfs(processes, n);
}

// Implementação do Priority
void run_priority(Process *processes, int n) {
    printf("\nExecutando Priority Scheduling\n");
    
    // Ordenação por prioridade (menor número = maior prioridade)
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            if (processes[j].priority < processes[i].priority) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
    
    run_fcfs(processes, n);
}

// Implementação do Round Robin
void run_rr(Process *processes, int n, int quantum) {
    printf("\nExecutando Round Robin (Quantum=%d)\n", quantum);
    
    int *remaining_time = malloc(n * sizeof(int));
    int current_time = 0;
    float total_waiting = 0;
    float total_turnaround = 0;
    
    for (int i = 0; i < n; i++) {
        remaining_time[i] = processes[i].burst_time;
    }
    
    int completed = 0;
    while (completed < n) {
        for (int i = 0; i < n; i++) {
            if (remaining_time[i] > 0) {
                int exec_time = (remaining_time[i] > quantum) ? quantum : remaining_time[i];
                current_time += exec_time;
                remaining_time[i] -= exec_time;
                
                if (remaining_time[i] == 0) {
                    completed++;
                    int turnaround = current_time - processes[i].arrival_time;
                    int waiting = turnaround - processes[i].burst_time;
                    total_waiting += waiting;
                    total_turnaround += turnaround;
                }
            }
        }
    }
    
    printf("\nMédias: Espera=%.2f, Turnaround=%.2f\n",
           total_waiting/n, total_turnaround/n);
    free(remaining_time);
}

// Implementação do EDF
void run_edf(Process *processes, int n) {
    printf("\nExecutando EDF (Earliest Deadline First)\n");
    
    // Ordenação por deadline
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            if (processes[j].deadline < processes[i].deadline) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
    
    run_fcfs(processes, n);
}

// Implementação de calculate_stats
SchedulerStats calculate_stats(Process *processes, int n, int total_time) {
    SchedulerStats stats = {0};
    // Implementação das estatísticas
    return stats;
}
