#include <pthread.h>
#include <stdio.h>
#include "schedule_rr.h"
#include "list.h"

// add a task to the list 
void add(char *name, int burst) {
   struct task newTask;
   newTask.name = name;
   newTask.tid = nextTid;
   newTask.priority = NULL;
   newTask.burst = burst;
   newTask.deadline = NULL;

   nextTid++;

   struct node *listEnd = end(taskList);
   insert(listEnd, &newTask);
}

// invoke the scheduler
void schedule(){
   pthread_t timer_tid;

   while (taskList != NULL) {
      // get the task from the head of the list
      Task *task = taskList->task;

      // remove the task from the list and run it
      delete(&taskList, task);
      if (pthread_create(&timer_tid, NULL, run(), task) != 0) {
         perror("Error: Failed to create timer thread.");
         return;
      }
      pthread_join(timer_tid, NULL); // wait for the timer thread to finish

      if (task->burst > 0) {
         // task is not completed, reinsert it at the end of the list
         struct node *listEnd = end(taskList);
         insert(listEnd, task);
      }
   }
}
