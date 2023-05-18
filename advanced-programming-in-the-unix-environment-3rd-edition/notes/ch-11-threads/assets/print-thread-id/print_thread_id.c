#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <string.h>

/* Write a function that prints the current thread id 
 * */

pthread_t ntid;

/* Notice how similar the Hex numbers (address ranges) are
 * This means that they are near each other in memory.
 *
 * Running on Linux (Fedora 37), these are the results.
 *
 * PIDs are the same.
 *
 * TID addresses are different,
 *  First 4 positions are the same.
 *  Last 5 positions are the same.
 */
void 
print_tid(const char * s)
{
	pthread_t tid  = pthread_self();
	printf("%s (TID): %u, (HEX) %08x%x, (PID) %u\n", 
			s, 
			(unsigned int) tid,
			(unsigned int) tid,
			(unsigned int) getpid());
}

/* When you create a new thread, you must pass in a function.
 * In our case, our function just calls our print_tid() function.
 */
void *
thread_fn(void * arg)
{
	print_tid("new thread: ");

	// Note: In Linux, returning just 0 is allowable.
	return (void *) 0;
}

int
main(void)
{
	int 	err;

	// This will call a thread that will then print it's TID.
	err = pthread_create(&ntid, NULL, thread_fn, NULL);

	if (err != 0)
	{
		printf("error: can't create thread: %d\n", strerror(err));
		exit(EXIT_FAILURE);
	}

	// Print the main thread's TID
	print_tid("main thread: ");
	sleep(1);
	exit(EXIT_SUCCESS);
}
