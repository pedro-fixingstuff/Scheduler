#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "CPU.h"
#include "schedule_pa.h"

struct node *taskLists[MAX_PRIORITY - MIN_PRIORITY + 1] = {NULL};
int nextTid = 0;

void add(char *name, int priority, int burst) {
    if (priority < MIN_PRIORITY || priority > MAX_PRIORITY) {
        fprintf(stderr, "Erro: Prioridade [%d] para a tarefa [%s] fora do intervalo válido (%d-%d).\n",
                priority, name, MIN_PRIORITY, MAX_PRIORITY);
        return;
    }

    Task *newTask = (Task *)malloc(sizeof(Task));
    if (newTask == NULL) {
        fprintf(stderr, "Erro: Falha ao alocar memória para a nova tarefa [%s].\n", name);
        return;
    }

    newTask->name = strdup(name);
    if (newTask->name == NULL) {
        fprintf(stderr, "Erro: Falha ao duplicar o nome para a tarefa [%s].\n", name);
        free(newTask);
        return;
    }

    newTask->tid = nextTid++;
    newTask->priority = priority;
    newTask->burst = burst;
    newTask->deadline = -1;
    newTask->time_since_last_run = 0;

    int priority_index = priority - MIN_PRIORITY;

    insert(&taskLists[priority_index], newTask);

    // printf("Tarefa [%s] (ID: %d) adicionada com prioridade %d e burst %d na fila de índice %d.\n",
    //        newTask->name, newTask->tid, newTask->priority, newTask->burst, priority_index);
}

void schedule() {
    pthread_t timer_tid;
    ThreadArgs args;
    Task *task_to_run = NULL;
    int original_burst_before_run;
    int actual_time_run;

    printf("\n--- Iniciando Escalonador de Prioridade com Aging ---\n");

    while (1) {
        task_to_run = NULL;
        int current_task_priority_index = -1;

        for (int i = 0; i <= (MAX_PRIORITY - MIN_PRIORITY); i++) {
            if (taskLists[i] != NULL) {
                task_to_run = taskLists[i]->task;
                current_task_priority_index = i;
                break;
            }
        }

        if (task_to_run == NULL) {
            printf("Nenhuma tarefa restante para escalonar. Encerrando o escalonador.\n");
            break;
        }

        delete(&taskLists[current_task_priority_index], task_to_run); //
        task_to_run->time_since_last_run = 0;

        args.task = task_to_run;
        args.slice = QUANTUM;

        original_burst_before_run = task_to_run->burst;

        // printf("Escalonador: Enviando para CPU tarefa [%s], Prio: %d, Burst: %d, Slice: %d\n",
        //        task_to_run->name, task_to_run->priority, original_burst_before_run, args.slice);

        if (pthread_create(&timer_tid, NULL, run, &args) != 0) {
            perror("Erro crítico: Falha ao criar a thread de execução da tarefa.");

            if (task_to_run->name) free(task_to_run->name);
            free(task_to_run);
            return;
        }
        pthread_join(timer_tid, NULL);

        actual_time_run = original_burst_before_run - task_to_run->burst;

        for (int p_idx = 0; p_idx <= (MAX_PRIORITY - MIN_PRIORITY); p_idx++) {
            struct node *current_node = taskLists[p_idx];
            while (current_node != NULL) {
                Task *other_task = current_node->task;

                struct node *next_node_in_iteration = current_node->next;


                other_task->time_since_last_run += actual_time_run;

                if (other_task->time_since_last_run >= AGING_THRESHOLD) {
                    if (other_task->priority > MIN_PRIORITY) {
                        // printf("Aging: Tarefa [%s] (Prio: %d, Espera: %d) atingiu o limiar %d.\n",
                        //        other_task->name, other_task->priority, other_task->time_since_last_run, AGING_THRESHOLD);


                        delete(&taskLists[p_idx], other_task);

                        other_task->priority--;
                        other_task->time_since_last_run = 0;

                        int new_priority_idx = other_task->priority - MIN_PRIORITY;

                        insert(&taskLists[new_priority_idx], other_task);

                        // printf("Aging: Tarefa [%s] promovida para prioridade %d (fila %d).\n",
                        //        other_task->name, other_task->priority, new_priority_idx);
                    }
                }
                current_node = next_node_in_iteration;
            }
        }

        if (task_to_run->burst > 0) {
            // printf("Escalonador: Reenfileirando tarefa [%s], Burst restante: %d, Prio: %d\n",
            //        task_to_run->name, task_to_run->burst, task_to_run->priority);

            insert(&taskLists[task_to_run->priority - MIN_PRIORITY], task_to_run);
        } else {
            // printf("Escalonador: Tarefa [%s] concluída.\n", task_to_run->name);
            if (task_to_run->name) free(task_to_run->name);
            free(task_to_run);
        }
        // printf("--- Fim do ciclo do escalonador ---\n");
    }
}