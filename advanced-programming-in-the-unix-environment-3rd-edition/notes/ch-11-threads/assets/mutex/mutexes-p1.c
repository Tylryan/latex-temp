#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/* Using a signle thread, get this
 * code to work.
 */

struct foo 
{
	int 		 f_count;
	pthread_mutex_t  f_lock;
};

struct foo *
foo_alloc(void)
{
	struct foo * fp;
	fp = malloc(sizeof(struct foo));
	// Keep track of f_lock
	int res = pthread_mutex_init(&fp->f_lock, NULL);

	if (res != 0)
	{
		free(fp);
		return (NULL);
	}

	return fp;
};

void
foo_hold(struct foo * fp)
{
	// Lock f_lock
	pthread_mutex_lock(&fp->f_lock);
	// Do something
	fp->f_count++;
	// Unlock f_lock
	pthread_mutex_unlock(&fp->f_lock);
}

void
foo_release(struct foo * fp)
{
	/* "--fp->f_count" is interesting code.
	 *
	 * What it does is pre-decrement the actual
	 * f_count amount by 1.
	 *
	 * if fp->f_count was 1, then --fp->f_count
	 * would == 0.
	 *
	 * If that is the case, then we can go ahead and
	 * do the following:
	 * 1. Unlock it
	 * 2. Destroy it (make it uninitialized)
	 * 3. Free it's memory
	 */

	if (--fp->f_count == 0)
	{
		pthread_mutex_unlock(&fp->f_lock);
		pthread_mutex_destroy(&fp->f_lock);
		free(fp);
		fp == NULL;
	}
	/* However, if there are multiple foo objects,
	 * then just unlock this particular one.
	 */
	else
	{
		pthread_mutex_unlock(&fp->f_lock);
	}
}

int
main(void)
{
	/* Note: For the sake of simplicity, this example
	 * is only showing how to use these functions with
	 * 1 thread.
	 */
	struct foo * fpointer = foo_alloc();

	// Get 200
	for (int i = 0; i < 200; i++)
	{
		foo_hold(fpointer);
	}

	printf("200: %d\n", fpointer->f_count); // 200
	// Release all but 1
	for (int i = 0; i < 199; i++)
	{
		foo_release(fpointer);
	}

	printf("1: %d\n", fpointer->f_count); // 1
	
	// Release the last one
	foo_release(fpointer);
	// Invalid memory
	printf("Destroyed: %d\n", fpointer->f_count);

	exit(EXIT_SUCCESS);
}
