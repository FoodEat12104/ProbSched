#include "utils.h"
#include "process.h"
#include <stdio.h>
#include <stdlib.h>

void print_initial_state(Process *processes, int n) {
    printf("\n=== Simulador de Escalonamento de Processos ===\n");
    printf("%-5s %-8s %-6s %-10s %-9s %-7s\n",
           "PID", "Chegada", "Burst", "Prioridade", "Deadline", "Período");
    printf("----------------------------------------------------\n");
    
    for (int i = 0; i < n; i++) {
        printf("%-5d %-8d %-6d %-10d %-9d ",
               processes[i].pid, 
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].priority,
               processes[i].deadline);
        
        if (processes[i].period > 0) {
            printf("%-7d\n", processes[i].period);
        } else {
            printf("%-7s\n", "N/A");
        }
    }
}

void print_timeline(Process *processes, int n) {
    printf("\nEXECUÇÃO: ");
    
    for (int i = 0; i < n; i++) {
        printf("P%d", processes[i].pid);
        for (int j = 0; j < processes[i].burst_time; j++) {
            printf("▉");
        }
        
        if (processes[i].completion_time > processes[i].deadline && processes[i].deadline > 0) {
            printf("!");
        } else if (processes[i].completion_time > 0) {
            printf("✓");
        }
        
        if (i < n-1) printf("|");
    }
    printf("\n");
}

void print_final_results(Process *processes, int n) {
    printf("\n=== Resultado Final ===\n");
    printf("%-5s %-8s %-6s %-10s %-9s %-7s %-10s %-6s\n",
           "PID", "Chegada", "Burst", "Prioridade", "Deadline", "Período", 
           "Finalizado", "Espera");
    printf("-----------------------------------------------------------");
    printf("-----------------------\n");
    
    for (int i = 0; i < n; i++) {
        printf("%-5d %-8d %-6d %-10d %-9d ",
               processes[i].pid, 
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].priority,
               processes[i].deadline);

        // Período
        if (processes[i].period > 0) {
            printf("%-7d ", processes[i].period);
        } else {
            printf("%-7s ", "N/A");
        }

        // Finalizado e Espera
        if (processes[i].completion_time >= 0) {
            printf("%-10d %-6d\n", 
                   processes[i].completion_time,
                   processes[i].waiting_time);
        } else {
            printf("%-10s %-6s\n", "N/A", "N/A");
        }
    }
}
