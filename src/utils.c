#include "utils.h"
#include "process.h"
#include <stdio.h>
#include <stdlib.h>

void print_initial_state(Process *processes, int n) {
    printf("\n=== Simulador de Escalonamento de Processos ===\n");
    printf("%-5s %-8s %-6s %-10s %-7s\n",
           "PID", "Chegada", "Burst", "Prioridade", "Período");
    printf("-----------------------------------------\n");
    
    for (int i = 0; i < n; i++) {
        printf("%-5d %-8d %-6d %-10d ",
               processes[i].pid, 
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].priority);
        
        if (processes[i].period > 0) {
            printf("%-7d\n", processes[i].period);
        } else {
            printf("%-7s\n", "N/A");
        }
    }
}

void print_timeline(Process *processes, int n) {
    printf("\nEXECUÇÃO: ");
    
    // Encontra o tempo total de execução
    int total_time = 0;
    for (int i = 0; i < n; i++) {
        if (processes[i].completion_time > total_time) {
            total_time = processes[i].completion_time;
        }
    }

    // Cria uma representação precisa da execução
    int *execution = malloc(total_time * sizeof(int));
    int *remaining = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        remaining[i] = processes[i].burst_time;
    }

    // Simula a execução passo a passo
    int current_pid = 0;
    int time_used = 0;
    int quantum = 2; // Para RR com quantum=2
    
    for (int t = 0; t <= total_time; t++) {
        // Verifica se o processo atual terminou
        if (current_pid > 0 && (t == total_time || execution[t-1] != current_pid)) {
            int finished = 1;
            for (int i = 0; i < n; i++) {
                if (processes[i].pid == current_pid && remaining[i] > 0) {
                    finished = 0;
                    break;
                }
            }
            if (finished) {
                printf("✓");
            }
        }

        // Seleciona próximo processo para executar
        if (t < total_time) {
            execution[t] = 0; // Inicializa como ocioso
            
            // Encontra o próximo processo pronto para executar
            for (int i = 0; i < n; i++) {
                if (processes[i].arrival_time <= t && remaining[i] > 0) {
                    if (current_pid == 0 || time_used >= quantum || remaining[current_pid-1] == 0) {
                        // Troca de processo
                        if (current_pid != 0 && remaining[current_pid-1] > 0) {
                            printf("|");
                        }
                        current_pid = processes[i].pid;
                        time_used = 0;
                    }
                    break;
                }
            }

            // Executa o processo atual
            if (current_pid > 0) {
                for (int i = 0; i < n; i++) {
                    if (processes[i].pid == current_pid && remaining[i] > 0) {
                        execution[t] = current_pid;
                        remaining[i]--;
                        time_used++;
                        break;
                    }
                }
            }

            // Imprime o estado atual
            if (t == 0 || execution[t] != execution[t-1]) {
                if (execution[t] == 0) {
                    printf("##");
                } else {
                    printf("P%d", execution[t]);
                }
            }
            if (execution[t] > 0) {
                printf("▉");
            }
        }
    }

    printf("\n");
    free(execution);
    free(remaining);
}

void print_final_results(Process *processes, int n) {
    printf("\n=== Resultado Final ===\n");
    printf("%-5s %-8s %-6s %-10s %-7s %-10s %-6s\n",
           "PID", "Chegada", "Burst", "Prioridade", "Período", 
           "Finalizado", "Espera");
    printf("----------------------------------------------------------\n");
    
    for (int i = 0; i < n; i++) {
        printf("%-5d %-8d %-6d %-10d ",
               processes[i].pid, 
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].priority);

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
