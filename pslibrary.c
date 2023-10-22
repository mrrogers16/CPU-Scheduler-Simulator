#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pslibrary.h"

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

void rr(char *s1, char *s2, int quantum, int x1, int y1, int z1,
        int x2, int y2, int z2)
{
    struct CalculationResult *result;
    int i;              /* next string position (time) */
    int state1 = READY; /* start with both ready */
    int state2 = READY;
    int cpuLeft1 = x1; /* P1 next CPU burst remaining */
    int cpuLeft2 = x2; /* P2 next CPU burst remaining */
    int ioLeft1 = y1;  /* P1 next IO burst remaining, 0 if no more IO */
    int ioLeft2 = y2;  /* P2 next IO burst remaining, 0 if no more IO */
    int qleft = quantum;

    for (i = 0; (state1 != DONE) || (state2 != DONE); i++)
    {
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
        /* running process has quantum expire */
        if ((state1 == RUNNING) && (qleft == 0))
        {
            state1 = READY;

            if (state2 == READY)
            {
                state2 = RUNNING;
                qleft = quantum;
            }
        }
        if ((state2 == RUNNING) && (qleft == 0))
        {
            state2 = READY;

            if (state1 == READY)
            {
                state1 = RUNNING;
                qleft = quantum;
            }
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
            qleft = quantum;
        }
        /* handle one ready and CPU available */
        else if ((state1 == READY) && (state2 != RUNNING))
        {
            state1 = RUNNING;
            qleft = quantum;
        }
        else if ((state2 == READY) && (state1 != RUNNING))
        {
            state2 = RUNNING;
            qleft = quantum;
        }
        /* insert chars in string, but avoid putting in extra string terminators */
        if (state1 != DONE)
            s1[i] = stateChars[state1];
        if (state2 != DONE)
            s2[i] = stateChars[state2];
        /* decrement counts */
        qleft--; /* OK to decrement even if nothing running */
        if (state1 == RUNNING)
            cpuLeft1--;
        if (state1 == WAITING)
            ioLeft1--;
        if (state2 == RUNNING)
            cpuLeft2--;
        if (state2 == WAITING)
            ioLeft2--;
    } /* end of main for loop */
    printf("Scheduler RR:\n");
    for (i = 0; s1[i] != '\0'; i++)
    {
        printf("%c", s1[i]);
    }
    printf("\n");
    for (i = 0; s2[i] != '\0'; i++)
    {
        printf("%c", s2[i]);
    }
    printf("\n");
    result = calculate(s1, s2);
    printCalculations(result);
}

