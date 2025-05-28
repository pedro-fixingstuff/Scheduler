#include "schedulers.h"

struct node *taskList = NULL;
int nextTid = 0;

// add a task to the list 
void add(char *name, int priority, int burst, int deadline) {
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

   insert(&taskList, newTask);
}

// invoke the scheduler
void schedule(){
   pthread_t timer_tid;
   ThreadArgs args;
   args.slice = -1; // not used

   while (taskList != NULL) {
      // find the task with the earliest deadline
      struct node *earliestNode = taskList;
      struct node *temp = taskList;
      while (temp != NULL) {
         if (temp->task->deadline <= earliestNode->task->deadline) {
            earliestNode = temp;
         }
         temp = temp->next;
      }

      Task *task = earliestNode->task;

      // remove the task from the list and run it
      delete(&taskList, task);

      args.task = task;
      if (pthread_create(&timer_tid, NULL, run, &args) != 0) {
         perror("Error: Failed to create timer thread.");
         return;
      }
      pthread_join(timer_tid, NULL); // wait for the timer thread to finish

      // task is completed, free its resources
      free(task->name);
      free(task);
   }
}
