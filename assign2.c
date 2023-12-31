#include <stdio.h>
#include <stdlib.h>
#include "pslibrary.h"

#define READY 0
#define RUNNING 1
#define WAITING 2
#define DONE 3

/*
pslibrary.c
pslibrary.h
assign2.c
runall.c
makefile
*/

int main(int argc, char *argv[])
{
    int i;
    int quantum = atoi(argv[1]);
    int x1 = atoi(argv[2]);
    int y1 = atoi(argv[3]);
    int z1 = atoi(argv[4]);
    int x2 = atoi(argv[5]);
    int y2 = atoi(argv[6]);
    int z2 = atoi(argv[7]);
    int size = x1 + y1 + z1 + x2 + y2 + z2;
    char s1[size];
    char s2[size];
    struct CalculationResult result;

    if (argc < 8)
    {
        fprintf(stderr, "ERROR USAGE: %s quantum x1 y1 z1 x2 y2 z2\n", argv[0]);
        return 1;
    }
    else
    {
        printf("CS 3733 / Assignment 2 / written by Mickey Clarke / hpz729\n");
        printf("Inputs: ");
        for (i = 1; i < argc; i++)
        {
            printf("%s ", argv[i]);
        }
        printf("\n");
    }

    fcfs(s1, s2, x1, y1, z1, x2, y2, z2);
    sjf(s1, s2, x1, y1, z1, x2, y2, z2);
    psjf(s1, s2, x1, y1, z1, x2, y2, z2);
    rr(s1, s2, quantum, x1, y1, z1, x2, y2, z2);

    return EXIT_SUCCESS;
}