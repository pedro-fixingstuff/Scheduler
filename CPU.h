// length of a time quantum
#define QUANTUM 10
#include "task.h"

typedef struct {
    Task *task; // task to run
    int slice;  // time slice for the task
} ThreadArgs;

// run the specified task for the following time slice
void *run(void *args);
