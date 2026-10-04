#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "ipc.h"

int create_fifo(void)
{
    unlink(CROND_FIFO);

    if (mkfifo(CROND_FIFO, 0666) == -1)
    {
        perror("mkfifo");

        return -1;
    }

    return 0;
}


int send_command(const char *command)
{
    int fd;

    fd = open(
        CROND_FIFO,
        O_WRONLY
    );

    if (fd == -1)
    {
        perror(
            "Unable to connect to CronD"
        );

        return -1;
    }

    if (
        write(
            fd,
            command,
            strlen(command)
        ) == -1
    )
    {
        perror("write");

        close(fd);

        return -1;
    }

    close(fd);

    return 0;
}
