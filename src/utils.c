#include "utils.h"
#include "process.h"
#include "scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>
#include <string.h>

static int compare_arrival(const void *a, const void *b) {
    const Process *p1 = (const Process *)a;
    const Process *p2 = (const Process *)b;
    return p1->arrival_time - p2->arrival_time;
}

int compare_period(const void *a, const void *b) {
    Process *p1 = (Process *)a;
    Process *p2 = (Process *)b;
    return p1->period - p2->period;
}

void print_initial_state(Process *processes, int n) {
    printf("\n=== Simulador de Escalonamento de Processos ===\n\n");
    printf("%-5s %-8s %-6s %-10s %-7s %-9s\n",
           "PID", "Chegada", "Burst", "Prioridade", "Período", "Deadline");
    printf("-------------------------------------------------\n");
    
    for (int i = 0; i < n; i++) {
        printf("%-5d %-8d %-6d %-10d %-9d %-7d\n",
               processes[i].pid, 
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].priority,
               processes[i].deadline,
               processes[i].period);
    }
}

void print_non_preemptive(Process *processes, int n) {
    printf("\nEXECUÇÃO: ");
    int current_time = 0;
    
    Process *sorted = malloc(n * sizeof(Process));
    memcpy(sorted, processes, n * sizeof(Process));
    qsort(sorted, n, sizeof(Process), compare_arrival);

    for (int i = 0; i < n; i++) {
        if (current_time < sorted[i].arrival_time) {
            for (int t = current_time; t < sorted[i].arrival_time; t++) {
                printf("#");
            }
            current_time = sorted[i].arrival_time;
        }
        
        printf("P%d", sorted[i].pid);
        for (int j = 0; j < sorted[i].burst_time; j++) {
            printf("▉");
        }
        
        if (sorted[i].deadline > 0 && current_time + sorted[i].burst_time > sorted[i].deadline) {
            printf("!");
        } else {
            printf("✓");
        }
        
        current_time += sorted[i].burst_time;
    }
    printf("\n");
    free(sorted);
}

void print_priority_preemptive(Process *processes, int n) {
    printf("\nEXECUÇÃO: ");
    
    // Criar cópias para não alterar os dados originais durante a impressão
    int *remaining = malloc(n * sizeof(int));
    int *completion_times = malloc(n * sizeof(int));
    bool *completed = malloc(n * sizeof(bool));
    
    for (int i = 0; i < n; i++) {
        remaining[i] = processes[i].burst_time;
        completion_times[i] = 0;
        completed[i] = false;
    }
    
    int current_time = 0;
    int last_pid = -1;
    int total_completed = 0;
    
    while (total_completed < n) {
        // Selecionar processo com maior prioridade
        int selected = -1;
        int highest_priority = INT_MAX;
        
        for (int i = 0; i < n; i++) {
            if (!completed[i] && processes[i].arrival_time <= current_time) {
                if (processes[i].priority < highest_priority) {
                    highest_priority = processes[i].priority;
                    selected = i;
                }
            }
        }
        
        if (selected != -1) {
            // Mostrar troca de contexto
            if (processes[selected].pid != last_pid) {
                if (last_pid != -1) printf("|");
                printf("P%d", processes[selected].pid);
                last_pid = processes[selected].pid;
            }
            
            printf("▉");
            remaining[selected]--;
            current_time++;
            
            // Verificar conclusão
            if (remaining[selected] == 0) {
                printf("✓");
                completion_times[selected] = current_time;
                completed[selected] = true;
                total_completed++;
                last_pid = -1;
            }
        } else {
            // Tempo ocioso
            printf("#");
            current_time++;
        }
    }
    
    // Atualizar os tempos de conclusão nos processos originais
    for (int i = 0; i < n; i++) {
        processes[i].completion_time = completion_times[i];
        processes[i].waiting_time = processes[i].completion_time - 
                                   processes[i].arrival_time - 
                                   processes[i].burst_time;
    }
    
    free(remaining);
    free(completion_times);
    free(completed);
    printf("\n");
}

void print_rr(Process *processes, int n, int quantum) {
    printf("\nEXECUÇÃO: ");
    
    int *remaining = malloc(n * sizeof(int));
    int *last_run = malloc(n * sizeof(int));
    
    for (int i = 0; i < n; i++) {
        remaining[i] = processes[i].burst_time;
        last_run[i] = -1;
    }
    
    int current_time = 0;
    int last_pid = -1;
    bool done = false;
    
    while (!done) {
        done = true;
        int selected = -1;
        
        // Seleção circular para RR
        static int rr_index = 0;
        for (int i = 0; i < n; i++) {
            int idx = (rr_index + i) % n;
            if (remaining[idx] > 0 && processes[idx].arrival_time <= current_time) {
                selected = idx;
                rr_index = (idx + 1) % n;
                break;
            }
        }
        
        if (selected != -1) {
            done = false;
            
            // Mostrar troca de contexto
            if (last_pid != processes[selected].pid && current_time > 0) {
                printf("|");
            }
            
            printf("P%d", processes[selected].pid);
            
            // Executar por quantum ou tempo restante
            int exec_time = (remaining[selected] > quantum) ? quantum : remaining[selected];
            for (int j = 0; j < exec_time; j++) {
                printf("▉");
            }
            
            remaining[selected] -= exec_time;
            current_time += exec_time;
            last_pid = processes[selected].pid;
            
            // Mostrar conclusão
            if (remaining[selected] == 0) {
                printf("✓");
            }
        } else {
            // Tempo ocioso apenas se houver processos futuros
            for (int i = 0; i < n; i++) {
                if (remaining[i] > 0 && processes[i].arrival_time > current_time) {
                    printf("#");
                    current_time++;
                    done = false;
                    break;
                }
            }
        }
    }
    
    printf("\n");
    free(remaining);
    free(last_run);
}

