#ifndef PROCESS_H
#define PROCESS_H

#include "crond.h"

int start_process(Job *job);
int cancel_process(Job *job);
void check_processes(Job jobs[], int job_count);

#endif
