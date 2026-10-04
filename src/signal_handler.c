#include <signal.h>

#include "signal_handler.h"

static volatile sig_atomic_t stop_flag = 0;


static void handle_sigterm(
    int signal_number
)
{
    (void)signal_number;

    stop_flag = 1;
}


static void handle_sigint(
    int signal_number
)
{
    (void)signal_number;

    stop_flag = 1;
}


void setup_signal_handlers(void)
{
    signal(
        SIGTERM,
        handle_sigterm
    );

    signal(
        SIGINT,
        handle_sigint
    );
}


int shutdown_requested(void)
{
    return stop_flag;
}
