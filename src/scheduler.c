#include "scheduler.h"
#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include <limits.h>

bool all_processes_completed(Process *processes, int n) {
    for (int i = 0; i < n; i++) {
        if (processes[i].remaining_time > 0) {
            return false;
        }
    }
    return true;
}

void run_fcfs(Process *processes, int n) {  
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            if (processes[j].arrival_time < processes[i].arrival_time) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
    
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

void run_sjf_nonpreemptive(Process *processes, int n) {    
    int current_time = 0;
    int completed = 0;
    int *completed_flags = calloc(n, sizeof(int));
    
    while (completed < n) {
        int shortest_index = -1;
        int shortest_burst = __INT_MAX__;
        
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

void run_sjf_preemptive(Process *processes, int n) {    
    int *remaining_time = malloc(n * sizeof(int));
    
    for (int i = 0; i < n; i++) {
        remaining_time[i] = processes[i].burst_time;
    }
    
    int current_time = 0;
    int completed = 0;
    
    while (completed < n) {
        int shortest_index = -1;
        int shortest_time = __INT_MAX__;
        
        for (int i = 0; i < n; i++) {
            if (remaining_time[i] > 0 && processes[i].arrival_time <= current_time && 
                remaining_time[i] < shortest_time) {
                shortest_time = remaining_time[i];
                shortest_index = i;
            }
        }
        
        if (shortest_index == -1) {
            current_time++;
            continue;
        }
        
        remaining_time[shortest_index]--;
        current_time++;
        
        if (remaining_time[shortest_index] == 0) {
            completed++;
            processes[shortest_index].completion_time = current_time;
            processes[shortest_index].waiting_time = processes[shortest_index].completion_time - 
                                                    processes[shortest_index].arrival_time - 
                                                    processes[shortest_index].burst_time;
        }
    }
    
    free(remaining_time);
}

void run_priority_nonpreemptive(Process *processes, int n) {    
    int current_time = 0;
    int completed = 0;
    int *completed_flags = calloc(n, sizeof(int));
    
    while (completed < n) {
        int highest_priority_index = -1;
        int highest_priority = __INT_MAX__;
        
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
    int *remaining_time = malloc(n * sizeof(int));
    
    for (int i = 0; i < n; i++) {
        remaining_time[i] = processes[i].burst_time;
    }
    
    int current_time = 0;
    int completed = 0;
    
    while (completed < n) {
        int highest_priority_index = -1;
        int highest_priority = __INT_MAX__;
        
        for (int i = 0; i < n; i++) {
            if (remaining_time[i] > 0 && processes[i].arrival_time <= current_time && 
                processes[i].priority < highest_priority) {
                highest_priority = processes[i].priority;
                highest_priority_index = i;
            }
        }
        
        if (highest_priority_index == -1) {
            current_time++;
            continue;
        }
        
        remaining_time[highest_priority_index]--;
        current_time++;
        
        if (remaining_time[highest_priority_index] == 0) {
            completed++;
            processes[highest_priority_index].completion_time = current_time;
            processes[highest_priority_index].waiting_time = processes[highest_priority_index].completion_time - 
                                                          processes[highest_priority_index].arrival_time - 
                                                          processes[highest_priority_index].burst_time;
        }
    }
    
    free(remaining_time);
}

void run_rr(Process *processes, int n, int quantum) {    
    int *remaining_time = malloc(n * sizeof(int));
    int *last_execution = malloc(n * sizeof(int));
    
    for (int i = 0; i < n; i++) {
        remaining_time[i] = processes[i].burst_time;
        last_execution[i] = processes[i].arrival_time;
    }
    
    int current_time = 0;
    int completed = 0;
    
    while (completed < n) {
        bool any_executed = false;
        
        for (int i = 0; i < n; i++) {
            if (remaining_time[i] > 0 && processes[i].arrival_time <= current_time) {
                any_executed = true;
                
                processes[i].waiting_time += current_time - last_execution[i];
                
                int exec_time = (remaining_time[i] > quantum) ? quantum : remaining_time[i];
                current_time += exec_time;
                remaining_time[i] -= exec_time;
                last_execution[i] = current_time;
                
                if (remaining_time[i] == 0) {
                    completed++;
                    processes[i].completion_time = current_time;
                }
            }
        }
        
        if (!any_executed) {
            current_time++;
        }
    }
    
    free(remaining_time);
    free(last_execution);
}


void run_rate_monotonic(Process *processes, int n) {
    // 1. Filtrar apenas processos periódicos válidos (período > 0)
    int valid_processes = 0;
    for (int i = 0; i < n; i++) {
        if (processes[i].period > 0) valid_processes++;
    }

    if (valid_processes == 0) {
        printf("AVISO: Nenhum processo periódico válido (período > 0)\n");
        return;
    }

    // 2. Ordenar por período (menor período = maior prioridade)
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            if (processes[j].period > 0 && (processes[i].period == 0 || 
                processes[j].period < processes[i].period)) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }

    // 3. Cálculo da utilização apenas para processos válidos
    float utilization = 0;
    for (int i = 0; i < n; i++) {
        if (processes[i].period > 0) {
            utilization += (float)processes[i].burst_time / processes[i].period;
        }
    }

    float bound = valid_processes * (pow(2, 1.0/valid_processes) - 1);
    printf("- Utilização total: %.2f\n", utilization);
    printf("- Limite de escalonabilidade: %.2f\n", bound);

    // 4. Simulação mesmo se não escalonável (modo best-effort)
    int *remaining_time = malloc(n * sizeof(int));
    int *next_release = malloc(n * sizeof(int));
    int hyperperiod = 1;
    int current_time = 0;
    int completed = 0;

    // Inicialização
    for (int i = 0; i < n; i++) {
        remaining_time[i] = 0;
        next_release[i] = processes[i].arrival_time;
        processes[i].completion_time = -1;
        processes[i].waiting_time = -1;
        if (processes[i].period > 0) {
            hyperperiod = lcm(hyperperiod, processes[i].period);
        }
    }

    printf("- Simulando em modo best-effort (hiperperíodo: %d)\n", hyperperiod);

    while (current_time < hyperperiod && completed < valid_processes) {
        // Liberar processos
        for (int i = 0; i < n; i++) {
        if (processes[i].period > 0 && current_time >= processes[i].arrival_time && 
            current_time == next_release[i]) {
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
                processes[selected].completion_time = current_time + 1;
                processes[selected].waiting_time = (current_time + 1) - 
                                                processes[selected].arrival_time - 
                                                processes[selected].burst_time;
                completed++;
                
                // Verificar deadline
                int deadline = next_release[selected] - processes[selected].period;
                if (processes[selected].completion_time > deadline) {
                    processes[selected].deadline_misses++;
                    printf("! Deadline perdido para PID %d (tempo %d > deadline %d)\n",
                           processes[selected].pid, 
                           processes[selected].completion_time,
                           deadline);
                }
            }
        }
        current_time++;
    }

    free(remaining_time);
    free(next_release);
}

void run_edf(Process *processes, int n) {
    // 1. Pré-processamento
    int has_periodic = 0;
    int simulation_time = 0;

    // Calcular tempo de simulação (hiperperíodo ou tempo até último deadline)
    for (int i = 0; i < n; i++) {
        if (processes[i].period > 0) {
            has_periodic = 1;
            simulation_time = lcm(simulation_time, processes[i].period);
        } else if (processes[i].deadline > simulation_time) {
            simulation_time = processes[i].deadline;
        }
    }

    if (!has_periodic) {
        simulation_time = (simulation_time == 0) ? 100 : simulation_time;
    }

    printf("- Tempo de simulação: %d unidades\n", simulation_time);

    // 2. Simulação
    int *remaining_time = malloc(n * sizeof(int));
    int *next_release = malloc(n * sizeof(int));
    int current_time = 0;

    // Inicialização
    for (int i = 0; i < n; i++) {
        remaining_time[i] = 0;
        next_release[i] = processes[i].arrival_time;
        processes[i].deadline_misses = 0;
    }

    while (current_time <= simulation_time) {
        // Liberação de processos
        for (int i = 0; i < n; i++) {
            if (current_time == next_release[i]) {
                remaining_time[i] = processes[i].burst_time;
                if (processes[i].period > 0) {
                    next_release[i] += processes[i].period;
                }
            }
        }

        // Seleção EDF
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
                processes[selected].waiting_time = (current_time + 1) - 
                                                processes[selected].arrival_time - 
                                                processes[selected].burst_time;
                
                // Verificação de deadline
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

int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int lcm(int a, int b) {
    return (a / gcd(a, b)) * b;
}