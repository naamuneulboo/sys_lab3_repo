#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    pid_t pid;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s command [args...]", argv[0]);
        exit(1);
    }

    pid = fork();
    if (pid == 0)
    {
        execvp(argv[1], &argv[1]);
        perror("exec failed");
        exit(1);
    }
    else if (pid > 0)
    {
        wait((int *) 0);
    }
    else
    {
        perror("fork failed");
        exit(1);
    }

    return 0;
}


