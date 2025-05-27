#include <pthread.h>
#include <stdio.h>
#include "CPU.h"
#include "schedule_rr.h"

// add a task to the list 
void add(char *name, int burst) {
   struct task newTask;
   newTask.name = name;
   newTask.tid = nextTid;
   newTask.priority = -1;
   newTask.burst = burst;
   newTask.deadline = -1;

   nextTid++;

   struct node *listEnd = end(taskList);
   insert(&listEnd, &newTask);
}

// invoke the scheduler
void schedule(){
   pthread_t timer_tid;
   ThreadArgs args;

   while (taskList != NULL) {
      // get the task from the head of the list
      Task *task = taskList->task;

      // remove the task from the list and run it
      delete(&taskList, task);

      args.task = task;
      args.slice = QUANTUM;
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
