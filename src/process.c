#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

#include "process.h"

int start_process(Job *job)
{
    pid_t pid;
    int status;

    fflush(stdout);

    pid = fork();

    if (pid < 0)
    {
        perror("[CronD] fork");

        job->state = FAILED;

        return -1;
    }

    if (pid == 0)
    {
        char command_copy[MAX_COMMAND_LENGTH];
        char *args[64];
        int count = 0;

        strcpy(command_copy, job->command);

        char *token = strtok(command_copy, " ");

        while (token != NULL && count < 63)
        {
            args[count] = token;
            count++;

            token = strtok(NULL, " ");
        }

        args[count] = NULL;

        printf("\n[Child] Job ID: %d\n", job->id);
        printf("[Child] PID: %d\n", getpid());
        printf("[Child] Executing: %s\n", job->command);

        fflush(stdout);

        if (count == 0)
        {
            fprintf(stderr, "[Child] Empty command.\n");
            exit(EXIT_FAILURE);
        }

        execvp(args[0], args);

        perror("[Child] execvp");

        exit(EXIT_FAILURE);
    }

    job->pid = pid;
    job->state = RUNNING;

    printf("\n[CronD] Job %d started.\n", job->id);
    printf("[CronD] Child PID: %d\n", job->pid);

    fflush(stdout);

    /*
     * Wait for the child process to finish.
     * This keeps the terminal output clean and prevents
     * the parent menu from accepting input while the
     * child is executing.
     */
    if (waitpid(pid, &status, 0) == -1)
    {
        perror("[CronD] waitpid");

        job->state = FAILED;

        return -1;
    }

    if (WIFEXITED(status))
    {
        if (WEXITSTATUS(status) == 0)
        {
            job->state = COMPLETED;

            printf("\n[CronD] Job %d completed successfully.\n",
                   job->id);
        }
        else
        {
            job->state = FAILED;

            printf("\n[CronD] Job %d failed.\n",
                   job->id);
        }
    }
    else if (WIFSIGNALED(status))
    {
        job->state = FAILED;

        printf("\n[CronD] Job %d terminated by signal %d.\n",
               job->id,
               WTERMSIG(status));
    }
    else
    {
        job->state = FAILED;

        printf("\n[CronD] Job %d failed.\n",
               job->id);
    }

    return 0;
}

int cancel_process(Job *job)
{
    if (job == NULL)
    {
        return -1;
    }

    if (job->state != RUNNING)
    {
        printf("\nJob %d is not currently running.\n",
               job->id);

        return -1;
    }

    if (kill(job->pid, SIGTERM) == -1)
    {
        perror("[CronD] kill");

        return -1;
    }

    printf("\n[CronD] Termination signal sent to Job %d.\n",
           job->id);

    waitpid(job->pid, NULL, 0);

    job->state = CANCELLED;

    printf("[CronD] Job %d cancelled.\n",
           job->id);

    return 0;
}

void check_processes(Job jobs[], int job_count)
{
    int i;

    for (i = 0; i < job_count; i++)
    {
        if (jobs[i].state == RUNNING)
        {
            int status;

            pid_t result = waitpid(
                jobs[i].pid,
                &status,
                WNOHANG
            );

            if (result == 0)
            {
                printf("Job %d is still running.\n",
                       jobs[i].id);
            }
            else if (result == jobs[i].pid)
            {
                if (WIFEXITED(status) &&
                    WEXITSTATUS(status) == 0)
                {
                    jobs[i].state = COMPLETED;
                }
                else
                {
                    jobs[i].state = FAILED;
                }
            }
            else
            {
                jobs[i].state = FAILED;
            }
        }
    }
}
