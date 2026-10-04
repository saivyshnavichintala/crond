#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "crond.h"

void add_job(
    Job jobs[],
    int *job_count,
    int max_jobs
);

void list_jobs(
    Job jobs[],
    int job_count
);

void run_job(Job *job);

#endif
