#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() { 
   pid_t pid, mypid, myppid; 
   
   pid = getpid(); 
   printf("Before fork: Process id is %d\n", pid); 
   
   pid = fork(); 
 
   if (pid < 0) { 
      perror("fork() failure\n"); 
      return 1; 
   } 
   
   if (pid == 0) { 
      // This is child process 
      printf("This is child process\n"); 
      mypid = getpid(); 
      myppid = getppid(); 
      printf("Process id is %d and PPID is %d\n", mypid, myppid); 
   } else { 
      // This is parent process  
      sleep(2); // Wait for child to print first
      printf("This is parent process\n"); 
      mypid = getpid(); 
      myppid = getppid(); 
      printf("Process id is %d and PPID is %d\n", mypid, myppid); 
      printf("Newly created process id or child pid is %d\n", pid); 
   } 
   return 0; 
}





#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
    pid_t cpid;
    if (fork() == 0) {
        exit(0); // Child terminates immediately
    } else {
        cpid = wait(NULL); // Parent waits and reaps the child
        printf("Parent pid = %d\n", getpid());
        printf("Child pid = %d\n", cpid);
    }
    return 0;
}







#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void waitexample() { 
    int i;  
    int stat; 
    pid_t pid[5]; 

    // Creation Loop
    for (i = 0; i < 5; i++) { 
        if ((pid[i] = fork()) == 0) { 
            sleep(1); 
            exit(100 + i); // Exit with 100, 101, 102, 103, 104
        } 
    } 

    // Reaping Loop using waitpid
    for (i = 0; i < 5; i++) { 
        pid_t cpid = waitpid(pid[i], &stat, 0); 
        if (WIFEXITED(stat)) {
            printf("Child %d terminated with status: %d\n", 
                    cpid, WEXITSTATUS(stat)); 
        }
    } 
}  

int main() { 
    waitexample(); 
    return 0; 
}
















#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys.h>
#include<wait.h>

int main()
{
    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } else if (pid == 0) {
        // Child process
        printf("Child Process: PID = %d, Parent PID = %d\n", getpid(), getppid());
        exit(42); // Child exits with status 42
    } else {
        // Parent process
        int status;
        waitpid(pid, &status, 0); // Wait for child to finish
        if (WIFEXITED(status)) {
            int exit_status = WEXITSTATUS(status);
            printf("Parent Process: Child exited with status = %d\n", exit_status);
        } else {
            printf("Parent Process: Child did not terminate normally\n");
        }
    }
    

    return 0
}