#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>
#include <errno.h>

#include "ipc.h"

/* =========================================================
   SIGNAL HANDLING
   ========================================================= */

volatile sig_atomic_t signal_received = 0;

void handle_sigchld(int signal_number)
{
    signal_received = signal_number;

    printf("\n[IPC] SIGCHLD received.\n");
    fflush(stdout);
}

void handle_sigusr1(int signal_number)
{
    signal_received = signal_number;

    printf("\n[IPC] SIGUSR1 received.\n");
    fflush(stdout);
}

void handle_sigterm(int signal_number)
{
    signal_received = signal_number;

    printf("\n[IPC] SIGTERM received.\n");
    fflush(stdout);
}

void setup_signal_handlers(void)
{
    struct sigaction sa;

    memset(&sa, 0, sizeof(sa));

    sigemptyset(&sa.sa_mask);

    sa.sa_flags = 0;

    /* SIGCHLD */
    sa.sa_handler = handle_sigchld;

    if (sigaction(SIGCHLD, &sa, NULL) == -1)
    {
        perror("sigaction SIGCHLD");
    }

    /* SIGUSR1 */
    sa.sa_handler = handle_sigusr1;

    if (sigaction(SIGUSR1, &sa, NULL) == -1)
    {
        perror("sigaction SIGUSR1");
    }

    /* SIGTERM */
    sa.sa_handler = handle_sigterm;

    if (sigaction(SIGTERM, &sa, NULL) == -1)
    {
        perror("sigaction SIGTERM");
    }

    printf("[IPC] Signal handlers installed.\n");
}

void send_job_signal(pid_t pid, int signal_number)
{
    if (kill(pid, signal_number) == -1)
    {
        perror("[IPC] kill");
        return;
    }

    printf(
        "[IPC] Signal %d sent to PID %d.\n",
        signal_number,
        pid
    );
}


/* =========================================================
   ANONYMOUS PIPE
   ========================================================= */

int create_anonymous_pipe(void)
{
    int pipe_fd[2];
    pid_t pid;

    if (pipe(pipe_fd) == -1)
    {
        perror("[IPC] pipe");
        return -1;
    }

    pid = fork();

    if (pid < 0)
    {
        perror("[IPC] fork");

        close(pipe_fd[0]);
        close(pipe_fd[1]);

        return -1;
    }

    if (pid == 0)
    {
        char buffer[256];

        /* Child reads */
        close(pipe_fd[1]);

        ssize_t bytes = read(
            pipe_fd[0],
            buffer,
            sizeof(buffer) - 1
        );

        if (bytes < 0)
        {
            perror("[Child] pipe read");

            close(pipe_fd[0]);

            exit(EXIT_FAILURE);
        }

        buffer[bytes] = '\0';

        printf(
            "\n[Child] Received through anonymous pipe:\n"
        );

        printf(
            "%s\n",
            buffer
        );

        close(pipe_fd[0]);

        exit(EXIT_SUCCESS);
    }

    /* Parent writes */
    close(pipe_fd[0]);

    const char *message =
        "Hello from CronD parent through anonymous pipe.";

    if (write(
            pipe_fd[1],
            message,
            strlen(message) + 1
        ) == -1)
    {
        perror("[Parent] pipe write");

        close(pipe_fd[1]);

        return -1;
    }

    close(pipe_fd[1]);

    /*
     * wait() is declared in <sys/wait.h>
     */
    if (wait(NULL) == -1)
    {
        perror("[IPC] wait");
        return -1;
    }

    printf(
        "[IPC] Anonymous pipe communication completed.\n"
    );

    return 0;
}


/* =========================================================
   NAMED PIPE / FIFO
   ========================================================= */

int create_fifo(const char *fifo_path)
{
    if (mkfifo(fifo_path, 0666) == -1)
    {
        if (errno != EEXIST)
        {
            perror("[IPC] mkfifo");
            return -1;
        }
    }

    printf(
        "[IPC] FIFO created: %s\n",
        fifo_path
    );

    return 0;
}

int write_fifo(
    const char *fifo_path,
    const char *message
)
{
    int fd;

    fd = open(
        fifo_path,
        O_WRONLY
    );

    if (fd == -1)
    {
        perror(
            "[IPC] FIFO open for writing"
        );

        return -1;
    }

    if (write(
            fd,
            message,
            strlen(message)
        ) == -1)
    {
        perror(
            "[IPC] FIFO write"
        );

        close(fd);

        return -1;
    }

    close(fd);

    printf(
        "[IPC] Message written to FIFO.\n"
    );

    return 0;
}

int read_fifo(const char *fifo_path)
{
    int fd;

    char buffer[256];

    fd = open(
        fifo_path,
        O_RDONLY
    );

    if (fd == -1)
    {
        perror(
            "[IPC] FIFO open for reading"
        );

        return -1;
    }

    ssize_t bytes = read(
        fd,
        buffer,
        sizeof(buffer) - 1
    );

    if (bytes < 0)
    {
        perror(
            "[IPC] FIFO read"
        );

        close(fd);

        return -1;
    }

    buffer[bytes] = '\0';

    printf(
        "[IPC] FIFO received: %s\n",
        buffer
    );

    close(fd);

    return 0;
}


/* =========================================================
   PROCESS GROUPS
   ========================================================= */

int create_process_group(pid_t pid)
{
    if (setpgid(pid, pid) == -1)
    {
        perror(
            "[IPC] setpgid"
        );

        return -1;
    }

    printf(
        "[IPC] Process group created. PGID = %d\n",
        pid
    );

    return 0;
}

int send_group_signal(
    pid_t pgid,
    int signal_number
)
{
    /*
     * Negative PGID sends signal to the
     * complete process group.
     */
    if (kill(-pgid, signal_number) == -1)
    {
        perror(
            "[IPC] group signal"
        );

        return -1;
    }

    printf(
        "[IPC] Signal %d sent to process group %d.\n",
        signal_number,
        pgid
    );

    return 0;
}


/* =========================================================
   SESSIONS
   ========================================================= */

int create_new_session(void)
{
    pid_t pid;

    pid = fork();

    if (pid < 0)
    {
        perror("[IPC] fork");

        return -1;
    }

    /*
     * Parent
     */
    if (pid > 0)
    {
        printf(
            "[IPC] Session child created: PID %d\n",
            pid
        );

        return 1;
    }

    /*
     * Child creates a new session.
     */
    if (setsid() == -1)
    {
        perror(
            "[IPC] setsid"
        );

        exit(EXIT_FAILURE);
    }

    printf(
        "[IPC] New session created.\n"
    );

    printf(
        "[IPC] PID  : %d\n",
        getpid()
    );

    printf(
        "[IPC] PGID : %d\n",
        getpgrp()
    );

    printf(
        "[IPC] SID  : %d\n",
        getsid(0)
    );

    exit(EXIT_SUCCESS);
}
