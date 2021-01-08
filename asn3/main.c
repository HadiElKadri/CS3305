/*
AUTHOR: HADI EL-KADRI
Date: October 18, 2020
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

int x, y, z;

void *sum()
{
    z = y + x;
    return NULL;
}

int main()
{
    pid_t child;
    pthread_t thread;

    // Initializing the global variables
    x = 10, y = 20, z = 0;

    // create child process
    child = fork();

    // check for child creation error
    if (child < 0)
    {
        printf("Error creating child.");
        exit(0);
    }

    // in parent:
    // wait for child, and print value of z
    else if (child > 0)
    {
        wait(NULL);
        printf("Using a fork(), the value of z in the parent process is: %d\n", z);

        // create thread, wait for it to complete, then print value of z
        if (pthread_create(&thread, NULL, sum, NULL) != 0)
            printf("main function: errno number is %d\n", errno);
        ;
        pthread_join(thread, NULL);
        printf("Using a thread, the value of z is: %d\n", z); //
    }

    // in child:
    // sum x and y and store it in z
    // print value of z
    else
    {
        sum();
        printf("Using a fork(), the value of z in the child process is: %d\n", z);
    }

    return 0;
}
