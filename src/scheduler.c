#include "scheduler.h"
#include "process.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

// Função auxiliar para verificar se todos os processos terminaram
bool all_processes_completed(Process *processes, int n) {
    for (int i = 0; i < n; i++) {
        if (processes[i].remaining_time > 0) {
            return false;
        }
    }
    return true;
}

// Implementação do FCFS (não preemptivo)
void run_fcfs(Process *processes, int n) {
    printf("\nExecutando FCFS (First-Come, First-Served)\n");
    
    // Ordenar processos por tempo de chegada
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

// Implementação do SJF não preemptivo
void run_sjf_nonpreemptive(Process *processes, int n) {
    printf("\nExecutando SJF não preemptivo (Shortest Job First)\n");
    
    int current_time = 0;
    float total_waiting = 0;
    float total_turnaround = 0;
    int completed = 0;
    int *completed_flags = calloc(n, sizeof(int));
    
    while (completed < n) {
        int shortest_index = -1;
        int shortest_burst = __INT_MAX__;
        
        // Encontrar o processo com menor burst time que já chegou
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
        
        // Executar o processo até completar
        int waiting_time = current_time - processes[shortest_index].arrival_time;
        int turnaround_time = waiting_time + processes[shortest_index].burst_time;
        
        total_waiting += waiting_time;
        total_turnaround += turnaround_time;
        
        printf("Processo %d: Espera=%d, Turnaround=%d\n",
               processes[shortest_index].pid, waiting_time, turnaround_time);
        
        current_time += processes[shortest_index].burst_time;
        completed_flags[shortest_index] = 1;
        completed++;
    }
    
    printf("\nMédias: Espera=%.2f, Turnaround=%.2f\n",
           total_waiting/n, total_turnaround/n);
    free(completed_flags);
}

// Implementação do SJF preemptivo
void run_sjf_preemptive(Process *processes, int n) {
    printf("\nExecutando SJF preemptivo (Shortest Remaining Time First)\n");
    
    int *remaining_time = malloc(n * sizeof(int));
    int *waiting_time = calloc(n, sizeof(int));
    int *completion_time = calloc(n, sizeof(int));
    int *is_completed = calloc(n, sizeof(int));
    
    for (int i = 0; i < n; i++) {
        remaining_time[i] = processes[i].burst_time;
    }
    
    int current_time = 0;
    int completed = 0;
    
    while (completed < n) {
        int shortest_index = -1;
        int shortest_time = __INT_MAX__;
        
        // Encontrar o processo com menor tempo restante que já chegou
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && processes[i].arrival_time <= current_time && 
                remaining_time[i] < shortest_time) {
                shortest_time = remaining_time[i];
                shortest_index = i;
            }
        }
        
        if (shortest_index == -1) {
            current_time++;
            continue;
        }
        
        // Executar o processo por 1 unidade de tempo
        remaining_time[shortest_index]--;
        current_time++;
        
        if (remaining_time[shortest_index] == 0) {
            completed++;
            is_completed[shortest_index] = 1;
            completion_time[shortest_index] = current_time;
        }
    }
    
    // Calcular estatísticas
    float total_waiting = 0;
    float total_turnaround = 0;
    
    for (int i = 0; i < n; i++) {
        int turnaround = completion_time[i] - processes[i].arrival_time;
        int waiting = turnaround - processes[i].burst_time;
        total_waiting += waiting;
        total_turnaround += turnaround;
        
        printf("Processo %d: Espera=%d, Turnaround=%d\n",
               processes[i].pid, waiting, turnaround);
    }
    
    printf("\nMédias: Espera=%.2f, Turnaround=%.2f\n",
           total_waiting/n, total_turnaround/n);
    
    free(remaining_time);
    free(waiting_time);
    free(completion_time);
    free(is_completed);
}

// Implementação do Priority não preemptivo
void run_priority_nonpreemptive(Process *processes, int n) {
    printf("\nExecutando Priority Scheduling não preemptivo\n");
    
    int current_time = 0;
    float total_waiting = 0;
    float total_turnaround = 0;
    int completed = 0;
    int *completed_flags = calloc(n, sizeof(int));
    
    while (completed < n) {
        int highest_priority_index = -1;
        int highest_priority = __INT_MAX__;
        
        // Encontrar o processo com maior prioridade (menor número) que já chegou
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
        
        // Executar o processo até completar
        int waiting_time = current_time - processes[highest_priority_index].arrival_time;
        int turnaround_time = waiting_time + processes[highest_priority_index].burst_time;
        
        total_waiting += waiting_time;
        total_turnaround += turnaround_time;
        
        printf("Processo %d: Espera=%d, Turnaround=%d\n",
               processes[highest_priority_index].pid, waiting_time, turnaround_time);
        
        current_time += processes[highest_priority_index].burst_time;
        completed_flags[highest_priority_index] = 1;
        completed++;
    }
    
    printf("\nMédias: Espera=%.2f, Turnaround=%.2f\n",
           total_waiting/n, total_turnaround/n);
    free(completed_flags);
}

