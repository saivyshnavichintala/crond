#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int pipe_fd[2];

    pid_t pid;

    if (
        pipe(pipe_fd) == -1
    )
    {
        perror("pipe");

        return 1;
    }

    pid = fork();

    if (pid < 0)
    {
        perror("fork");

        return 1;
    }

    if (pid == 0)
    {
        char buffer[100];

        close(
            pipe_fd[1]
        );

        read(
            pipe_fd[0],
            buffer,
            sizeof(buffer)
        );

        printf(
            "[Child] Message received: %s\n",
            buffer
        );

        close(
            pipe_fd[0]
        );

        exit(0);
    }

    close(
        pipe_fd[0]
    );

    char message[] =
        "Hello Child!";

    write(
        pipe_fd[1],
        message,
        strlen(message) + 1
    );

    close(
        pipe_fd[1]
    );

    wait(NULL);

    printf(
        "[Parent] Anonymous pipe test completed.\n"
    );

    return 0;
}
