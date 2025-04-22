#ifndef PROCESS_H
#define PROCESS_H

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    int remaining_time;
    int deadline;
    int period; // Para processos periódicos
} Process;

Process *generate_processes(int n);
void print_process(Process p);
void print_processes(Process *processes, int n);

#endif
