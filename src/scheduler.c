#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "scheduler.h"
#include "process.h"

void add_job(Job jobs[], int *job_count, int max_jobs)
{
    if (*job_count >= max_jobs)
    {
        printf("\nMaximum number of jobs reached.\n");
        return;
    }

    Job *job = &jobs[*job_count];

    job->id = *job_count + 1;
    job->pid = 0;
    job->state = SCHEDULED;

    printf("\nEnter delay in seconds: ");
    scanf("%d", &job->delay);

    while (getchar() != '\n')
        ;

    printf("Enter command: ");
    fgets(job->command, MAX_COMMAND_LENGTH, stdin);

    job->command[strcspn(job->command, "\n")] = '\0';

    (*job_count)++;

    printf("\nJob added successfully.\n");
    printf("Job ID : %d\n", job->id);
    printf("Delay  : %d seconds\n", job->delay);
    printf("Command: %s\n", job->command);
}

void list_jobs(Job jobs[], int job_count)
{
    int i;

    if (job_count == 0)
    {
        printf("\nNo jobs available.\n");
        return;
    }

    printf("\n============== Job List ==============\n");
    printf("%-5s %-10s %-8s %-12s %s\n",
           "ID", "PID", "DELAY", "STATE", "COMMAND");

    for (i = 0; i < job_count; i++)
    {
        char *state;

        switch (jobs[i].state)
        {
            case SCHEDULED:
                state = "SCHEDULED";
                break;

            case RUNNING:
                state = "RUNNING";
                break;

            case COMPLETED:
                state = "COMPLETED";
                break;

            case FAILED:
                state = "FAILED";
                break;

            case CANCELLED:
                state = "CANCELLED";
                break;

            default:
                state = "UNKNOWN";
        }

        printf("%-5d %-10d %-8d %-12s %s\n",
               jobs[i].id,
               jobs[i].pid,
               jobs[i].delay,
               state,
               jobs[i].command);
    }

    printf("=======================================\n");
}

void run_job(Job *job)
{
    if (job == NULL)
    {
        printf("Invalid job.\n");
        return;
    }

    if (job->state != SCHEDULED)
    {
        printf("\nJob %d cannot be run.\n", job->id);
        printf("Current state: ");

        switch (job->state)
        {
            case RUNNING:
                printf("RUNNING\n");
                break;

            case COMPLETED:
                printf("COMPLETED\n");
                break;

            case FAILED:
                printf("FAILED\n");
                break;

            case CANCELLED:
                printf("CANCELLED\n");
                break;

            default:
                printf("UNKNOWN\n");
        }

        return;
    }

    printf("\n[CronD] Job %d selected.\n", job->id);

    printf("[CronD] Waiting %d seconds before execution...\n",
           job->delay);

    sleep(job->delay);

    printf("[CronD] Scheduled time reached.\n");

    start_process(job);
}