void print_rm(Process *processes, int n) {
    printf("\nEXECUÇÃO: ");

    qsort(processes, n, sizeof(Process), compare_period);
    
    // Criar cópias para não modificar os originais
    int *remaining = malloc(n * sizeof(int));
    int *next_release = malloc(n * sizeof(int));
    int current_time = 0;
    int last_pid = -1;
    
    // Inicialização
    for (int i = 0; i < n; i++) {
        remaining[i] = processes[i].burst_time;
        next_release[i] = processes[i].arrival_time;
        processes[i].completion_time = -1;
        processes[i].deadline_misses = 0;
    }
    
    while (1) {
        // Verificar se todos os processos foram completados
        bool all_completed = true;
        for (int i = 0; i < n; i++) {
            if (remaining[i] > 0) {
                all_completed = false;
                break;
            }
        }
        if (all_completed) break;
        
        // Selecionar processo com menor período (maior prioridade)
        int selected = -1;
        int min_period = INT_MAX;
        
        for (int i = 0; i < n; i++) {
            if (remaining[i] > 0 && processes[i].arrival_time <= current_time) {
                if (processes[i].period < min_period) {
                    min_period = processes[i].period;
                    selected = i;
                }
            }
        }
        
        if (selected != -1) {
            // Mostrar troca de contexto
            if (processes[selected].pid != last_pid) {
                if (last_pid != -1) printf("|");
                printf("P%d", processes[selected].pid);
                last_pid = processes[selected].pid;
            }
            
            printf("▉");
            remaining[selected]--;
            current_time++;
            
            // Verificar conclusão
            if (remaining[selected] == 0) {
                processes[selected].completion_time = current_time;
                printf("✓");
            }
        } else {
            // Tempo ocioso
            printf("#");
            current_time++;
        }
    }
    
    // Calcular waiting times e verificar deadlines
    for (int i = 0; i < n; i++) {
        if (processes[i].completion_time != -1) {
            processes[i].waiting_time = processes[i].completion_time - 
                                      processes[i].arrival_time - 
                                      processes[i].burst_time;
            
            // Verificar se perdeu o deadline
            int absolute_deadline = processes[i].arrival_time + processes[i].deadline;
            if (processes[i].completion_time > absolute_deadline) {
                processes[i].deadline_misses = 1;
            }
        }
    }
    
    free(remaining);
    free(next_release);
    printf("\n");
}

void print_edf(Process *processes, int n) {
    printf("\nEXECUÇÃO: ");
    
    // Estruturas temporárias para simulação
    int *remaining = malloc(n * sizeof(int));
    int *deadlines = malloc(n * sizeof(int));
    int current_time = 0;
    int completed = 0;
    int last_pid = -1;
    
    // Inicialização
    for (int i = 0; i < n; i++) {
        remaining[i] = processes[i].burst_time;
        deadlines[i] = processes[i].arrival_time + processes[i].deadline;
        processes[i].completion_time = -1;
        processes[i].waiting_time = 0;
        processes[i].deadline_misses = 0;
    }
    
    while (completed < n) {
        // Selecionar processo com deadline mais próximo que já chegou
        int selected = -1;
        int min_deadline = INT_MAX;
        
        for (int i = 0; i < n; i++) {
            if (remaining[i] > 0 && processes[i].arrival_time <= current_time) {
                if (deadlines[i] < min_deadline) {
                    min_deadline = deadlines[i];
                    selected = i;
                }
            }
        }
        
        if (selected != -1) {
            // Mostrar troca de contexto se necessário
            if (processes[selected].pid != last_pid) {
                if (last_pid != -1) printf("|");
                printf("P%d", processes[selected].pid);
                last_pid = processes[selected].pid;
            }
            
            printf("▉");
            remaining[selected]--;
            current_time++;
            
            // Verificar conclusão
            if (remaining[selected] == 0) {
                processes[selected].completion_time = current_time;
                completed++;
                
                if (current_time > deadlines[selected]) {
                    printf("!");
                    processes[selected].deadline_misses++;
                } else {
                    printf("✓");
                }
            }
        } else {
            // Tempo ocioso
            printf("#");
            current_time++;
        }
        
        // Verificar deadlines perdidos para processos que não começaram
        for (int i = 0; i < n; i++) {
            if (remaining[i] == processes[i].burst_time && 
                current_time >= deadlines[i] && 
                processes[i].completion_time == -1) {
                processes[i].deadline_misses++;
            }
        }
    }
    
    // Calcular waiting times
    for (int i = 0; i < n; i++) {
        if (processes[i].completion_time > 0) {
            processes[i].waiting_time = processes[i].completion_time - 
                                      processes[i].arrival_time - 
                                      processes[i].burst_time;
        }
    }
    
    free(remaining);
    free(deadlines);
    printf("\n");
}

void print_final_results(Process *processes, int n) {
    printf("\n=== Resultados Finais ===\n\n");
    printf("%-5s %-8s %-6s %-10s %-7s %-9s %-10s %-6s\n",
           "PID", "Chegada", "Burst", "Prioridade", "Deadline", "Período",
           "Finalizado", "Espera");
    printf("--------------------------------------------------------------------\n");
    
    for (int i = 0; i < n; i++) {
        printf("%-5d %-8d %-6d %-10d %-7d %-9d %-10d %-6d\n",
               processes[i].pid, 
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].priority,
               processes[i].deadline,
               processes[i].period,
               processes[i].completion_time,
               processes[i].waiting_time);
    }
}