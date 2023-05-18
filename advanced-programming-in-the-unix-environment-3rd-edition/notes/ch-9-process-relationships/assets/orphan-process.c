#include <signal.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>

/* This program creates an orphaned process by ending 
 * a parent process while it's child is stopped.
 */

// What to do if SIGHUP is sent
static void sig_hup(int signo)
{
    printf("SIGHUP received, pid = %d\n", getpid());
}

// Simple debug
static void pr_ids(char * name)
{
    printf("%s: pid = %d, ppid = %d, pgrp = %d, tpgrp = %d\n",
            name, getpid(), getppid(), 
            getpgrp(), tcgetpgrp(STDIN_FILENO));
    fflush(stdout);
}

int main()
{
    char  c;
    pid_t pid;

    // First to print
    pr_ids("parent");

    pid = fork();

    if (pid < 0)
    {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    // Parent
    else if (pid > 0) 
    {
        sleep(2);
        printf("Parent process ended\n");
        exit(EXIT_SUCCESS);
    }

    // Child 
    else 
    {

        // Second to print
        pr_ids("child");

        /* POSIX.1 requires that every process in the newly orphaned
         * process group that is STOPPED (as our child is) be sent 
         * the hang-up signal (SIGHUP) FOLLOWED BY THE CONTINUE
         * SIGNAL (SIGCONT).
         *
         * The default action of SIGHUP is to terminate the process.
         * This would terminate our program.
         *
         * signal() tells the program what to do in with a 
         * particular signal. Essentially allowing us to override 
         * the default action of any given signal.
         *
         * To override the default action of SIGHUP, we pass our 
         * sig_hup() function, which does not terminate the process, 
         * to signal(). Thus allowing it to continue.
         * 
         * NOTE: Without signal() or kill(), the child would still 
         * continue in the background even after the parent ended.
         *
         * In short, the kill() function is being ran and thus
         * our child is stopped. However, it becomes an orphan
         * after the parent process ends. Every stopped orphan 
         * must be continued. Thus is why our child continues.
         */

        // signal() looks to be a higher order function.
        signal(SIGHUP, sig_hup);
        printf("After signal()\n");

        printf("Program Stopped\n");
        kill(getpid(), SIGTSTP);
        printf("Continue after kill() (Being Stopped)\n");


        /* I believe this is trying to illustrate the fact that because
         * the this process is orphaned, it is sent to the background.
         * However, a background process cannot take input from the 
         * controlling terminal so it errors.
         *
         * To get this to work, you need to enter a single letter 
         * before the parent exits.
         */

        /* ssize_t status = read(STDIN_FILENO, &c, 1); */

        /* if (status != 1) */
        /*     printf("read error from controlling TTY, errno = %d\n", 
                                                     *     errno); */


        /* This demonstrates how the process is run in the background 
         * even after the parent ends.
         */

        printf("Continuing in the background.\n");
        sleep(3);
        pr_ids("\nchild");
        printf("Child process ends\n");
        printf(
        "Notice how the Parent PID (PPID) changes to 1 (ie. init)\n"
        );
        exit(EXIT_SUCCESS);
    }
}
