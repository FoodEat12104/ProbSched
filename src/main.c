#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "process.h"
#include "scheduler.h"
#include "distributions.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Uso: %s <algoritmo> <num_processos> [quantum]\n", argv[0]);
        printf("Algoritmos disponíveis: FCFS, SJF, PRIORITY, RR, EDF\n");
        return 1;
    }

    int num_processes = atoi(argv[2]);
    int quantum = (argc > 3) ? atoi(argv[3]) : 0;
    
    Process *processes = generate_processes(num_processes);
    
    if (strcmp(argv[1], "FCFS") == 0) {
        run_fcfs(processes, num_processes);
    } else if (strcmp(argv[1], "SJF") == 0) {
        run_sjf(processes, num_processes);
    } else if (strcmp(argv[1], "PRIORITY") == 0) {
        run_priority(processes, num_processes);
    } else if (strcmp(argv[1], "RR") == 0) {
        if (quantum <= 0) {
            printf("RR requer um quantum positivo\n");
            return 1;
        }
        run_rr(processes, num_processes, quantum);
    } else if (strcmp(argv[1], "EDF") == 0) {
        run_edf(processes, num_processes);
    } else {
        printf("Algoritmo desconhecido\n");
        return 1;
    }

    free(processes);
    return 0;
}
