#include <stdio.h>

int main(int argc, char *argv[])
{
    int i;

    if (argc < 7)
    {
        fprintf(stderr, "ERROR USAGE: file1 file2 ... file n\n", argv[0]);
        return 1;
    }
    else
    {
        printf("CS 3733 / Assignment 2 / written by Mickey Clarke / hpz729");

        for(i = 0; i < argc; i++)
        {
            printf("Inputs: %d", argv[i]);
        }
    }
}