#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/types.h>
#include <wait.h>

/* Create a program that forks a new shell.
 * From that child shell, send a signal to the 
 * parent and let the parent handle that signal.
 *
 * NOTE: IDK if this is the correct way to go about
 * this. Upon trying to test things out, there seems
 * to be a few issues.
 *
 * When the child sends a Signal to the parent, it 
 * causes an error with wait().
 */

static void sig_handler(int);
static void * my_sigaction(int, void (*)(int));

int
main(void)
{
	pid_t pid;
	int status;

	// watch for signal.
	if (my_sigaction(SIGUSR1, sig_handler) == SIG_ERR)
	{
		printf("error: catching signal\n");
		exit(EXIT_FAILURE);
	}

	pid = fork();

	if (pid < 0)
	{
		printf("error: forking\n");
		exit(EXIT_FAILURE); 
	} 

	else if (pid == 0) 
	{ 
		printf("Child PID: %d\n", getpid());
		status = execlp("alacritty", "terminal", NULL); 
		if (status < 0) 
		{ 
			printf("error: executing\n"); 
			exit(EXIT_FAILURE); 
		} 

		exit(EXIT_SUCCESS);
	} 

	printf("Parent PID: %d\n", getpid());
	printf("Parent Doing something else\n");
	// This wait() function would fail due to an Interrupted system call
	// wait(&status);

	while (1) {}
}

static void sig_handler(int signo)
{
	printf("SIGUSR1 was sent\n");
}

static void * my_sigaction(int signo, void (*func)(int))
{
	struct sigaction sa, osa;
	sa.sa_handler = func;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);

	sigaction(signo, &sa, &osa);

	return osa.sa_handler;
}