// Implementação do Priority preemptivo
void run_priority_preemptive(Process *processes, int n) {
    printf("\nExecutando Priority Scheduling preemptivo\n");
    
    int *remaining_time = malloc(n * sizeof(int));
    int *waiting_time = calloc(n, sizeof(int));
    int *completion_time = calloc(n, sizeof(int));
    int *is_completed = calloc(n, sizeof(int));
    
    for (int i = 0; i < n; i++) {
        remaining_time[i] = processes[i].burst_time;
    }
    
    int current_time = 0;
    int completed = 0;
    
    while (completed < n) {
        int highest_priority_index = -1;
        int highest_priority = __INT_MAX__;
        
        // Encontrar o processo com maior prioridade (menor número) que já chegou
        for (int i = 0; i < n; i++) {
            if (!is_completed[i] && processes[i].arrival_time <= current_time && 
                processes[i].priority < highest_priority) {
                highest_priority = processes[i].priority;
                highest_priority_index = i;
            }
        }
        
        if (highest_priority_index == -1) {
            current_time++;
            continue;
        }
        
        // Executar o processo por 1 unidade de tempo
        remaining_time[highest_priority_index]--;
        current_time++;
        
        if (remaining_time[highest_priority_index] == 0) {
            completed++;
            is_completed[highest_priority_index] = 1;
            completion_time[highest_priority_index] = current_time;
        }
    }
    
    // Calcular estatísticas
    float total_waiting = 0;
    float total_turnaround = 0;
    
    for (int i = 0; i < n; i++) {
        int turnaround = completion_time[i] - processes[i].arrival_time;
        int waiting = turnaround - processes[i].burst_time;
        total_waiting += waiting;
        total_turnaround += turnaround;
        
        printf("Processo %d: Espera=%d, Turnaround=%d\n",
               processes[i].pid, waiting, turnaround);
    }
    
    printf("\nMédias: Espera=%.2f, Turnaround=%.2f\n",
           total_waiting/n, total_turnaround/n);
    
    free(remaining_time);
    free(waiting_time);
    free(completion_time);
    free(is_completed);
}

// Implementação do Round Robin
void run_rr(Process *processes, int n, int quantum) {
    printf("\nExecutando Round Robin (Quantum=%d)\n", quantum);
    
    int *remaining_time = malloc(n * sizeof(int));
    int *waiting_time = calloc(n, sizeof(int));
    int *last_execution = malloc(n * sizeof(int));
    int current_time = 0;
    float total_waiting = 0;
    float total_turnaround = 0;
    
    for (int i = 0; i < n; i++) {
        remaining_time[i] = processes[i].burst_time;
        last_execution[i] = processes[i].arrival_time;
    }
    
    int completed = 0;
    while (completed < n) {
        bool any_executed = false;
        
        for (int i = 0; i < n; i++) {
            if (remaining_time[i] > 0 && processes[i].arrival_time <= current_time) {
                any_executed = true;
                
                // Calcular tempo de espera acumulado
                waiting_time[i] += current_time - last_execution[i];
                
                int exec_time = (remaining_time[i] > quantum) ? quantum : remaining_time[i];
                current_time += exec_time;
                remaining_time[i] -= exec_time;
                last_execution[i] = current_time;
                
                if (remaining_time[i] == 0) {
                    completed++;
                    int turnaround = current_time - processes[i].arrival_time;
                    total_waiting += waiting_time[i];
                    total_turnaround += turnaround;
                    
                    printf("Processo %d: Espera=%d, Turnaround=%d\n",
                           processes[i].pid, waiting_time[i], turnaround);
                }
            }
        }
        
        if (!any_executed) {
            current_time++;
        }
    }
    
    printf("\nMédias: Espera=%.2f, Turnaround=%.2f\n",
           total_waiting/n, total_turnaround/n);
    
    free(remaining_time);
    free(waiting_time);
    free(last_execution);
}

