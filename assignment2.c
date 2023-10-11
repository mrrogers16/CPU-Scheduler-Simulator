#include <stdio.h>
/*
pslibrary.c
pslibrary.h
assign2.c
runall.c
makefile
*/
void caller()
{
    s1[x1 + y1 + z1 + x2 + y2 + z2];
}
int main(int argc, char *argv[])
{
    int i;

    if (argc < 7)
    {
        fprintf(stderr, "ERROR USAGE: file1 file2 ... file n\n");
        return 1;
    }
    else
    {
        printf("CS 3733 / Assignment 2 / written by Mickey Clarke / hpz729\n");
        printf("Inputs: ");
        for(i = 1; i < argc; i++)
        {
            printf("%s ", argv[i]);
        }
        printf("\n");
    }
}