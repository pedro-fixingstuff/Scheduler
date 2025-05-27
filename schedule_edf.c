#include <pthread.h>
#include <stdio.h>
#include "CPU.h"
#include "schedule_edf.h"

// add a task to the list 
void add(char *name, int burst, int deadline) {
   struct task newTask;
   newTask.name = name;
   newTask.tid = nextTid;
   newTask.priority = -1;
   newTask.burst = burst;
   newTask.deadline = deadline;

   nextTid++;

   struct node *listEnd = end(taskList);
   insert(&listEnd, &newTask);
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
   }
}