// Implementação do Rate Monotonic (para processos periódicos)
void run_rate_monotonic(Process *processes, int n) {
    printf("\nExecutando Rate Monotonic Scheduling\n");
    
    // Ordenar processos por período (menor período = maior prioridade)
    for (int i = 0; i < n-1; i++) {
        for (int j = i+1; j < n; j++) {
            if (processes[j].period < processes[i].period) {
                Process temp = processes[i];
                processes[i] = processes[j];
                processes[j] = temp;
            }
        }
    }
    
    // Verificar escalonabilidade (Teste de Liu & Layland)
    float utilization = 0;
    for (int i = 0; i < n; i++) {
        utilization += (float)processes[i].burst_time / processes[i].period;
    }
    
    float bound = n * (pow(2, 1.0/n) - 1);
    printf("Utilização total: %.2f\n", utilization);
    printf("Limite de escalonabilidade: %.2f\n", bound);
    
    if (utilization <= bound) {
        printf("O conjunto de processos é escalonável pelo Rate Monotonic.\n");
    } else {
        printf("O conjunto de processos pode não ser escalonável pelo Rate Monotonic.\n");
    }
    
    // Simular execução por um hiperperíodo (LCM dos períodos)
    int hyperperiod = 1;
    for (int i = 0; i < n; i++) {
        hyperperiod = lcm(hyperperiod, processes[i].period);
    }
    
    printf("Simulando execução por hiperperíodo de %d unidades de tempo\n", hyperperiod);
    
    int *remaining_time = malloc(n * sizeof(int));
    int *next_release = malloc(n * sizeof(int));
    int *deadlines_met = calloc(n, sizeof(int));
    int *deadlines_missed = calloc(n, sizeof(int));
    
    for (int i = 0; i < n; i++) {
        remaining_time[i] = 0;
        next_release[i] = processes[i].arrival_time;
    }
    
    for (int current_time = 0; current_time < hyperperiod; current_time++) {
        // Verificar se algum processo foi liberado
        for (int i = 0; i < n; i++) {
            if (current_time == next_release[i]) {
                remaining_time[i] = processes[i].burst_time;
                next_release[i] += processes[i].period;
            }
        }
        
        // Escolher o processo de maior prioridade (menor período) que está pronto
        int selected = -1;
        for (int i = 0; i < n; i++) {
            if (remaining_time[i] > 0) {
                if (selected == -1 || processes[i].period < processes[selected].period) {
                    selected = i;
                }
            }
        }
        
        if (selected != -1) {
            // Executar o processo selecionado
            remaining_time[selected]--;
            printf("Tempo %d: Executando processo %d\n", current_time, processes[selected].pid);
            
            // Verificar se completou a execução antes do deadline
            if (remaining_time[selected] == 0) {
                int deadline = next_release[selected] - processes[selected].period + processes[selected].deadline;
                if (current_time + 1 <= deadline) {
                    deadlines_met[selected]++;
                } else {
                    deadlines_missed[selected]++;
                    printf("DEADLINE PERDIDO para processo %d no tempo %d\n", processes[selected].pid, current_time);
                }
            }
        } else {
            printf("Tempo %d: Nenhum processo para executar\n", current_time);
        }
    }
    
    // Imprimir estatísticas
    printf("\nResumo de deadlines:\n");
    for (int i = 0; i < n; i++) {
        int total = deadlines_met[i] + deadlines_missed[i];
        if (total > 0) {
            printf("Processo %d: %d/%d deadlines atendidos (%.1f%%)\n",
                   processes[i].pid, deadlines_met[i], total,
                   (float)deadlines_met[i] / total * 100);
        }
    }
    
    free(remaining_time);
    free(next_release);
    free(deadlines_met);
    free(deadlines_missed);
}

