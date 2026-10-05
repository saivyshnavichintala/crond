#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

#define FIFO_PATH "/tmp/crond_test_fifo"

int main()
{
    int fd;

    char buffer[256];

    /*
     * Create FIFO.
     */
    if (
        mkfifo(
            FIFO_PATH,
            0666
        ) == -1
    )
    {
        perror(
            "mkfifo"
        );
    }

    printf(
        "FIFO created: %s\n",
        FIFO_PATH
    );

    printf(
        "Open another terminal and run:\n"
    );

    printf(
        "echo \"Hello CronD\" > %s\n",
        FIFO_PATH
    );

    printf(
        "Waiting for message...\n"
    );

    fd = open(
        FIFO_PATH,
        O_RDONLY
    );

    if (fd == -1)
    {
        perror(
            "open"
        );

        return 1;
    }

    int bytes =
        read(
            fd,
            buffer,
            sizeof(buffer) - 1
        );

    if (bytes > 0)
    {
        buffer[bytes] = '\0';

        printf(
            "Received: %s\n",
            buffer
        );
    }

    close(fd);

    unlink(
        FIFO_PATH
    );

    return 0;
}
