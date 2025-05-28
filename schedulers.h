#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CPU.h"
#include "list.h"

#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

// add a task to the list 
void add(char *name, int priority, int burst, int deadline);

// invoke the scheduler
void schedule();
