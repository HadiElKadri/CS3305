#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
//Author: Hadi El-Kadri
//This program will create 2 child processes from a parent proccess
//and a child within one of those child processes

int main(int argc, char** argv)
{
  //Check to see if path of external program is provided, exit if not
  if (argc != 2){
      printf("Invalid arguments!");
      exit(0);
  }
  
  //Create two process id objects
  pid_t pid1, pid2;
  
  //Create child1 process from parent process
  pid1 = fork();
  
  //If returned pid is < 0 child process not created
  if (pid1 < 0){
    printf("Creation of child process failed.");
  }
  
  //If in parent process, wait for child process to finish
  else if (pid1 > 0){
    wait(NULL);
    
    //Create child2 process 
    pid1 = fork();
    
    //If child 2 process fails to create 
    if (pid1 < 0) printf("Creation of child 2 failed.");
    
    //If child2 is created wait for child2 process to finish before parent
    else if (pid1 > 0) {
      wait(NULL);
    }
    
    else {
      
    //Jump to external program and terminate process
    printf("parent process (PID %d) created child_2 (PID %d) \n", getppid(), getpid());
    
    printf("child_2 (PID %d) is calling an external program external_program.out and leaving child_2..\n", getpid());
    char pid2_str[20];
    int char2 = getpid();
    snprintf(pid2_str, 19, "%d", char2);
    execl(argv[1], "external_program.out", pid2_str, NULL);
    }
  }
  
  //In child1 process print statements about creations and ids
  else{
    
    printf("parent process (PID %d) created child_1 (PID %d) \n", getppid(), getpid());
    
    printf("parent (PID %d) is waiting for child_1 (PID %d) to complete before creating child_2\n", getppid(), getpid());
    
    //Create a child1.1 process within child1 process 
    pid2 = fork();
    
    //If child 1.1 process failed 
    if (pid2 < 0) printf("Creation of child process failed.");
    
    //Wait for child1.1 process before proceeding with child1 process
    else if (pid2 >0) {
      wait(NULL);
      //When child 1.1 process is complete print statement
      printf("child_1 (PID %d) is now complete\n", getpid());
    }
    
    //Print completion of child1.1
    else{
      printf("child_1 (PID %d) created child_1.1 (PID %d)\n", getppid(), getpid());
    }

  }
}