// Implementação do EDF (Earliest Deadline First)
void run_edf(Process *processes, int n) {
    printf("\nExecutando EDF (Earliest Deadline First)\n");
    
    // Simular execução por um hiperperíodo (LCM dos períodos para processos periódicos)
    int hyperperiod = 1;
    bool has_periodic = false;
    for (int i = 0; i < n; i++) {
        if (processes[i].period > 0) {
            hyperperiod = lcm(hyperperiod, processes[i].period);
            has_periodic = true;
        }
    }
    
    if (!has_periodic) {
        hyperperiod = 100; // Valor padrão para processos não periódicos
    }
    
    printf("Simulando execução por %d unidades de tempo\n", hyperperiod);
    
    int *remaining_time = malloc(n * sizeof(int));
    int *next_release = malloc(n * sizeof(int));
    int *deadlines_met = calloc(n, sizeof(int));
    int *deadlines_missed = calloc(n, sizeof(int));
    
    for (int i = 0; i < n; i++) {
        remaining_time[i] = 0;
        next_release[i] = processes[i].arrival_time;
    }
    
    for (int current_time = 0; current_time < hyperperiod; current_time++) {
        // Verificar se algum processo foi liberado
        for (int i = 0; i < n; i++) {
            if (current_time == next_release[i]) {
                remaining_time[i] = processes[i].burst_time;
                if (processes[i].period > 0) {
                    next_release[i] += processes[i].period;
                } else {
                    next_release[i] = __INT_MAX__; // Não periódico
                }
            }
        }
        
        // Escolher o processo com deadline mais próximo que está pronto
        int selected = -1;
        int earliest_deadline = __INT_MAX__;
        
        for (int i = 0; i < n; i++) {
            if (remaining_time[i] > 0) {
                int deadline;
                if (processes[i].period > 0) {
                    deadline = next_release[i] - processes[i].period + processes[i].deadline;
                } else {
                    deadline = processes[i].deadline;
                }
                
                if (deadline < earliest_deadline) {
                    earliest_deadline = deadline;
                    selected = i;
                }
            }
        }
        
        if (selected != -1) {
            // Executar o processo selecionado
            remaining_time[selected]--;
            printf("Tempo %d: Executando processo %d (deadline %d)\n", 
                   current_time, processes[selected].pid, earliest_deadline);
            
            // Verificar se completou a execução antes do deadline
            if (remaining_time[selected] == 0) {
                int deadline;
                if (processes[selected].period > 0) {
                    deadline = next_release[selected] - processes[selected].period + processes[selected].deadline;
                } else {
                    deadline = processes[selected].deadline;
                }
                
                if (current_time + 1 <= deadline) {
                    deadlines_met[selected]++;
                } else {
                    deadlines_missed[selected]++;
                    printf("DEADLINE PERDIDO para processo %d no tempo %d\n", processes[selected].pid, current_time);
                }
            }
        } else {
            printf("Tempo %d: Nenhum processo para executar\n", current_time);
        }
    }
    
    // Imprimir estatísticas
    printf("\nResumo de deadlines:\n");
    for (int i = 0; i < n; i++) {
        int total = deadlines_met[i] + deadlines_missed[i];
        if (total > 0) {
            printf("Processo %d: %d/%d deadlines atendidos (%.1f%%)\n",
                   processes[i].pid, deadlines_met[i], total,
                   (float)deadlines_met[i] / total * 100);
        }
    }
    
    free(remaining_time);
    free(next_release);
    free(deadlines_met);
    free(deadlines_missed);
}

// Função auxiliar para calcular MDC
int gcd(int a, int b) {
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

// Função auxiliar para calcular MMC
int lcm(int a, int b) {
    return (a / gcd(a, b)) * b;
}

// Implementação de calculate_stats
SchedulerStats calculate_stats(Process *processes, int n, int total_time) {
    SchedulerStats stats = {0};
    
    // Calcular throughput (processos completados por unidade de tempo)
    int completed = 0;
    for (int i = 0; i < n; i++) {
        if (processes[i].remaining_time == 0) {
            completed++;
        }
    }
    stats.throughput = (float)completed / total_time;
    
    // Calcular utilização da CPU
    int busy_time = 0;
    for (int i = 0; i < n; i++) {
        busy_time += processes[i].burst_time - processes[i].remaining_time;
    }
    stats.cpu_utilization = (float)busy_time / total_time * 100;
    
    // Calcular tempo médio de espera e turnaround
    float total_waiting = 0;
    float total_turnaround = 0;
    int count = 0;
    int deadline_misses = 0;
    
    for (int i = 0; i < n; i++) {
        if (processes[i].completion_time > 0) {
            int turnaround = processes[i].completion_time - processes[i].arrival_time;
            int waiting = turnaround - processes[i].burst_time;
            total_waiting += waiting;
            total_turnaround += turnaround;
            count++;

            // Verificar se o deadline foi perdido
            if (processes[i].deadline > 0 && processes[i].completion_time > processes[i].deadline) {
                deadline_misses++;
            }
        }
    }
    
    if (count > 0) {
        stats.avg_waiting_time = total_waiting / count;
        stats.avg_turnaround_time = total_turnaround / count;
    }
    stats.deadline_misses = deadline_misses;
    
    return stats;
}

void print_stats(SchedulerStats stats) {
    printf("\n=== Estatísticas da Simulação ===\n");
    printf("Tempo médio de espera: %.2f\n", stats.avg_waiting_time);
    printf("Tempo médio de turnaround: %.2f\n", stats.avg_turnaround_time);
    printf("Utilização da CPU: %.2f%%\n", stats.cpu_utilization);
    printf("Throughput: %.2f processos/unidade de tempo\n", stats.throughput);
    printf("Deadlines perdidos: %d\n", stats.deadline_misses);
}
