#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>


int glob   = 6;     // External variable in initialized data
char buf[] = "a write to stdout\n";

int main()
{
    int var; // Automatic variable on the stack.
    pid_t pid;

    var = 88;

    // Not flushing STDOUT
    int result = write(STDOUT_FILENO, buf, sizeof(buf) -1);
    if (result != sizeof(buf) -1)
    {
        perror("error");
    }

    printf("Before Fork\n");

    pid = fork();

    if (pid < 0)
    {
        perror("Error");
        exit(EXIT_FAILURE);
    }

    else if (pid == 0) // Child Process
    {
        // Modifies these variables.
        glob++;
        var++;
    }
    else  // Parent Process
    {
        // Making parent sleep for 2 seconds
        // This will increase the likelyhood of the Child
        // executing first.
        sleep(2);
    }

    printf("pid = %d, glob = %d, var = %d\n", getpid(), glob, var);
    exit(EXIT_SUCCESS);
}
