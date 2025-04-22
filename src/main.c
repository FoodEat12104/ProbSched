#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "process.h"
#include "scheduler.h" 
#include "distributions.h"

void print_usage(const char *program_name) {
    printf("Uso: %s <algoritmo> <num_processos> [quantum]\n", program_name);
    printf("Algoritmos disponíveis:\n");
    printf("  FCFS          - First-Come, First-Served\n");
    printf("  SJF_NP        - Shortest Job First (não preemptivo)\n");
    printf("  SJF_P         - Shortest Remaining Time First (preemptivo)\n");
    printf("  PRIORITY_NP   - Priority Scheduling (não preemptivo)\n");
    printf("  PRIORITY_P    - Priority Scheduling (preemptivo)\n");
    printf("  RR            - Round Robin (requer quantum)\n");
    printf("  RM            - Rate Monotonic (para processos periódicos)\n");
    printf("  EDF           - Earliest Deadline First\n");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        print_usage(argv[0]);
        return 1;
    }

    const char *algorithm = argv[1];
    int num_processes = atoi(argv[2]);
    int quantum = (argc > 3) ? atoi(argv[3]) : 0;
    
    if (num_processes <= 0) {
        printf("Número de processos deve ser positivo\n");
        return 1;
    }

    // Determina se é algoritmo de tempo real
    bool is_real_time = (strcmp(algorithm, "RM") == 0 || strcmp(algorithm, "EDF") == 0);
    Process *processes = generate_processes(num_processes, is_real_time);
    
    printf("\n=== Simulador de Escalonamento de Processos ===\n");
    printf("Algoritmo: %s\n", algorithm);
    printf("Número de processos: %d\n", num_processes);
    if (strcmp(algorithm, "RR") == 0) {
        printf("Quantum: %d\n", quantum);
    }
    
    print_processes(processes, num_processes);

    // Executa o algoritmo selecionado
    if (strcmp(algorithm, "FCFS") == 0) {
        run_fcfs(processes, num_processes);
    } 
    else if (strcmp(algorithm, "SJF_NP") == 0) {
        run_sjf_nonpreemptive(processes, num_processes);
    }
    else if (strcmp(algorithm, "SJF_P") == 0) {
        run_sjf_preemptive(processes, num_processes);
    }
    else if (strcmp(algorithm, "PRIORITY_NP") == 0) {
        run_priority_nonpreemptive(processes, num_processes);
    }
    else if (strcmp(algorithm, "PRIORITY_P") == 0) {
        run_priority_preemptive(processes, num_processes);
    }
    else if (strcmp(algorithm, "RR") == 0) {
        if (quantum <= 0) {
            printf("Erro: RR requer um quantum positivo\n");
            free_processes(processes);
            return 1;
        }
        run_rr(processes, num_processes, quantum);
    }
    else if (strcmp(algorithm, "RM") == 0) {
        run_rate_monotonic(processes, num_processes);
    }
    else if (strcmp(algorithm, "EDF") == 0) {
        run_edf(processes, num_processes);
    }
    else {
        printf("Erro: Algoritmo desconhecido\n");
        print_usage(argv[0]);
        free_processes(processes);
        return 1;
    }

    free_processes(processes);
    return 0;
}
