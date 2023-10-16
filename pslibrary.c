#include <stdio.h>

#define READY 0
#define RUNNING 1
#define WAITING 2
#define DONE 3

static char stateChars[] = {'r', 'R', 'w', '\0'};

/* 1) handle state changes:
running process completes CPU burst
IO complete
2) do context switch if necessary
both ready
one ready and CPU free
3) append appropriate characters to character arrays
avoid putting in multiple string terminators
*/
/* assume s1 and s2 point to buffers with enough space to hold the result */
/* assume that the int parameters are strictly greater than 0 */
void fcfs(char *s1, char *s2, int x1, int y1, int z1,
          int x2, int y2, int z2)
{
    int i;              /* next string position (time) */
    int state1 = READY; /* start with both ready */
    int state2 = READY;
    int cpuLeft1 = x1; /* P1 next CPU burst remaining */
    int cpuLeft2 = x2; /* P2 next CPU burst remaining */
    int ioLeft1 = y1;  /* P1 next IO burst remaining, 0 if no more IO */
    int ioLeft2 = y2;  /* P2 next IO burst remaining, 0 if no more IO */

    int cpu_total = 0; /* Total execution time*/

    for (i = 0; (state1 != DONE) || (state2 != DONE); i++)
    {
        cpu_total++; /* Increment total time each loop through*/

        /* running process completes its CPU burst */
        if ((state1 == RUNNING) && (cpuLeft1 == 0))
        {
            if (ioLeft1 == 0)
            {
                state1 = DONE;
                s1[i] = stateChars[state1]; /* terminate the string */
            }
            else
                state1 = WAITING;
        }
        else if ((state2 == RUNNING) && (cpuLeft2 == 0))
        {
            if (ioLeft2 == 0)
            {
                state2 = DONE;
                s2[i] = stateChars[state2]; /* terminate the string */
            }
            else
                state2 = WAITING;
        }
        /* handle IO complete */
        if ((state1 == WAITING) && (ioLeft1 == 0))
        {
            state1 = READY;
            cpuLeft1 = z1;
        }
        if ((state2 == WAITING) && (ioLeft2 == 0))
        {
            state2 = READY;
            cpuLeft2 = z2;
        }
        /* if both ready, depends on algorithm */
        if ((state1 == READY) && (state2 == READY))
        {
            state1 = RUNNING;
        }
        /* handle one ready and CPU available */
        else if ((state1 == READY) && (state2 != RUNNING))
        {
            state1 = RUNNING;
        }
        else if ((state2 == READY) && (state1 != RUNNING))
        {
            state2 = RUNNING;
        }
        /* insert chars in string, but avoid putting in extra string terminators */
        if (state1 != DONE)
            s1[i] = stateChars[state1];
        if (state2 != DONE)
            s2[i] = stateChars[state2];
        /* decrement counts */

        if (state1 == RUNNING)
            cpuLeft1--;
        if (state1 == WAITING)
            ioLeft1--;
        if (state2 == RUNNING)
            cpuLeft2--;
        if (state2 == WAITING)
            ioLeft2--;
    } /* end of main for loop */

    printf("S1: ");
    for (i = 0; s1[i] != '\0'; i++)
    {
        printf("%c", s1[i]);
    }
    printf("\n");

    printf("S2: ");
    for (i = 0; s2[i] != '\0'; i++)
    {
        printf("%c", s2[i]);
    }
    printf("\n");
}
