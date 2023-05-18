#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


struct foo { int a, b, c, d; } ;

void
print_foo(const char *s, const struct foo *fp)
{
	printf(s);
	printf(" structure at 0x%x\n", *(unsigned int*)fp);
	printf(" foo.a = %d\n", fp->a);
	printf(" foo.b = %d\n", fp->b);
	printf(" foo.c = %d\n", fp->c);
	printf(" foo.d = %d\n", fp->d);
}

void *
thread_func1(void * arg)
{
	// Creating a foo struct on this thread's stack
	struct foo foo = {1,2,3,4};
	print_foo("thread 1:\n", &foo);

	/* Because foo was not made static or put on the
	 * heap, it will die when we exit this function.
	 */
	pthread_exit((void*)&foo);
}

void *
thread_func2(void * arg)
{
	printf("thread 2: ID is %ld\n", pthread_self());
	pthread_exit((void*)0);
}

int
main(void)
{
	/* For the sake of simplicity, I'm not handling 
	 * any errors
	 */
	pthread_t tid1, tid2;
	struct    foo *fp;
	int 	  err;

	// CREATES a foo struct WITHIN this thread
	err = pthread_create(&tid1, NULL, thread_func1, NULL);

	/* Returns a pointer to the foo struct here.
	 * However, that thread has already been terminated.
	 * If you tried to access the value of fp which points
	 * to memory that has been terminated, you will get
	 * junk.
	 */
	err = pthread_join(tid1, (void*)&fp);

	sleep(1);

	printf("parent starting second thread\n");

	err = pthread_create(&tid2, NULL, thread_func2, NULL);

	sleep(1);
	print_foo("parent:\n", fp);

	exit(EXIT_SUCCESS);
}
