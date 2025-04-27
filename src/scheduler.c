#include "scheduler.h"
#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

// Função auxiliar para qsort - ordenar por tempo de chegada
static int compare_arrival(const void *a, const void *b) {
    const Process *p1 = (const Process *)a;
    const Process *p2 = (const Process *)b;
    return p1->arrival_time - p2->arrival_time;
}

// Função auxiliar para qsort - ordenar por período (Rate Monotonic)
static int compare_period(const void *a, const void *b) {
    const Process *p1 = (const Process *)a;
    const Process *p2 = (const Process *)b;
    return p1->period - p2->period;
}

// Funções auxiliares para LCM (Rate Monotonic e EDF)
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int lcm(int a, int b) {
    if (a == 0 || b == 0) return 0;
    return (a / gcd(a, b)) * b;
}

void run_fcfs(Process *processes, int n) {
    qsort(processes, n, sizeof(Process), compare_arrival);
    
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        
        processes[i].waiting_time = current_time - processes[i].arrival_time;
        processes[i].completion_time = current_time + processes[i].burst_time;
        current_time += processes[i].burst_time;
    }
}

void run_sjf(Process *processes, int n) {
    int *completed_flags = calloc(n, sizeof(int));
    if (!completed_flags) return;
    
    int current_time = 0;
    int completed = 0;
    
    while (completed < n) {
        int shortest_index = -1;
        int shortest_burst = INT_MAX;
        
        for (int i = 0; i < n; i++) {
            if (!completed_flags[i] && processes[i].arrival_time <= current_time && 
                processes[i].burst_time < shortest_burst) {
                shortest_burst = processes[i].burst_time;
                shortest_index = i;
            }
        }
        
        if (shortest_index == -1) {
            current_time++;
            continue;
        }
        
        processes[shortest_index].waiting_time = current_time - processes[shortest_index].arrival_time;
        processes[shortest_index].completion_time = current_time + processes[shortest_index].burst_time;
        current_time += processes[shortest_index].burst_time;
        completed_flags[shortest_index] = 1;
        completed++;
    }
    
    free(completed_flags);
}

void run_priority_nonpreemptive(Process *processes, int n) {
    int *completed_flags = calloc(n, sizeof(int));
    if (!completed_flags) return;
    
    int current_time = 0;
    int completed = 0;
    
    while (completed < n) {
        int highest_priority_index = -1;
        int highest_priority = INT_MAX;
        
        for (int i = 0; i < n; i++) {
            if (!completed_flags[i] && processes[i].arrival_time <= current_time && 
                processes[i].priority < highest_priority) {
                highest_priority = processes[i].priority;
                highest_priority_index = i;
            }
        }
        
        if (highest_priority_index == -1) {
            current_time++;
            continue;
        }
        
        processes[highest_priority_index].waiting_time = current_time - processes[highest_priority_index].arrival_time;
        processes[highest_priority_index].completion_time = current_time + processes[highest_priority_index].burst_time;
        current_time += processes[highest_priority_index].burst_time;
        completed_flags[highest_priority_index] = 1;
        completed++;
    }
    
    free(completed_flags);
}

void run_priority_preemptive(Process *processes, int n) {
      int time = 0;
    int completed = 0;
    
    // Inicializa remaining_time
    for (int i = 0; i < n; i++) {
        processes[i].remaining_time = processes[i].burst_time;
    }

    while (completed < n) {
        int highest_priority = INT_MAX;
        int selected = -1;

        // Encontra o processo com maior prioridade (menor número) que já chegou e ainda tem trabalho
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= time && processes[i].remaining_time > 0) {
                if (processes[i].priority < highest_priority) {
                    highest_priority = processes[i].priority;
                    selected = i;
                }
            }
        }

        if (selected == -1) {
            time++;
            continue;
        }

        // Executa o processo por 1 unidade de tempo
        processes[selected].remaining_time--;
        time++;

        // Verifica se o processo foi concluído
        if (processes[selected].remaining_time == 0) {
            completed++;
            processes[selected].completion_time = time;
        }
    }
}

void run_rr(Process *processes, int n, int quantum) {
    int *remaining_time = malloc(n * sizeof(int));
    int *last_execution = malloc(n * sizeof(int));
    if (!remaining_time || !last_execution) {
        free(remaining_time);
        free(last_execution);
        return;
    }
    
    for (int i = 0; i < n; i++) {
        remaining_time[i] = processes[i].burst_time;
        last_execution[i] = processes[i].arrival_time;
        processes[i].waiting_time = 0;
    }

    int current_time = 0;
    while (1) {
        bool all_done = true;
        bool executed = false;

        for (int i = 0; i < n; i++) {
            if (remaining_time[i] > 0) {
                all_done = false;
                
                if (processes[i].arrival_time <= current_time) {
                    executed = true;
                    processes[i].waiting_time += current_time - last_execution[i];
                    
                    int exec_time = (remaining_time[i] > quantum) ? quantum : remaining_time[i];
                    current_time += exec_time;
                    remaining_time[i] -= exec_time;
                    last_execution[i] = current_time;
                    
                    if (remaining_time[i] == 0) {
                        processes[i].completion_time = current_time;
                    }
                    break;
                }
            }
        }

        if (all_done) break;
        if (!executed) current_time++;
    }

    free(remaining_time);
    free(last_execution);
}

