#include <pthread.h>
#include <stdio.h>
#include "CPU.h"
#include "schedule_rr_p.h"

// add a task to the list 
void add(char *name, int priority, int burst) {
   struct task newTask;
   newTask.name = name;
   newTask.tid = nextTid;
   newTask.priority = priority;
   newTask.burst = burst;
   newTask.deadline = -1;

   nextTid++;

   if (priority < MIN_PRIORITY || priority > MAX_PRIORITY) {
      fprintf(stderr, "Error: Priority out of range.\n");
      return;
   }

   // insert the new task into the appropriate priority list
   struct node *head = taskLists[priority - MIN_PRIORITY];
   struct node *listEnd = end(head);

   insert(&listEnd, &newTask);
}

// invoke the scheduler
void schedule(){
   pthread_t timer_tid;
   ThreadArgs args;

   while (1) {
      struct node *head;
      int hasTasks = 0;
      // get the first task from the highest-priority list with tasks
      for (int i = 0; i < MAX_PRIORITY - MIN_PRIORITY + 1; i++) {
         if (taskLists[i] != NULL) {
            head = taskLists[i];
            hasTasks = 1;
            break;
         }
      }
      if (!hasTasks) {
         return; // no tasks to run, exit the scheduler
      }

      Task *task = head->task;

      // remove the task from the list and run it
      delete(&head, task);

      args.task = task;
      args.slice = QUANTUM;
      if (pthread_create(&timer_tid, NULL, run, &args) != 0) {
         perror("Error: Failed to create timer thread.");
         return;
      }
      pthread_join(timer_tid, NULL); // wait for the timer thread to finish

      if (task->burst > 0) {
         // task is not completed, reinsert it at the end of the list
         struct node *listEnd = end(head);
         insert(&listEnd, task);
      }
   }
}
