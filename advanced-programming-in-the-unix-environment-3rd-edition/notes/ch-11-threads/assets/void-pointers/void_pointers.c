#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


int
main(void)
{
	void * a = "Hello There";
	char * b = a;

	/* The (char *) cast is not requird here.
	 * Though cpp requires it. So for the 
	 * sake of portability, you should
	 * probably use it.
	 */

	// Hello There
	printf("(char *) a   : %s\n", (char *) a);
	// Hello There
	printf("b  	     : %s\n", b);

	/* Cast to char *, then dereference that.
	 *
	 * The cast is required here because we
	 * are dereferencing the void pointer.
	 */
	// H
	printf("*((char *) a): %c\n", *((char *) a));

	// Completely changing the type of a.
	a = (int *) 1;
	// IDK why, but you can't dereference a.
	printf("(int *) a    : %d\n", (int *) a); // 1

	// ------------------------------------------------------
	// int * acts just like an int in some cases
	printf("\nvoid * with primitives\n");
	void * one = (void*) 1;
	// 1 : Correct Method
	printf("(int *) one: %d\n", (int *) one);
	// 1 : Works
	printf("one 	   : %d\n", one);
	// 1: Works, but causes warning
	printf("(int) one  : %d\n", (int) one);
}
