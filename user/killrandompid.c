#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
    int i, pid;

    printf(1, "Starting killrandom test. Creating 5 background child processes...\n");

    for (i = 0; i < 5; i++)
    {
        pid = fork();
        if (pid == 0)
        {
            // Child process
            printf(1, "Child process %d is alive.\n", getpid());
            for (;;)
                sleep(100);
        }
    }

    sleep(10);

    pid = killrandom();
    if (pid < 0)
    {
        printf(1, "No process was killed.\n");
    }
    else
    {
        printf(1, "Killed process with PID: %d\n", pid);
    }
    exit();
}