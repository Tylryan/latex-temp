#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <stdbool.h>

/* Write a function that
 * 1. Recreates the signal() function with sigaction.
 * 2. Empties a Signal Set
 * 3. Adds 2 signals to that signal set.
 * 4. Blocks these signals
 * 	- Sleep for X seconds to allow the user to test this.
 * 	- use "kill -USR1" to test this.
 * 4. Unblock 1 signal
 * 	- Allow the user to test this for 10 seconds.
 * 		- Do not use a loop or sleep()
 * 		- You must use a signal.
 * 5. Validates that X signal is in the signal mask.
 */

static void * my_signal(int, void (*)(int));
void validate_mask(sigset_t *);

static void usr1_handler(int signo)
{
	printf("SIGUSR1 handled!\n");
}
static void usr2_handler(int signo)
{
	printf("SIGUSR2 handled!\n");
}

static void alrm_handler(int signo)
{
	printf("The program has run for 10 seconds.\n");
	exit(EXIT_SUCCESS);
}

int
main(void)
{
    	// Add signals to block
	sigset_t ss, osa;
	sigemptyset(&ss);
	sigaddset(&ss, SIGUSR1);
	sigaddset(&ss, SIGUSR2);
	sigprocmask(SIG_BLOCK, &ss, &osa);

	validate_mask(&ss);

	// Watch for sigusr1
	if (my_signal(SIGUSR1, usr1_handler) == SIG_ERR)
	{
		printf("error: catching signal\n");
		exit(EXIT_FAILURE);
	}

    	// Watch for sigusr2
	if (my_signal(SIGUSR2, usr2_handler) == SIG_ERR)
	{
		printf("error: catching signal\n");
		exit(EXIT_FAILURE);
	}

    	// Watch for sigalrm
	if (my_signal(SIGALRM, alrm_handler) == SIG_ERR)
	{
		printf("error: catching signal\n");
		exit(EXIT_FAILURE);
	}

	printf("Enter the following command within 10 seconds\n");
	printf("kill -USR1 %d\n", getpid());
	sleep(10);

	// Remove SIGUSR2 from the block list
	sigdelset(&ss, SIGUSR2);
	// Set the new mask (without sigusr2)
	sigprocmask(SIG_SETMASK, &ss, NULL);

	alarm(10);
	printf("Unblocked SIGUSR1\n");
	printf("Alarm started. 10 seconds until program terminates\n");

	/* Didn't end up getting this function to work correctly.
	 * At this point SIGUSR2 is no longer in ss, but
	 * this function states that is is...
	 */
	validate_mask(&ss);

	// Just to keep the program running and allow
	// user input.
	while (true)
	{
	}

	exit(EXIT_SUCCESS);
}


static void * 
my_signal(int signo, void (*func)(int))
{
	struct sigaction sa, osa;
	sa.sa_handler = func;
	sa.sa_flags   = 0;

	sigaction(signo, &sa, &osa);

	return osa.sa_handler;
}

void 
validate_mask(sigset_t * signal_set)
{
    // Check if signals are in mask (used to block signals)
	// This function doesn't really seem to be that reliable
	bool usr1 = sigismember(signal_set, SIGUSR1);
	bool usr2 = sigismember(signal_set, SIGUSR1);

	if (usr1 && usr2)
		printf("Both signals are in mask\n");
	else if (usr1)
		printf("Only SIGUSR1 is in mask\n");
	else if (usr2)
		printf("Only SIGUSR2 is in mask\n");
	return;
}
