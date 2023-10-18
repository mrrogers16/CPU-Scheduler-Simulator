#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define READY 0
#define RUNNING 1
#define WAITING 2
#define DONE 3

static char stateChars[] = {'r', 'R', 'w', '\0'};


typedef struct {
    int avg_wait_time;
    int cpu_ut;
    int wait_count1;
    int wait_count2;
} CalculationResult;

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

CalculationResult* calculate(char *s1, char *s2)
{
    int i;
    CalculationResult* result = malloc(sizeof(CalculationResult));
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
    else
    {
        longest = length2;
    }

    for (i = 0; i != '\0'; i++)
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

    for (i = 0; i != '\0'; i++)
    {
        if (s2[i] == wait)
        {
            wait_count2++;
        }
        else if (s2[i] == running)
        {
            running_count2++;
        }
        else if (s2[i] == ready)
        {
            ready_count2++;
        }
    }

    int avg_wait_time = (ready_count1 + ready_count2) / 2;
    result->avg_wait_time = avg_wait_time;
    int cpu_ut = (running_count1 + running_count2) / longest;
    result->cpu_ut = cpu_ut;
    result->wait_count1 = wait_count1;
    result->wait_count2 = wait_count2;

    return result;

    /*
    the first two values are integers that indicate the waiting time for the first and second processes
    (i.e., the number of times r occurs in the first and second string), respectively. The third value
    is the average waiting time (i.e., average of the two integers) and the last one indicates the CPU
    utilization (i.e., the total number of R’s in the two strings divided by the length of the longer string).
    */
}

void psjf(char *s1, char *s2, int x1, int y1, int z1,
          int x2, int y2, int z2)
{
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

    printf("PSJF\nS1: ");
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

void sjf(char *s1, char *s2, int x1, int y1, int z1,
         int x2, int y2, int z2)
{
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
    printf("SJF\nS1: ");
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

void fcfs(char *s1, char *s2, int x1, int y1, int z1,
          int x2, int y2, int z2)
{
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

    printf("FCFS\nS1: ");
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
