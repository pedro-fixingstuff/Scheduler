/**
 * Driver.c
 *
 * Schedule is in the format
 *
 *  [name] [priority] [CPU burst]
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "schedule_rr.h"
#include "schedule_rr_p.h"
#include "schedule_edf.h"
#include "schedule_pa.h"

#define SIZE    100

int main(int argc, char *argv[])
{
    FILE *in;
    char *temp;
    char task[SIZE];

    char *name;
    int priority;
    int burst;
    int deadline;

    in = fopen(argv[1],"r");
    
    while (fgets(task,SIZE,in) != NULL) {
        temp = strdup(task);
        name = strsep(&temp,",");
        priority = atoi(strsep(&temp,","));
        burst = atoi(strsep(&temp,","));
        //Only to EDF algorithm
        deadline = atoi(strsep(&temp, ","));

        // add the task to the scheduler's list of tasks
        add(name,priority,burst);
        //to EDF only
        //add(name,priority,burst, deadline);

        free(temp);
    }

    fclose(in);

    // invoke the scheduler
    schedule();

    return 0;
}
