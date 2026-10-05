#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t received = 0;

void signal_handler(int signal_number)
{
    received = signal_number;

    printf(
        "\nSignal received: %d\n",
        signal_number
    );

    if (
        signal_number == SIGUSR1
    )
    {
        printf(
            "SIGUSR1 received successfully.\n"
        );
    }

    if (
        signal_number == SIGTERM
    )
    {
        printf(
            "SIGTERM received. Exiting.\n"
        );

        exit(0);
    }
}

int main()
{
    struct sigaction sa;

    sa.sa_handler =
        signal_handler;

    sigemptyset(
        &sa.sa_mask
    );

    sa.sa_flags = 0;

    sigaction(
        SIGUSR1,
        &sa,
        NULL
    );

    sigaction(
        SIGTERM,
        &sa,
        NULL
    );

    printf(
        "Signal test process started.\n"
    );

    printf(
        "PID: %d\n",
        getpid()
    );

    printf(
        "Send SIGUSR1 using:\n"
    );

    printf(
        "kill -SIGUSR1 %d\n",
        getpid()
    );

    printf(
        "Send SIGTERM using:\n"
    );

    printf(
        "kill -SIGTERM %d\n",
        getpid()
    );

    while (1)
    {
        pause();
    }

    return 0;
}
