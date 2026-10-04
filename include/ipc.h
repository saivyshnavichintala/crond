#ifndef IPC_H
#define IPC_H

#define CROND_FIFO "/tmp/crond_fifo"

int create_fifo(void);

int send_command(const char *command);

#endif
