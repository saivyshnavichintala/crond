#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

#include "crond.h"
#include "scheduler.h"
#include "process.h"
#include "ipc.h"

#define FIFO_PATH "/tmp/crond_fifo"

int main()
{
    Job jobs[MAX_JOBS];

    int job_count = 0;
    int choice;

    printf(
        "=============================================\n"
    );

    printf(
        "        CronD - Job Scheduler Daemon\n"
    );

    printf(
        "=============================================\n\n"
    );

    printf(
        "CronD started successfully.\n"
    );

    /*
     * Install POSIX signal handlers.
     */
    setup_signal_handlers();

    while (1)
    {
        printf(
            "\n\n============== CronD Menu ==============\n"
        );

        printf(
            "1. Add Job\n"
        );

        printf(
            "2. List Jobs\n"
        );

        printf(
            "3. Run Job\n"
        );

        printf(
            "4. Cancel Job\n"
        );

        printf(
            "5. Check Status\n"
        );

        printf(
            "6. Test Anonymous Pipe\n"
        );

        printf(
            "7. Create FIFO\n"
        );

        printf(
            "8. Send Signal\n"
        );

        printf(
            "9. Process Group Info\n"
        );

        printf(
            "10. Create New Session\n"
        );

        printf(
            "11. Exit\n"
        );

        printf(
            "========================================\n"
        );

        printf(
            "Enter choice: "
        );

        if (
            scanf(
                "%d",
                &choice
            ) != 1
        )
        {
            printf(
                "Invalid input. Please enter a number.\n"
            );

            while (
                getchar() != '\n'
            )
                ;

            continue;
        }

        switch (choice)
        {
            case 1:

                add_job(
                    jobs,
                    &job_count,
                    MAX_JOBS
                );

                break;


            case 2:

                list_jobs(
                    jobs,
                    job_count
                );

                break;


            case 3:
            {
                int id;

                printf(
                    "Enter Job ID to run: "
                );

                scanf(
                    "%d",
                    &id
                );

                if (
                    id >= 1 &&
                    id <= job_count
                )
                {
                    run_job(
                        &jobs[id - 1]
                    );
                }
                else
                {
                    printf(
                        "Invalid Job ID.\n"
                    );
                }

                break;
            }


            case 4:
            {
                int id;

                printf(
                    "Enter Job ID to cancel: "
                );

                scanf(
                    "%d",
                    &id
                );

                if (
                    id >= 1 &&
                    id <= job_count
                )
                {
                    cancel_process(
                        &jobs[id - 1]
                    );
                }
                else
                {
                    printf(
                        "Invalid Job ID.\n"
                    );
                }

                break;
            }


            case 5:

                check_processes(
                    jobs,
                    job_count
                );

                list_jobs(
                    jobs,
                    job_count
                );

                break;


            case 6:

                printf(
                    "\n[IPC] Testing anonymous pipe...\n"
                );

                create_anonymous_pipe();

                break;


            case 7:

                printf(
                    "\n[IPC] Creating named pipe...\n"
                );

                create_fifo(
                    FIFO_PATH
                );

                printf(
                    "[IPC] FIFO path: %s\n",
                    FIFO_PATH
                );

                printf(
                    "[IPC] Use another terminal to test read/write.\n"
                );

                break;


            case 8:
            {
                int pid;
                int signal_number;

                printf(
                    "Enter PID: "
                );

                scanf(
                    "%d",
                    &pid
                );

                printf(
                    "Enter signal number "
                    "(10=SIGUSR1, 15=SIGTERM): "
                );

                scanf(
                    "%d",
                    &signal_number
                );

                send_job_signal(
                    pid,
                    signal_number
                );

                break;
            }


            case 9:

                printf(
                    "\n========== Process Group Information ==========\n"
                );

                printf(
                    "CronD PID  : %d\n",
                    getpid()
                );

                printf(
                    "CronD PGID : %d\n",
                    getpgrp()
                );

                printf(
                    "CronD SID  : %d\n",
                    getsid(0)
                );

                printf(
                    "===============================================\n"
                );

                break;


            case 10:

                printf(
                    "\n[IPC] Creating a new session...\n"
                );

                create_new_session();

                break;


            case 11:

                printf(
                    "\nCronD shutting down...\n"
                );

                unlink(
                    FIFO_PATH
                );

                return 0;


            default:

                printf(
                    "Invalid choice. Please select 1-11.\n"
                );
        }
    }

    return 0;
}
