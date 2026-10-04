#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#include "process.h"

void execute_command(const char *command)
{
    pid_t pid;
    int status;

    pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return;
    }

    /*
     * Child process
     */
    if (pid == 0)
    {
        char command_copy[256];
        char *args[64];
        int count = 0;

        strcpy(command_copy, command);

        char *token = strtok(command_copy, " ");

        while (token != NULL && count < 63)
        {
            args[count++] = token;
            token = strtok(NULL, " ");
        }

        args[count] = NULL;

        printf("[Child] PID: %d\n", getpid());
        printf("[Child] Executing: %s\n", command);

        execvp(args[0], args);

        perror("execvp");
        exit(EXIT_FAILURE);
    }

    /*
     * Parent process
     */
    printf("[CronD] Child created with PID: %d\n", pid);

    waitpid(pid, &status, 0);

    if (WIFEXITED(status))
    {
        printf(
            "[CronD] Child finished with exit status: %d\n",
            WEXITSTATUS(status)
        );
    }
}
