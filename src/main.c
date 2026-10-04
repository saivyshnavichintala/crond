#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "crond.h"
#include "scheduler.h"

int main(void)
{
    int delay;
    char command[MAX_COMMAND_LENGTH];

    printf("=========================================\n");
    printf("       CronD - Job Scheduler Daemon      \n");
    printf("=========================================\n");

    printf("\nCronD started.\n");

    while (1)
    {
        printf("\nEnter delay in seconds (0 to exit): ");
        scanf("%d", &delay);

        if (delay == 0)
        {
            printf("\nCronD shutting down...\n");
            break;
        }

        getchar();

        printf("Enter command: ");

        fgets(
            command,
            sizeof(command),
            stdin
        );

        command[
            strcspn(command, "\n")
        ] = '\0';

        schedule_job(
            delay,
            command
        );
    }

    return 0;
}
