#ifndef TASKS_H
#define TASKS_H

// Declaración de las tareas usadas por FreeRTOS.
// Este header permite que main.cpp conozca las firmas sin necesidad
// de incluir la implementación completa.

void taskSensor(void *parameter);
void taskSD(void *parameter);
void taskWatchdog(void *parameter);

#endif