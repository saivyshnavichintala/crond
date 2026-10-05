#ifndef CROND_H
#define CROND_H

#include <sys/types.h>

#define MAX_JOBS 100
#define MAX_COMMAND_LENGTH 256

typedef enum
{
    SCHEDULED,
    RUNNING,
    COMPLETED,
    FAILED,
    CANCELLED
} JobState;

typedef struct
{
    int id;
    pid_t pid;
    int delay;
    JobState state;
    char command[MAX_COMMAND_LENGTH];

    pid_t process_group;
} Job;

#endif
