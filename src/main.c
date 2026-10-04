#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "crond.h"
#include "scheduler.h"
#include "process.h"

int main()
{
    Job jobs[MAX_JOBS];

    int job_count = 0;
    int choice;

    printf("=============================================\n");
    printf("        CronD - Job Scheduler Daemon\n");
    printf("=============================================\n\n");

    printf("CronD started successfully.\n");

    while (1)
    {
        printf("\n\n============== CronD Menu ==============\n");
        printf("1. Add Job\n");
        printf("2. List Jobs\n");
        printf("3. Run Job\n");
        printf("4. Cancel Job\n");
        printf("5. Check Status\n");
        printf("6. Exit\n");
        printf("========================================\n");

        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");

            while (getchar() != '\n')
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

                printf("Enter Job ID to run: ");

                if (scanf("%d", &id) != 1)
                {
                    printf("Invalid Job ID.\n");

                    while (getchar() != '\n')
                        ;

                    break;
                }

                if (id >= 1 && id <= job_count)
                {
                    run_job(&jobs[id - 1]);
                }
                else
                {
                    printf("Invalid Job ID.\n");
                }

                break;
            }

            case 4:
            {
                int id;

                printf("Enter Job ID to cancel: ");

                if (scanf("%d", &id) != 1)
                {
                    printf("Invalid Job ID.\n");

                    while (getchar() != '\n')
                        ;

                    break;
                }

                if (id >= 1 && id <= job_count)
                {
                    cancel_process(&jobs[id - 1]);
                }
                else
                {
                    printf("Invalid Job ID.\n");
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

                printf("\nCronD shutting down...\n");

                return 0;

            default:

                printf("Invalid choice. Please select 1-6.\n");
        }
    }

    return 0;
}