void psjf(char *s1, char *s2, int x1, int y1, int z1,
          int x2, int y2, int z2)
{
    struct CalculationResult *result;
    int i;              /* next string position (time) */
    int state1 = READY; /* start with both ready */
    int state2 = READY;
    int cpuLeft1 = x1; /* P1 next CPU burst remaining */
    int cpuLeft2 = x2; /* P2 next CPU burst remaining */
    int ioLeft1 = y1;  /* P1 next IO burst remaining, 0 if no more IO */
    int ioLeft2 = y2;  /* P2 next IO burst remaining, 0 if no more IO */

    for (i = 0; (state1 != DONE) || (state2 != DONE); i++)
    {
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
            if (cpuLeft1 < cpuLeft2)
            {
                state1 = RUNNING;
                state2 = READY;
            }
        }
        if ((state2 == WAITING) && (ioLeft2 == 0))
        {
            state2 = READY;
            cpuLeft2 = z2;
            if (cpuLeft1 > cpuLeft2)
            {
                state2 = RUNNING;
                state1 = READY;
            }
        }
        /* if both ready, depends on algorithm */
        if ((state1 == READY) && (state2 == READY))
        {
            if (cpuLeft1 <= cpuLeft2)
            {
                state1 = RUNNING;
            }
            else
            {
                state2 = RUNNING;
            }
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

    printf("Scheduler PSJF:\n");
    for (i = 0; s1[i] != '\0'; i++)
    {
        printf("%c", s1[i]);
    }
    printf("\n");
    for (i = 0; s2[i] != '\0'; i++)
    {
        printf("%c", s2[i]);
    }
    printf("\n");
    result = calculate(s1, s2);
    printCalculations(result);
}

void sjf(char *s1, char *s2, int x1, int y1, int z1,
         int x2, int y2, int z2)
{
    struct CalculationResult *result;
    int i;              /* next string position (time) */
    int state1 = READY; /* start with both ready */
    int state2 = READY;
    int cpuLeft1 = x1; /* P1 next CPU burst remaining */
    int cpuLeft2 = x2; /* P2 next CPU burst remaining */
    int ioLeft1 = y1;  /* P1 next IO burst remaining, 0 if no more IO */
    int ioLeft2 = y2;  /* P2 next IO burst remaining, 0 if no more IO */

    for (i = 0; (state1 != DONE) || (state2 != DONE); i++)
    {
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
            if (cpuLeft1 < cpuLeft2)
            {
                state1 = RUNNING;
            }
            else if (cpuLeft1 > cpuLeft2)
            {
                state2 = RUNNING;
            }
            else
            {
                state1 = RUNNING; /* In case of tie lower PID goes first */
            }
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
    printf("Scheduler SJF:\n");
    for (i = 0; s1[i] != '\0'; i++)
    {
        printf("%c", s1[i]);
    }
    printf("\n");
    for (i = 0; s2[i] != '\0'; i++)
    {
        printf("%c", s2[i]);
    }
    printf("\n");
    result = calculate(s1, s2);
    printCalculations(result);
}

void fcfs(char *s1, char *s2, int x1, int y1, int z1,
          int x2, int y2, int z2)
{
    struct CalculationResult *result;
    int i;              /* next string position (time) */
    int state1 = READY; /* start with both ready */
    int state2 = READY; /* start with both ready */
    int cpuLeft1 = x1;  /* P1 next CPU burst remaining */
    int cpuLeft2 = x2;  /* P2 next CPU burst remaining */
    int ioLeft1 = y1;   /* P1 next IO burst remaining, 0 if no more IO */
    int ioLeft2 = y2;   /* P2 next IO burst remaining, 0 if no more IO */

    for (i = 0; (state1 != DONE) || (state2 != DONE); i++)
    {

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

    printf("Scheduler FCFS:\n");
    for (i = 0; s1[i] != '\0'; i++)
    {
        printf("%c", s1[i]);
    }
    printf("\n");
    for (i = 0; s2[i] != '\0'; i++)
    {
        printf("%c", s2[i]);
    }
    printf("\n");
    result = calculate(s1, s2);
    printCalculations(result);
}

void printCalculations(struct CalculationResult *result)
{
    printf("%d %d %.1f %.5f \n", result->wait_count1, result->wait_count2, result->avg_wait_time, result->cpu_ut);
}

CalculationResult *calculate(char *s1, char *s2)
{
    int i, j;
    CalculationResult *result = malloc(sizeof(CalculationResult));
    size_t length1 = strlen(s1);
    size_t length2 = strlen(s2);
    int longest;

    int wait_count1 = 0;
    int running_count1 = 0;
    int ready_count1 = 0;

    int wait_count2 = 0;
    int running_count2 = 0;
    int ready_count2 = 0;

    char wait = 'w';
    char running = 'R';
    char ready = 'r';

    if (length1 > length2)
    {
        longest = length1;
    }
    else if (length1 < length2)
    {
        longest = length2;
    }
    else
    {
        longest = length1;
    }

    for (i = 0; s1[i] != '\0'; i++)
    {
        if (s1[i] == wait)
        {
            wait_count1++;
        }
        else if (s1[i] == running)
        {
            running_count1++;
        }
        else if (s1[i] == ready)
        {
            ready_count1++;
        }
    }

    for (j = 0; s2[j] != '\0'; j++)
    {
        if (s2[j] == wait)
        {
            wait_count2++;
        }
        else if (s2[j] == running)
        {
            running_count2++;
        }
        else if (s2[j] == ready)
        {
            ready_count2++;
        }
    }

    result->avg_wait_time = (float)(ready_count1 + ready_count2) / 2;
    result->cpu_ut = (float)(running_count1 + running_count2) / longest;
    result->wait_count1 = wait_count1 - 1;
    result->wait_count2 = wait_count2 - 1;

    return result;

    /*
    the first two values are integers that indicate the waiting time for the first and second processes
    (i.e., the number of times r occurs in the first and second string), respectively. The third value
    is the average waiting time (i.e., average of the two integers) and the last one indicates the CPU
    utilization (i.e., the total number of R’s in the two strings divided by the length of the longer string).
    */
}