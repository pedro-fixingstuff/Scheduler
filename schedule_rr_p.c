#include "schedulers.h"

// list of lists structure for different priorities
struct node *taskLists[MAX_PRIORITY - MIN_PRIORITY + 1] = {NULL};
int nextTid = 0;

// add a task to the list 
void add(char *name, int priority, int burst, int deadline) {
   if (priority < MIN_PRIORITY || priority > MAX_PRIORITY) {
      fprintf(stderr, "Error: Priority [%d] for the task [%s] out of range (%d-%d).\n",
              priority, name, MIN_PRIORITY, MAX_PRIORITY);
      return;
   }

   Task *newTask = (Task *)malloc(sizeof(Task));
   if (newTask == NULL) {
      fprintf(stderr, "Error: Failed to allocate memory for task [%s].\n", name);
      return;
   }

   newTask->name = strdup(name);
   if (newTask->name == NULL) {
      fprintf(stderr, "Error: Failed to assign name to task [%s].\n", name);
      free(newTask);
      return;
   }

   newTask->tid = nextTid;
   newTask->priority = priority;
   newTask->burst = burst;
   newTask->deadline = deadline;

   nextTid++;

   // insert the new task into the appropriate priority list
   int priority_index = priority - MIN_PRIORITY;

   insert(&taskLists[priority_index], newTask);
}

// invoke the scheduler
void schedule(){
   pthread_t timer_tid;
   ThreadArgs args;

   while (1) {
      struct node **taskList;
      int hasTasks = 0;
      // get the task from the highest-priority list with tasks
      for (int i = 0; i <= (MAX_PRIORITY - MIN_PRIORITY); i++) {
         if (taskLists[i] != NULL) {
            taskList = &taskLists[i];
            hasTasks = 1;
            break;
         }
      }
      if (!hasTasks) {
         return; // no tasks to run, exit the scheduler
      }

      // get the task from the end of the list
      struct node *listEnd = end(*taskList);
      Task *task = listEnd->task;

      // remove the task from the list and run it
      delete(taskList, task);

      args.task = task;
      args.slice = QUANTUM;
      if (pthread_create(&timer_tid, NULL, run, &args) != 0) {
         perror("Error: Failed to create timer thread.");
         return;
      }
      pthread_join(timer_tid, NULL); // wait for the timer thread to finish

      if (task->burst > 0) {
         // task is not completed, reinsert it at the head of the list
         insert(taskList, task);
      }
      else {
         // task is completed, free its resources
         free(task->name);
         free(task);
      }
   }
}
