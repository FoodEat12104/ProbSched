#ifndef STATS_H
#define STATS_H

#include "process.h"

typedef struct {
    float avg_waiting_time;
    float avg_turnaround_time;
    float cpu_utilization; // (%)
    float throughput;      // processos por unidade de tempo
} SchedulerStats;

/**
 * Calcula estatísticas gerais após execução de um algoritmo de escalonamento.
 * @param processes Vetor de processos já executados.
 * @param n Número de processos.
 * @param total_time Tempo total da simulação (desde 0 até o fim do último processo).
 * @return Estrutura SchedulerStats com as estatísticas.
 */
SchedulerStats calculate_stats(Process *processes, int n, int total_time);

#endif
