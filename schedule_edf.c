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

   struct node *listEnd = end(taskList);
   insert(&listEnd, newTask);
}

// invoke the scheduler
void schedule(){
   pthread_t timer_tid;
   ThreadArgs args;
   args.slice = -1; // not used

   while (taskList != NULL) {
      struct node *node = taskList;

      // find the task with the earliest deadline
      while (node->next != NULL) {
         struct node *nextNode = node->next;
         if (nextNode->task->deadline < node->task->deadline) {
            node = nextNode;
         }
      }

      Task *task = node->task;

      // remove the task from the list and run it
      delete(&taskList, task);

      args.task = task;
      if (pthread_create(&timer_tid, NULL, run, &args) != 0) {
         perror("Error: Failed to create timer thread.");
         return;
      }
      pthread_join(timer_tid, NULL); // wait for the timer thread to finish

      if (task->burst > 0) {
         // task is not completed, reinsert it at the end of the list
         struct node *listEnd = end(taskList);
         insert(&listEnd, task);
      }
   }
}
