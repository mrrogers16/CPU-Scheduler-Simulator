// pslibrary.h
#ifndef PSLIBRARY_H
#define PSLIBRARY_H

struct CalculationResult
{
        int avg_wait_time;
        int cpu_ut;
        int wait_count1;
        int wait_count2;
}CalculationResult;

// Round Robin
void rr(char *s1, char *s2, int quantum, int x1, int y1, int z1,
        int x2, int y2, int z2);

// Preemptive Shortest Job First
void psjf(char *s1, char *s2, int x1, int y1, int z1,
          int x2, int y2, int z2);

// Shortest Job First Algo
void sjf(char *s1, char *s2, int x1, int y1, int z1,
         int x2, int y2, int z2);

// First Come First Serve Algo
void fcfs(char *s1, char *s2, int x1, int y1, int z1,
          int x2, int y2, int z2);

// Calculate Function for average wait time and cpu utilization
CalculationResult *calculate(char *s1, char *s2);

#endif