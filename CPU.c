/**
 * "Virtual" CPU that also maintains track of system time.
 */

#include <stdio.h>

#include "CPU.h"

// run this task for the specified time slice
void *run(void *args) {
    // cast arguments
    ThreadArgs *threadArgs = (ThreadArgs *)args;
    Task *task = threadArgs->task;
    int slice = threadArgs->slice;

    if (task->burst < slice) {
        slice = task->burst; // if the slice is larger than the burst time, run for the burst time
    }

    printf("Running task = [%s] [%d] [%d] for %d units.\n",task->name, task->priority, task->burst, slice);

    if (task->burst > slice) {
        task->burst -= slice; // reduce the burst time by the time slice
    } else {
        task->burst = 0; // task is completed
    }

    return NULL;
}
