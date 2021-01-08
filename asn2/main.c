/* AUTHOR: Hadi El-Kadri 
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

// Reading and writing end of the pipe
#define READ_END 0
#define WRITE_END 1

int main(int argc, char **argv)
{
    /*************************************** command line arguments ***************************************
		To compile assignment2.c: gcc assignment2.c -o assignment2
		To run your program: ./assignment2 "CS" "3305" " is fun!"
		argv[0] = "./assignment2"
		argv[1] = "CS"
		argv[2] = "3305"
		argv[3] = " is fun!"

		Number of arguments (argc) = 4

		In this assignment, the parent process reads argv[1] and the child process reads argv[2] and argv[3]
		e.g. In the parent process: char *x = argv[1];
	*******************************************************************************************************/

    // If the user does not pass X, Y and Z, the program will terminate
    if (argc != 4)
    {
        printf("Invalid arguments\n");
        exit(0);
    }

    //Initializing variables
    pid_t pid;
    int port[2];
    char *x, *y, *z;

    //Pipe and check to see if pipe was successful exit if not
    if (pipe(port) < 0)
    {
        printf("Pipe error!");
        exit(0);
    }

    //If pipe was successful
    else
    {
        printf("A pipe is created for communication between parent (PID %d) and child\n", getpid());
    }

    //Fork system call and create child function
    pid = fork();

    //If fork was unsuccessful exit program
    if (pid < 0)
    {
        printf("Fork error!");
        exit(0);
    }

    //If pid > 0 PARENT FUNCTION
    if (pid > 0)
    {
        //Print ids of parent and child
        printf("parent (PID %d) created a child (PID %d)\n", getpid(), pid);

        //Take the first input from user and have x point to it
        x = argv[1];

        //Character array to store x in for space
        char newx[100];
        strcat(newx, x);

        //Print x argument recieved from user
        printf("parent (PID %d) receives X = \"%s\" from the user\n", getpid(), newx);

        //Go on to child function
        wait(NULL);

        //Read what child has written into the pipe
        read(port[0], y, 100);

        //Print what is read from the pipe 
        printf("parent (PID %d) reads Y from the pipe (Y = \"%s\")\n", getpid(), y);

        //Take the variable from the pipe and concatenate it to the newx character array
        strcat(newx, y);

        //PRINT RESULT AND FINISH PROGRAM
        printf("parent (PID %d) concatenates X and Y to generate the string: %s\n", getpid(), newx);
    }

    else
    {
        //Have y equal to 2nd argument input from user
        y = argv[2];

        //Have z equal to 3rd argument input from user
        z = argv[3];
        
        //Print id of child and contents of pointers y and z
        printf("child (PID %d) receives Y = \"%s\" and Z = \"%s\" from the user\n", getpid(), y, z);

        //Concatenate z to y 
        strcat(y, z);

        //Print result of concatenation
        printf("child (PID %d) concatenates Y and Z to generate Y = %s\n", getpid(), y);

        //Write the concatenated string (of the 2 variables) into the pipe
        write(port[1], y, strlen(y) + 1);

        //Say that it was written into the pipe
        printf("child (PID %d) writes Y into the pipe\n", getpid());
    }

    return 0;
}
