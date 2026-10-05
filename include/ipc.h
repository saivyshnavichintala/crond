#ifndef IPC_H
#define IPC_H

#include "crond.h"

/* Anonymous pipe */
int create_anonymous_pipe(void);

/* Named pipe / FIFO */
int create_fifo(const char *fifo_path);

int write_fifo(
    const char *fifo_path,
    const char *message
);

int read_fifo(
    const char *fifo_path
);

/* Signals */
void setup_signal_handlers(void);

void send_job_signal(
    pid_t pid,
    int signal_number
);

/* Process groups */
int create_process_group(
    pid_t pid
);

int send_group_signal(
    pid_t pgid,
    int signal_number
);

/* Sessions */
int create_new_session(void);

#endif
