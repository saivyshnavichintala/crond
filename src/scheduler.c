#include <stdio.h>
#include <unistd.h>

#include "scheduler.h"
#include "process.h"

void schedule_job(int delay, const char *command)
{
    printf("\n[CronD] Job scheduled.\n");
    printf("[CronD] Delay: %d seconds\n", delay);
    printf("[CronD] Command: %s\n", command);

    printf("[CronD] Waiting...\n");

    sleep(delay);

    printf("\n[CronD] Scheduled time reached.\n");
    printf("[CronD] Executing job...\n");

    execute_command(command);
}
