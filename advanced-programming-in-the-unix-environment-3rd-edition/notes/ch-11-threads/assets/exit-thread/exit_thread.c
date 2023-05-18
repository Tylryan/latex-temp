#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

/* A simple program that shows how to
 * obtain the exit status and the 
 * return value of a thread using either
 * "return" or "pthread_exit()"
 */

void *
thread_func1(void * arg)
{
	printf("thread 1 returning\n");
	return ((void*)1);
}

// See if you can return a char *
// I'd prefer this one
void *
thread_func2(void * arg)
{
	printf("thread 2 exiting\n");
	// To return a char *
	//pthread_exit("HELLER\n");
	pthread_exit((void*)2);
}

int
main(void)
{
	pthread_t  thread1, thread2;
	int 	   err;
	void 	 * thread_return;

	// Create thread 1, which returns 1
	err = pthread_create(&thread1, NULL, thread_func1, NULL);
	if (err !=0)
	{
		printf("error: can't create thread 1: %s\n", strerror(err));
		exit(EXIT_FAILURE);
	}

	// Create thread 2, which returns 2 using pthread_exit()
	err = pthread_create(&thread2, NULL, thread_func2, NULL);
	if (err != 0)
	{
		printf("error: can't create thread 2: %s\n", strerror(err));
		exit(EXIT_FAILURE);
	}

	/* Waits for thread 1 and returns the exit status to err, 
	 * and your whatever thread_func1() returned to thread_return.
	 */
	err = pthread_join(thread1, &thread_return);
	// err == 0
	// thread_return == 1

	if (err !=0)
	{
		printf("error: can't join with thread 1: %s\n", strerror(err));
		exit(EXIT_FAILURE);
	}

	printf("thread 1 exit code: %d\n", (int*) thread_return);

	/* Waits for thread 2 and returns the exit status to err, 
	 * and your whatever thread_func2() returned to thread_return.
	 */
	err = pthread_join(thread2, &thread_return);
	// err == 0
	// thread_return == 2

	if (err !=0)
	{
		printf("error: can't join with thread 2: %s\n", strerror(err));
		exit(EXIT_FAILURE);
	}

	printf("thread 2 exit code: %d\n", (int*) thread_return);
	// To return a char *
	//printf("thread 2 exit code: %s\n", (char*) thread_return);

	exit(EXIT_SUCCESS);
}
