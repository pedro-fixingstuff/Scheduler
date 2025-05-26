#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

#include "list.h"

// list of lists structure for different priorities
struct node *taskLists[MAX_PRIORITY - MIN_PRIORITY + 1] = {NULL};

int nextTid = 0; // id of the next task to be added

// add a task to the list 
void add(char *name, int priority, int burst);

// invoke the scheduler
void schedule();
