#include <bits/types/sigset_t.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

/* A program that does the following:
 * 1. Checks for SIGTSTP using signal()
 * 2. Adds signal to the set of signals to be watched for.
 * 3. Blocks that set of signals (i.e. SIGTSTP) for a set 
 *    amount of time.
 * 4. Unblocks those signals 
 */

static void sig_quit(int);

int main()
{
    sigset_t newmask, oldmask, pendmask;

    /* Again, signal() is just used for learning purposes because it's easier
     * to use than sigaction.
     * Don't use it in real life.
     */

    if (signal(SIGTSTP, sig_quit) == SIG_ERR)
    {
        printf("can't catch SIGTSTP\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    // Add SIGTSTP to sigmask
    sigemptyset(&newmask);
    sigaddset(&newmask, SIGTSTP);


    // Declare that you wish to block all signals in sigmask
    int block_result = sigprocmask(SIG_BLOCK, &newmask, &oldmask);
    printf("Blocking all specified signals\n");
    if (block_result < 0)
    {
        printf("SIG_BLOCK error\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    printf("Sleeping\n");
    sleep(5);

    // Place all blocked and or waiting signals into pendmask
    if (sigpending(&pendmask) < 0)
    {
        printf("sigpending error\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }
    // Checks to see if SIGTSTP is in pendmask
    if (sigismember(&pendmask, SIGTSTP))
    {
        printf("\nSIGTSTP is a pending signal\n");
    }

    // Reset the mask to the old mask. I.e. Don't block anymore
    int result = sigprocmask(SIG_SETMASK, &oldmask, NULL);
    printf("Unblocking all signals\n");

    if (result < 0)
    {
        printf("SIG_SETMASK error\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    printf("SIGTSTP is now unblocked\n");
    fflush(stdout);
    sleep(5);
    printf("Program ended\n");

    exit(EXIT_SUCCESS);
}

static void 
sig_quit(int signo)
{
    printf("caught SIGTSTP with the signal handler\n");

    if (signal(SIGTSTP, SIG_DFL) == SIG_ERR)
    {
        printf("can't reset SIGTSTP");
        exit(EXIT_FAILURE);
    }
}
