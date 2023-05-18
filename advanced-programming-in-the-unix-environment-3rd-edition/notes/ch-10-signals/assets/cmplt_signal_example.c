#include <bits/types/__sigset_t.h>
#include <bits/types/sigset_t.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

// Define 2 signals (hint User Defined Signals)
#define MY_SIGNAL1 SIGUSR1
#define MY_SIGNAL2 SIGUSR2

// Define 2 signals (hint User Defined Signals)
// Add those signals to a signal mask
// Block those signals
// Watch for those signals using sigaction
// Send those signals to the process
//      Prove that they are blocked
// Check and see which signals are pending
// Unblock one of the signals
// Using sigaction, handle that signal
//      Prove that signal is unblocked and the other is blocked.
// Reset your signal mask
static void my_signal1_handler(int signal);
static void my_signal2_handler(int signal);
void check_pending(sigset_t * pending_mask);
void try_sending_signals();

int
main(void)
{
    // Watch for signals
    struct sigaction sa, osa;
    sa.sa_handler = my_signal1_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;

    struct sigaction sa2, osa2;
    sa2.sa_handler = my_signal2_handler;
    sigemptyset(&sa2.sa_mask);
    sa2.sa_flags = 0;

    sigaction(MY_SIGNAL1, &sa, NULL);
    sigaction(MY_SIGNAL2, &sa2, NULL);

    // Add those signals to a signal mask
    sigset_t newmask, oldmask, pendmask;
    sigemptyset(&newmask);
    sigaddset(&newmask, MY_SIGNAL1);
    sigaddset(&newmask, MY_SIGNAL2);

    // Block those signals
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);

    printf("Blocked Both Signals\n");
    try_sending_signals();
    printf("Kill signals were blocked\n");

    printf("\nChecking Pending Signals\n");
    check_pending(&pendmask);

    // Unblock one of the signals
    printf("\nUnblocking My_Signal1\n");
    printf("This should cause the pending SIGNAL1 to be handled\n");
    sigdelset(&newmask, MY_SIGNAL1);
    sigprocmask(SIG_SETMASK, &newmask, NULL);

    printf("\n");
    try_sending_signals();

    // Reset mask (Unblock all)
    printf("\nUnblock All\n");
    printf("This should cause the pending SIGNAL2 to be handled\n");
    sigprocmask(SIG_SETMASK, &oldmask, NULL);

    printf("\nSend Both Signals\n");
    printf("Both signals should be handled\n");
    try_sending_signals();

    exit(EXIT_SUCCESS);
}

void try_sending_signals()
{
    printf("Tried to send MY_SIGNAL1\n");
    kill(getpid(), MY_SIGNAL1);
    printf("Tried to send MY_SIGNAL2\n");
    kill(getpid(), MY_SIGNAL2);
}

static void my_signal2_handler(int signal)
{
    printf("Signal 2 Was Handled\n");
}

static void my_signal1_handler(int signal)
{
    printf("Signal 1 Was Handled\n");

}

void check_pending(sigset_t * pendmask)
{
    sigpending(pendmask);

    if (sigismember(pendmask, MY_SIGNAL1))
        printf("MY_SIGNAL1 is pending\n");
    else 
        printf("MY_SIGNAL1 is not pending\n");

    if (sigismember(pendmask, MY_SIGNAL2))
        printf("MY_SIGNAL2 is pending\n");
    else 
        printf("MY_SIGNAL2 is not pending\n");
}
