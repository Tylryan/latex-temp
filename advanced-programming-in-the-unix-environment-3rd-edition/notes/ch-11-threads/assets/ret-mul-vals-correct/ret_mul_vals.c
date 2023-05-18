#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

/* Write a program that returns a struct
 * (i.e. multiple values) from a thread
 * without using malloc.
 */

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

// Create a function struct within the thread and returns it
// Note, you don't actually need "void * arg" to be here.
// In fact, if you make warnings errors, then you wouldn't
// be able to compile with it there.
//
// To make it work and still have the explicit parameter type,
// just remove "arg". 
//
// i.e. thread_func1(void *)
void *
thread_func1(void * arg)
{
	// If you don't make this static, foo will die when
	// this thread dies.
	static struct foo foo = {
		.a = 1,
		.b = 2,
		.c = 3,
		.d = 4
	};
	// Proof foo exists within the thread
	print_foo("thread 1:\n", &foo);
	pthread_exit((void *)&foo);
}

void *
thread_func2(void * arg)
{
	printf("thread 2: ID is %ld\n", pthread_self());
	pthread_exit((void*)1);
}

void print_error(char * msg, int err)
{
	printf("%s: %s\n", msg, strerror(err));
	exit(EXIT_FAILURE);
}

int
main(void)
{
	int 	  err;
	struct 	  foo *fp;
	pthread_t tid1, tid2;

	// Creates a foo struct within this thread
	err = pthread_create(&tid1, NULL, thread_func1, NULL);
	if (err != 0) print_error("", err);

	/* Returns a pointer to the foo struct (which
	 * is located on the heap) here.
	 */
	err = pthread_join(tid1, (void*)&fp);
	if (err != 0) print_error("", err);

	sleep(1);
	printf("parent starting second thread\n");

	err = pthread_create(&tid2, NULL, thread_func2, NULL);
	if (err != 0) print_error("", err);

	sleep(1);
	print_foo("parent:\n", fp);

	exit(EXIT_SUCCESS);
}
