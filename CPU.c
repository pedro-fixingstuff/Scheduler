/**
 * "Virtual" CPU that also maintains track of system time.
 */

#include <stdio.h>

#include "CPU.h"

// run this task for the specified time slice
void *run(Task *task, int slice) {
    if (slice == NULL) {
        slice = QUANTUM; // default time slice if not specified
    }

    printf("Running task = [%s] [%d] [%d] for %d units.\n",task->name, task->priority, task->burst, slice);

    if (task->burst > slice) {
        task->burst -= slice; // reduce the burst time by the time slice
    } else {
        task->burst = 0; // task is completed
    }
}
