/****************
LE2: Introduction to Unnamed Pipes
****************/
#include <unistd.h> // pipe, fork, dup2, execvp, close
#include <stdio.h>
#include <stdlib.h>
using namespace std;

int main () {
    // lists all the files in the root directory in the long format
    char* cmd1[] = {(char*) "ls", (char*) "-al", (char*) "/", nullptr};
    // translates all input from lowercase to uppercase
    char* cmd2[] = {(char*) "tr", (char*) "a-z", (char*) "A-Z", nullptr};

    // TODO: add functionality
    // Create pipe
    int fd[2];
    pipe(fd);
    if (pipe(fd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Create child to run first command
    pid_t child1 = fork();
    if (child1 == 0) {
        dup2(fd[1], STDOUT_FILENO);     // In child, redirect output to write end of pipe
        close(fd[0]);     // Close the read end of the pipe on the child side.
        close(fd[1]);       //child 1 doesn't read
        execvp(cmd1[0], cmd1);       // In child, execute the command
    }
    else if (child1 == -1) { perror("fork"); exit(EXIT_FAILURE); }

    // Create another child to run second command
    pid_t child2 = fork();
    if (child2 == 0) {
        dup2(fd[0], STDIN_FILENO);      // In child, redirect input to the read end of the pipe
        close(fd[1]);               // Close the write end of the pipe on the child side.
        close(fd[0]);              //child 2 doesn't write, also write must be closed to allow EOF
        execvp(cmd2[0], cmd2);      // Execute the second command.
    }
    else if (child2 == -1) { perror("fork"); exit(EXIT_FAILURE); }

    
    // Reset the input and output file descriptors of the parent.
    close(fd[0]);
    close(fd[1]);

}
