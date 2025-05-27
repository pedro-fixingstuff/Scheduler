#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

#include <stdio.h>
#include "list.h"

struct node *taskList = NULL;

int nextTid = 0; // id of the next task to be added

// add a task to the list 
void add(char *name, int burst);

// invoke the scheduler
void schedule();
