#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "process.h"
#include "scheduler.h" 
#include "distributions.h"
#include "utils.h"

void print_usage(const char *program_name) {
    printf("Uso: %s <algoritmo> <num_processos> [quantum]\n", program_name);
    printf("Algoritmos disponíveis:\n");
    printf("  FCFS          - First-Come, First-Served\n");
    printf("  SJF           - Shortest Job First\n");
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
        printf("Número de processos deve ser positivo!\n");
        return 1;
    }

    bool is_real_time = (strcmp(algorithm, "RM") == 0 || strcmp(algorithm, "EDF") == 0);
    Process *processes = generate_processes(num_processes, is_real_time);
    
    print_initial_state(processes, num_processes);

    // Executa o algoritmo selecionado
    if (strcmp(algorithm, "FCFS") == 0) {
        printf("\n=== Executando FCFS (First-Come, First-Served) ===\n");
        run_fcfs(processes, num_processes);
        print_non_preemptive(processes, num_processes);
    } 
    else if (strcmp(algorithm, "SJF") == 0) {
        printf("\n=== Executando SJF (Shortest Job First) ===\n");
        run_sjf(processes, num_processes);
        print_non_preemptive(processes, num_processes);
    }
    else if (strcmp(algorithm, "PRIORITY_NP") == 0) {
        printf("\n=== Executando Priority Scheduling não preemptivo ===\n");
        run_priority_nonpreemptive(processes, num_processes);
        print_non_preemptive(processes, num_processes);
    }
    else if (strcmp(algorithm, "PRIORITY_P") == 0) {
        printf("\n=== Executando Priority Scheduling preemptivo ===\n");
        print_priority_preemptive(processes, num_processes);
    }
    else if (strcmp(algorithm, "RR") == 0) {
        if (quantum <= 0) {
            printf("Erro: RR requer um quantum positivo\n");
            free_processes(processes);
            return 1;
        }
        printf("\n=== Executando Round Robin (Quantum=%d) ===\n", quantum);
        run_rr(processes, num_processes, quantum);
        print_rr(processes, num_processes, quantum);
    }
    else if (strcmp(algorithm, "RM") == 0) {
        printf("\n=== Executando Rate Monotonic Scheduling ===\n");
        run_rate_monotonic(processes, num_processes);
        print_rm(processes, num_processes);
    }
    else if (strcmp(algorithm, "EDF") == 0) {
        printf("\n=== Executando Earliest Deadline First Scheduling ===\n");
        run_edf(processes, num_processes);
        print_edf(processes, num_processes);
    }
    else {
        printf("Erro: Algoritmo desconhecido!\n");
        print_usage(argv[0]);
        free_processes(processes);
        return 1;
    }

    // Calcula tempo total de execução
    int total_time = 0;
    for (int i = 0; i < num_processes; i++) {
        if (processes[i].completion_time > total_time) {
            total_time = processes[i].completion_time;
        }
    }

    // Mostra resultados
    print_final_results(processes, num_processes);
    
    SchedulerStats stats = calculate_stats(processes, num_processes, total_time);
    print_stats(stats);
    
    free_processes(processes);
    return 0;
}