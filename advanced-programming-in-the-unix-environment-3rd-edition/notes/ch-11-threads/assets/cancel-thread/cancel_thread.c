#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


int count = 0;

void cleanup(void * arg)
{
	printf("Cleanup\n");
}
void * 
thread_func(void * arg)
{

	for (int i = 0; i < 5; i++)
	{
		count++;
	}

	pthread_exit((void *) 1);
}

int
main(void)
{
	pthread_t tid;
	int status;
	void * ret_val;

	status = pthread_create(&tid, NULL, thread_func, &ret_val);

	status = pthread_join(tid, &ret_val);
	if (status < 0)
	{
		perror("");
		exit(EXIT_FAILURE);
	}

	printf("%d\n", count);

	exit(EXIT_SUCCESS);
}
