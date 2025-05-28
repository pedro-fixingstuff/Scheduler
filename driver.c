/**
 * Driver.c
 *
 * Schedule is in the format
 *
 *  [name] [priority] [CPU burst] [deadline]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schedulers.h"

#define SIZE    100

int main(int argc, char *argv[])
{
    FILE *in;
    char *line;
    char *temp;
    char task[SIZE];

    char *name;
    int priority;
    int burst;
    int deadline;

    in = fopen(argv[1],"r");
    
    while (fgets(task,SIZE,in) != NULL) {
        // copy the task line to save the pointer for later
        line = strdup(task);
        temp = line;

        name = strsep(&temp,",");
        priority = atoi(strsep(&temp,","));
        burst = atoi(strsep(&temp,","));
        deadline = atoi(strsep(&temp, ","));

        // add the task to the scheduler's list of tasks
        add(name, priority, burst, deadline);

        free(line);
    }

    fclose(in);

    // invoke the scheduler
    schedule();

    return 0;
}
