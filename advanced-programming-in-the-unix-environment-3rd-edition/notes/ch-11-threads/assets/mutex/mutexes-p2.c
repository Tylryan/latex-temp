#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

/* Using multiple threads,
 * get fp->f_count to equal to 200.
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
	 * would == 0
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
	}

	else 
	{
		pthread_mutex_unlock(&fp->f_lock);
	}
}

void 
empty_foo(struct foo * fp)
{
	int total = fp->f_count;

	for (int i = 0; i < total; i++)
	{
		foo_release(fp);
	}
}

void * 
thread_func(void * arg)
{
	for (int i = 0; i < 100; i++)
	{
		foo_hold(arg);
	}
}

int
main(void)
{
	struct foo * fp = foo_alloc();
	int 	  status;
	pthread_t tid1, tid2;

	// returns 0 upon success. I'm just not catching 
	// these errors for simplicity's sake.
	status = pthread_create(&tid1, NULL, thread_func, fp);
	status = pthread_create(&tid2, NULL, thread_func, fp);

	if (pthread_join(tid1, NULL) != 0)
	{
		printf("error: thread 1\n");
		exit(EXIT_FAILURE);
	}

	// Same thing as above with different syntax
	if (pthread_join(tid2, NULL))
	{
		printf("error: thread 2\n");
		exit(EXIT_FAILURE);
	}

	printf("200: %d\n", fp->f_count);

	// Unlock, Destroy, and Free structure memory
	empty_foo(fp);
	// Now has junk data
	printf("Destroyed: %d\n", fp->f_count);

	exit(EXIT_SUCCESS);
}
