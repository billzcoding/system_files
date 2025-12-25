#include <stdio.h>

int main() { 
   printf("I am EXEC.c called by an exec function\n"); 
   return 0; 
}

#include <stdio.h>
#include <unistd.h>

int main() { 
   char *args[] = {"./EXEC", NULL}; 
   execvp(args[0], args); 
   
   // This only prints if execvp fails
   printf("Ending-----\n"); 
   return 0; 
}






#include <stdio.h>
#include <unistd.h>

int main() { 
   char *args[] = {"./EXEC", NULL}; 
   execv(args[0], args); 
   
   printf("Ending-----\n"); 
   return 0; 
}



#include <stdio.h>
#include <unistd.h>

int main(void) { 
   char *binaryPath = "/bin/ls"; 
   char *arg1 = "-lh"; 
   char *arg2 = "/home"; 
   
   // Note: binaryPath is passed twice (once as path, once as arg0)
   execl(binaryPath, binaryPath, arg1, arg2, NULL); 
   
   return 0; 
}



#include <stdio.h>
#include <unistd.h>

int main(void) { 
   char *programName = "ls"; 
   char *arg1 = "-lh"; 
   char *arg2 = "/home"; 
   
   execlp(programName, programName, arg1, arg2, NULL); 
   
   return 0; 
}


#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 

int main() { 
   pid_t pid = fork(); 
   
   switch(pid) { 
      case -1: 
         perror("fork"); 
         exit(1); 
      case 0: 
         printf("Child (PID: %d) exiting...\n", getpid());
         exit(0); // Child becomes a Zombie now
      default: 
         printf("Parent (PID: %d) sleeping. Check 'ps -l' for Z status.\n", getpid());
         sleep(5); 
         execlp("ps", "ps", "-l", (char *)NULL); 
   } 
   return 0;
}



#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h> 

int main() { 
   pid_t pid = fork(); 
   
   switch(pid) { 
      case -1: 
         perror("fork"); 
         exit(1); 
      case 0: 
         printf("Child starting. My Parent is: %d\n", getppid());
         sleep(2); // Wait for parent to die
         printf("Child. My NEW Parent is: %d (init)\n", getppid());
         execlp("ps", "ps", "-f", (char *)NULL); 
         break;
      default: 
         printf("Parent exiting...\n");
         exit(0); // Parent dies, making child an orphan
   } 
   return 0;
}


#include <stdio.h>
#include <stdlib.h> 
#include <unistd.h> 
#include <string.h> 
#include <sys/wait.h> 

int main(int argc, char *argv[]) { 
   printf("hello world (pid:%d)\n", (int) getpid()); 
   int rc = fork(); 
   
   if (rc < 0) { 
      fprintf(stderr, "fork failed\n"); 
      exit(1); 
   } else if (rc == 0) { 
      // Child process
      printf("hello, I am child (pid:%d)\n", (int) getpid()); 
      char *myargs[3]; 
      myargs[0] = strdup("wc");        // Word count command
      myargs[1] = strdup("EXEC.c");    // Run it on our EXEC.c file
      myargs[2] = NULL;        
      
      execvp(myargs[0], myargs); 
      printf("This will not print if execvp succeeds\n"); 
   } else { 
      // Parent process
      int wc = wait(NULL); 
      printf("hello, I am parent of %d (wc:%d) (pid:%d)\n", rc, wc, (int) getpid()); 
   } 
   return 0; 
}






#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(1);
    } 
    else if (pid == 0) {
        // CHILD: Transform into the 'wc' command
        printf("Child: Counting words in Lab8ex8.c...\n");
        
        // execlp(file, cmd_name, options, target_file, terminator)
        execlp("wc", "wc", "-w", "Lab8ex8.c", (char *)NULL);

        // If execlp succeeds, this part is never reached
        perror("exec failed");
        exit(1);
    } 
    else {
        // PARENT: Wait for the child to finish the count
        wait(NULL);
        printf("Parent: Child process finished the word count.\n");
    }

    return 0;
}





#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

int main() {
    int rc = fork();

    if (rc == 0) {
        // Child process
        char *myargs[4];
        myargs[0] = "wc";        // The command
        myargs[1] = "-l";        // Option: count lines
        myargs[2] = "EXEC.c";    // The file to check
        myargs[3] = NULL;        // The NULL terminator

        printf("Child: Running 'wc -l' on EXEC.c\n");
        execvp(myargs[0], myargs);
        
        // This only runs if execvp fails
        perror("execvp failed");
        exit(1);
    } 
    else {
        // Parent path
        wait(NULL);
        printf("Parent: Word count complete.\n");
    }

    return 0;
}







#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    printf("Main Program (PID:%d) starting...\n", (int)getpid());

    int rc = fork();

    if (rc < 0) {
        // Fork failed
        fprintf(stderr, "fork failed\n");
        exit(1);
    } 
    else if (rc == 0) {
        // --- CHILD PROCESS ---
        printf("Child (PID:%d): I am becoming the 'wc' command now.\n", (int)getpid());

        // Prepare the arguments for wc
        char *myargs[3];
        myargs[0] = strdup("wc");        // The command name
        myargs[1] = strdup("EXEC.c");    // THE TARGET: The file you want to count
        myargs[2] = NULL;                // End of array marker

        // This replaces the child's code with the 'wc' program code
        execvp(myargs[0], myargs);

        // If execvp works, the lines below will NEVER execute
        printf("This line will not print unless there is an error!\n");
    } 
    else {
        // --- PARENT PROCESS ---
        // The parent waits for the 'wc' command to finish printing to the screen
        wait(NULL); 
        printf("Parent (PID:%d): The child is done counting. I can continue now.\n", (int)getpid());
    }

    return 0;
}