void run_rate_monotonic(Process *processes, int n) {
    // Filtrar processos periódicos válidos
    int valid_count = 0;
    for (int i = 0; i < n; i++) {
        if (processes[i].period > 0) valid_count++;
    }

    if (valid_count == 0) {
        printf("Nenhum processo periódico válido (período > 0)\n");
        return;
    }

    // Ordenar por período (Rate Monotonic)
    qsort(processes, n, sizeof(Process), compare_period);

    // Calcular utilização e limite
    float utilization = 0;
    for (int i = 0; i < n; i++) {
        if (processes[i].period > 0) {
            utilization += (float)processes[i].burst_time / processes[i].period;
        }
    }

    float bound = valid_count * (pow(2, 1.0/valid_count) - 1);
    printf("Utilização: %.2f, Limite: %.2f\n", utilization, bound);

    // Inicializar estruturas
    int *remaining_time = malloc(n * sizeof(int));
    int *next_release = malloc(n * sizeof(int));
    if (!remaining_time || !next_release) {
        free(remaining_time);
        free(next_release);
        return;
    }

    int hyperperiod = 1;
    for (int i = 0; i < n; i++) {
        remaining_time[i] = 0;
        next_release[i] = processes[i].arrival_time;
        if (processes[i].period > 0) {
            hyperperiod = lcm(hyperperiod, processes[i].period);
        }
    }

    // Simulação
    int current_time = 0;
    while (current_time < hyperperiod) {
        // Liberar processos
        for (int i = 0; i < n; i++) {
            if (processes[i].period > 0 && current_time >= next_release[i]) {
                remaining_time[i] = processes[i].burst_time;
                next_release[i] += processes[i].period;
            }
        }

        // Selecionar processo
        int selected = -1;
        for (int i = 0; i < n; i++) {
            if (processes[i].period > 0 && remaining_time[i] > 0 && 
                (selected == -1 || processes[i].period < processes[selected].period)) {
                selected = i;
            }
        }

        // Execução
        if (selected != -1) {
            remaining_time[selected]--;
            
            if (remaining_time[selected] == 0) {
                int deadline = next_release[selected] - processes[selected].period;
                processes[selected].completion_time = current_time + 1;
                processes[selected].waiting_time = current_time + 1 - 
                    processes[selected].arrival_time - processes[selected].burst_time;
                
                if (processes[selected].completion_time > deadline) {
                    processes[selected].deadline_misses++;
                }
            }
        }
        current_time++;
    }

    free(remaining_time);
    free(next_release);
}

void run_edf(Process *processes, int n) {
    // Calcular tempo de simulação
    int simulation_time = 0;
    for (int i = 0; i < n; i++) {
        if (processes[i].period > 0) {
            simulation_time = lcm(simulation_time, processes[i].period);
        } else if (processes[i].deadline > simulation_time) {
            simulation_time = processes[i].deadline;
        }
    }
    if (simulation_time == 0) simulation_time = 100;

    // Inicializar estruturas
    int *remaining_time = malloc(n * sizeof(int));
    int *next_release = malloc(n * sizeof(int));
    if (!remaining_time || !next_release) {
        free(remaining_time);
        free(next_release);
        return;
    }

    for (int i = 0; i < n; i++) {
        remaining_time[i] = 0;
        next_release[i] = processes[i].arrival_time;
        processes[i].deadline_misses = 0;
    }

    // Simulação
    int current_time = 0;
    while (current_time <= simulation_time) {
        // Liberar processos
        for (int i = 0; i < n; i++) {
            if (current_time == next_release[i]) {
                remaining_time[i] = processes[i].burst_time;
                if (processes[i].period > 0) {
                    next_release[i] += processes[i].period;
                }
            }
        }

        // Selecionar EDF
        int selected = -1;
        int earliest_deadline = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (remaining_time[i] > 0) {
                int current_deadline = (processes[i].period > 0) ? 
                    (next_release[i] - processes[i].period) : processes[i].deadline;
                
                if (current_deadline < earliest_deadline) {
                    earliest_deadline = current_deadline;
                    selected = i;
                }
            }
        }

        // Execução
        if (selected != -1) {
            remaining_time[selected]--;
            
            if (remaining_time[selected] == 0) {
                processes[selected].completion_time = current_time + 1;
                processes[selected].waiting_time = current_time + 1 - 
                    processes[selected].arrival_time - processes[selected].burst_time;
                
                if (processes[selected].completion_time > earliest_deadline) {
                    processes[selected].deadline_misses++;
                }
            }
        }
        current_time++;
    }

    free(remaining_time);
    free(next_release);
}