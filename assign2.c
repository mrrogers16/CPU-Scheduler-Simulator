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

    int x1 = atoi(argv[2]);
    int y1 = atoi(argv[3]);
    int z1 = atoi(argv[4]);
    int size = x1 + y1 + z1;
    char s1[size];


    if (argc < 7)
    {
        fprintf(stderr, "ERROR USAGE: file1 file2 ... file n\n");
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
}