// pslibrary.h
#ifndef PSLIBRARY_H
#define PSLIBRARY_H

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

#endif
