#ifndef SCHEDULE_PA_H
#define SCHEDULE_PA_H

#define MIN_PRIORITY 1
#define MAX_PRIORITY 10

// Limiar de envelhecimento: Se uma tarefa esperar mais que este tempo
// sem executar, sua prioridade aumenta.
// Exemplo: 150 unidades de tempo. Ajuste conforme necessário.
#define AGING_THRESHOLD 150

#include <stdio.h>
#include "list.h" // Sua implementação de lista
#include "task.h" // Sua struct Task, DEVE incluir time_since_last_run

// Array de listas de tarefas, uma para cada nível de prioridade
extern struct node *taskLists[MAX_PRIORITY - MIN_PRIORITY + 1];

// ID da próxima tarefa a ser adicionada
extern int nextTid;

// Adiciona uma tarefa à lista de prioridade apropriada
void add(char *name, int priority, int burst);

// Invoca o escalonador de Prioridade com Aging
void schedule();

#endif // SCHEDULE_PA_H