#include <unistd.h>
#include <stdio.h>
#include <signal.h>

 /* 
 * A simple program that demonstrates how handle 
 * signals with the signal() function.
 */

 /* 
 * To Use run the following commands:
 *
 * $ gcc main.c
 * $ ./a.out &
 * $ pkill -USR1 a.out      send SIGUSR1
 * $ pkill -uSR2 a.out      send SIGUSR2
 * $ pkill a.out            send SIGTERM
 */

 /*  
 *  SIGUSR1 == 10
 *  SIGUSR2 == 12
 *
 *  Look in /usr/include/signal.h for more definitions.
 */

// This is our signal handler called when a signal is thrown.
static void sig_usr(int);
int main()
{
     /*  
     *  signal() takes your desired signal (e.g. SIGUSRX) 
     *  and waits for it to happen.
     *
     *  When it happens, signal() then passes that signal 
     *  to your function to handle like so: sig_user(SIGUSRX)
     *
     *  Your function handles it however you defined it to.
     *
     *  Signal returns a __sighandler_t type.
     *
     * For more information, google "higher order functions"
     */

    // __sighandler_t is defined in /usr/include/signal.h
    void * my_signal1 = (int*) signal(SIGUSR1, sig_usr);
    __sighandler_t my_signal2 = signal(SIGUSR2, sig_usr);

    // Run this if you don't ever want it to stop (Unless you turn off your computer).
    // __sighandler_t term_signal = signal(SIGTERM, sig_usr);

     // If sig_usr returns an error, print this. 
    if (my_signal1 == SIG_ERR)
        printf("Can't catch SIGUSR1");

    // If sig_usr returns an error, print this.
    else if (my_signal2 == SIG_ERR)
        printf("can't catch SIGUSR2\n");
    
    /* 
    * Until we "kill" the program with "kill -USR1 pid" or "pkill -USR1 a.out"
    *
    * Sleep until a signal is delivered that either terminates
    * the process or causes the invocation of a signal-catching 
    * function.
    *
    * "man pause" for more information.
    * Also defined in /usr/include/unistd.h
    */
    for ( ; ; )
        pause();
}

static void sig_usr(int signo)
{
    // If signal == SIGUSR1 (i.e. 10)
    // Archieved with the flag "-USR1"
    if (signo == SIGUSR1)
        printf("received SIGUSR1\n");

    // If signal == SIGUSR2 (i.e. 12)
    // Archieved with the flag "-USR2"
    else if (signo == SIGUSR2)
        printf("received SIGUSR2\n");
    else 
        printf("received signal %d\n", signo);
}
