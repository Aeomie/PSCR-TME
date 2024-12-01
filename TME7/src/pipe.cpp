#include <ostream>
#include <iostream>
#include <semaphore>
#include <unistd.h>

#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char **argv)
{
    int tubeDesc[2];
    pid_t pid_fils;

    // Step 1: Identify the '|' symbol and split the arguments
    int i = 0;
    for (i; i < argc; i++)
    {
        if (!strcmp(argv[i], "|"))
        {
            argv[i] = nullptr;
            break;
        }
    }

    if (i == argc)
    {
        exit(1);
    }

    // Create the pipe
    if (pipe(tubeDesc) == -1)
    {
        perror("pipe");
        exit(1);
    }

    pid_fils = fork();
    // Fork to create a child process
    if (pid_fils == -1)
    {
        perror("fork");
        exit(2);
    }

    if (pid_fils == 0)
    { // Child process
        // Redirect stdout to the write end of the pipe
        dup2(tubeDesc[1], STDOUT_FILENO);

        // Close both ends of the pipe in the child process
        close(tubeDesc[1]);
        close(tubeDesc[0]);

        if (execvp(argv[1], argv + 1) == -1)
        {
            perror("execv");
            exit(3);
        }
    }
    else
    { // Parent process
        // Redirect stdin to the read end of the pipe
        dup2(tubeDesc[0], STDIN_FILENO);

        // Close both ends of the pipe in the parent process
        close(tubeDesc[0]);
        close(tubeDesc[1]);

        if (execvp(argv[i + 1], argv + i + 1) == -1)
        {
            perror("execv");
            exit(3);
        }
    }

    return EXIT_SUCCESS;
}
