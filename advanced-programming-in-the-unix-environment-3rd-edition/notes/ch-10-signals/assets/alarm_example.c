#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

#include <setjmp.h>

/* A program that takes in user input,
 * but only waits for X seconds to do it
 * before stopping.
 *
 * Shows setjmp.h library and alarm().
 *
 *
 * Note that this setjmp and longjmp are 
 * essentially like try catches.
 */

static jmp_buf env_alrm;
// Sets env_alrm == 1
static void sig_alrm(int);
int WAIT_DUR = 5;

int main()
{
    int  n;
    char line[BUFSIZ];

    // If SIGALRM is thrown, set env_alrm == 1
    if (signal(SIGALRM, sig_alrm) == SIG_ERR)
    {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    // When first ran setjmp() == 0, so it skips this.
    if (setjmp(env_alrm) != 0)
    {
        printf("Error: Read Timeout\n");
        exit(EXIT_FAILURE);
    }

    // Tell the kernel to throw SIGALRM after X seconds.
    alarm(WAIT_DUR);

    printf("Enter something: ");
    fflush(stdout);

    // Read user input
    n = read(STDIN_FILENO, line, BUFSIZ);
    if (n < 0)
    {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    // If the user enters something, then canecl that alarm
    alarm(0);

    // Print what they entered
    write(STDOUT_FILENO, line, n);

    exit(EXIT_SUCCESS);
}

// Sets env_alrm to 1
static void sig_alrm(int signo)
{
    printf("\nWaited for %d seconds!\n", WAIT_DUR);
    longjmp(env_alrm, 1);
}
