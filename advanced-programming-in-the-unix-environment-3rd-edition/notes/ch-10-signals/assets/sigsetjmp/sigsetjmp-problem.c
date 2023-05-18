#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>

static int loop_number = 0;
static void usr1_handler(int);
static void * my_sigaction(int, void (*)(int));
static sigjmp_buf usr1_env;

int
main(void)
{
	printf("Program Start\n");
	printf("PID: %d\n", getpid());
	my_sigaction(SIGUSR1, usr1_handler);

	int res = sigsetjmp(usr1_env, 0);
	if (res == 0)
		printf("Equals %d\n", res);
	else 
		printf("Equals %d\n", res);

	while (1) {}
}

static void
usr1_handler(int signo)
{
	printf("Handled Signal!\n");
}

static void *
my_sigaction(int signo, void (*func)(int))
{
	struct sigaction sa, osa;
	sa.sa_handler = func;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

	if (sigaction(signo, &sa, &osa) != 0)
	{
		printf("error: catching signal\n");
		exit(EXIT_FAILURE);
	}

	return osa.sa_handler;
}
