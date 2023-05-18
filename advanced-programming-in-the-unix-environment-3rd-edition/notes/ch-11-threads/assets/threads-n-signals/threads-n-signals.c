#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>

/* Write a program that creates a thread that
 * continues indefinitely.
 *
 * Set an alarm for X seconds. After which,
 * the thread should be force terminated.
 *
 * For the sake of simplicity, signal() is allowed.
 */

static void 
alrm_handler(int signo)
{
	printf("Alarm handled\n");
	pthread_kill(pthread_self(), SIGTERM);
}

void * 
thread_func()
{
	printf("Thread is doing something...\n");
	// A thread that does something for a while
	while (1) { }

	pthread_exit((void*)0);
}

int
main(void)
{
	pthread_t tid;
	void *  ret_val;
	int 	status;

	printf("Alarm Set\n");
	signal(SIGALRM, alrm_handler);
	alarm(5);
	status = pthread_create(&tid, NULL, thread_func, NULL);

	status = pthread_join(tid, &ret_val);
	if (status != 0)
	{
		printf("Error");
		exit(EXIT_FAILURE);
	}

	alarm(0);

	exit(EXIT_SUCCESS);
}
