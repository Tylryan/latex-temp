#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

/* Recreate the signal() function using sigaction
 */

static void * my_signal(int, void (*)(int));
static void usr1_handler(int);

int
main()
{
	my_signal(SIGUSR1, usr1_handler);
    // run "kill -USR1 <pid>" in another terminal
	printf("PID: %d\n", getpid());

	while (1) { }

	exit(EXIT_SUCCESS);
}

static void
usr1_handler(int signo)
{
	printf("Handled SIGUSR1\n");
}

static void * 
my_signal(int signo, void (* func)(int))
{
	struct sigaction sa, osa;
	sa.sa_handler = func;
	sa.sa_flags   = 0;

	sigemptyset(&sa.sa_mask);

	if (sigaction(signo, &sa, &osa) < 0)
    {
        printf("error: catching signal\n");
        exit(EXIT_FAILURE);
    }

	return osa.sa_handler;
}
