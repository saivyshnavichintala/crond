#ifndef CROND_H
#define CROND_H

#include <sys/types.h>

#define MAX_JOBS 50
#define MAX_COMMAND_LENGTH 256

typedef enum
{
    CREATED,
    SCHEDULED,
    RUNNING,
    COMPLETED,
    FAILED,
    CANCELLED
} JobState;

typedef struct
{
    int id;
    char command[MAX_COMMAND_LENGTH];
    int delay;
    pid_t pid;
    JobState state;
} Job;

const char *state_to_string(JobState state);

#endif
